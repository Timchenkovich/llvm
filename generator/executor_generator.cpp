#include <sourceCreator.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <regex>
#include <string>

namespace {
const auto NUM = std::regex("-?\\d+");
const auto TYPE = std::regex("i32|ptr|i1");
const auto PHI_INCOMING = std::regex("\\[.[\\d%\\s,\\-]*\\]");
const auto VAR = std::regex("%\\d+");
const std::array<std::string, 7> BINARY_INSTRS = {"add", "sub",  "mul", "shl",
                                                  "or",  "srem", "and"};
const auto BINARY_INSTR = std::regex("(add|sub|mul|shl|or|and|srem)");
const auto BINARY_MODS = std::regex("nsw|nuw");
const auto ICMP_P = std::regex("eq|ne|ugt|uge|ult|ule|sgt|sge|slt|sle");
const auto FUNC_NAME = std::regex("@[a-zA-Z_]+");
const auto BOOL = std::regex("true|false");

const auto commaJoin = [](auto acc, const auto& current) {
  if (!acc.empty()) {
    acc += ", ";
  }
  acc += current;
  return std::move(acc);
};
}  // namespace

struct generator {
  generator(const std::string& inputPath, const std::string& outputPath)
      : input(inputPath, std::ios_base::in), sourceCreator_(outputPath) {}

  void generatePreamble() {
    sourceCreator_.println("extern \"C\" {");
    sourceCreator_.println(" #include \"../sim/sim.h\"");
    sourceCreator_.println("}");
    sourceCreator_.println("#include <fstream>");
    sourceCreator_.println("#include <llvm/ExecutionEngine/GenericValue.h>");
    sourceCreator_.println("#include <llvm/ExecutionEngine/ExecutionEngine.h>");
    sourceCreator_.println("#include <llvm/ExecutionEngine/MCJIT.h>");
    sourceCreator_.println("#include <llvm/Support/TargetSelect.h>");
    sourceCreator_.println("#include <llvm/ADT/ArrayRef.h>");
    sourceCreator_.println("#include <llvm/IR/BasicBlock.h>");
    sourceCreator_.println("#include <llvm/IR/DerivedTypes.h>");
    sourceCreator_.println("#include <llvm/IR/Function.h>");
    sourceCreator_.println("#include <llvm/IR/IRBuilder.h>");
    sourceCreator_.println("#include <llvm/IR/LLVMContext.h>");
    sourceCreator_.println("#include <llvm/IR/Module.h>");
    sourceCreator_.println("#include <iostream>");

    sourceCreator_.println();
    sourceCreator_.println("using namespace llvm;");
    sourceCreator_.println();
    sourceCreator_.println("int main(int argc, char** argv) {");
    sourceCreator_.indent();

    sourceCreator_.println("if (argc > 2) {");
    sourceCreator_.indent();
    sourceCreator_.println(
        "std::cerr << \"Unexpected number of args, usage: executor "
        "[--dump].\" << std::endl;");
    sourceCreator_.println("return 1;");
    sourceCreator_.outdent();
    sourceCreator_.println("}");
    sourceCreator_.println();

    sourceCreator_.println("bool dumpOnly = false;");
    sourceCreator_.println("if (argc == 2) {");
    sourceCreator_.indent();
    sourceCreator_.println("std::string text = argv[1];");
    sourceCreator_.println(
        "if (text != \"--dump\") { std::cerr << \"Unexpected key.\" << "
        "std::endl; return 1; }");
    sourceCreator_.println("dumpOnly = true;");
    sourceCreator_.outdent();
    sourceCreator_.println("}");
    sourceCreator_.println();

    sourceCreator_.println("LLVMContext context;");
    sourceCreator_.println(
        "std::unique_ptr<Module> module = "
        "std::make_unique<Module>(\"generated\", "
        "context);");
    sourceCreator_.println("IRBuilder<> builder(context);");
    sourceCreator_.println("Type* voidType = Type::getVoidTy(context);");
    sourceCreator_.println();

    printSimFuncs();
  }

  void printSimFuncs() {
    sourceCreator_.println("Type* intType = Type::getInt32Ty(context);");

    sourceCreator_.println(
        "std::array<Type*, 3> simPutPixelArgsArr = {intType, intType, "
        "intType};");
    sourceCreator_.println(
        "ArrayRef<Type*> simPutPixelArgs = simPutPixelArgsArr;");
    sourceCreator_.println(
        "FunctionType* simPutPixelType = FunctionType::get(voidType, "
        "simPutPixelArgs, false);");
    sourceCreator_.println(
        "FunctionCallee simPutPixelFunc = "
        "module->getOrInsertFunction(\"simPutPixel\", simPutPixelType);");
    sourceCreator_.println();

    sourceCreator_.println(
        "FunctionType* simFlushType = FunctionType::get(voidType, "
        "false);");
    sourceCreator_.println(
        "FunctionCallee simFlushFunc = "
        "module->getOrInsertFunction(\"simFlush\", simFlushType);");
    sourceCreator_.println();

    sourceCreator_.println(
        "FunctionType* simRandType = FunctionType::get(intType, "
        "false);");
    sourceCreator_.println(
        "FunctionCallee simRandFunc = "
        "module->getOrInsertFunction(\"simRand\", simRandType);");
    sourceCreator_.println();
  }

  void generateExit() {
    sourceCreator_.println("return 0;");
    sourceCreator_.outdent();
    sourceCreator_.println("}");
  }

  size_t getParamCount() {
    return std::count(currentFuncDef.begin(), currentFuncDef.end(), '%');
  }

  std::string getFuncName() {
    size_t startPos = currentFuncDef.find('@') + 1;
    size_t endPos = currentFuncDef.find('(');
    return currentFuncDef.substr(startPos, endPos - startPos);
  }

  std::pair<std::string, size_t> getFuncTypes() {
    size_t argsStart = currentFuncDef.find('(') + 1;

    std::string args = currentFuncDef.substr(argsStart);
    std::vector<std::string> res;

    size_t cur_pos = 0;
    size_t cnt = 0;
    while (true) {
      size_t next_pos = args.find(',', cur_pos);
      if (next_pos == std::string::npos) {
        next_pos = args.find(')', cur_pos);
      }
      if (next_pos == std::string::npos) {
        return {
            std::accumulate(res.begin(), res.end(), std::string(), commaJoin),
            cnt};
      }

      std::string cur_arg = args.substr(cur_pos, next_pos - cur_pos);

      std::smatch match;
      std::regex_search(cur_arg, match, TYPE);
      if (!match.empty()) {
        res.push_back(getTypeGetter(match.str()));
        ++cnt;
      }

      cur_pos = next_pos + 1;
    }
  }

  void createFun() {
    std::string funcName = getFuncName();

    std::string funcNameVar = funcName + "FuncName";
    std::string argsArrVar = funcName + "ArgsArr";
    std::string argsVar = funcName + "Args";
    std::string funcTypeVar = funcName + "FuncType";
    std::string linkageVar = funcName + "Linkage";
    std::string funcVar = funcName + "Func";
    std::pair<std::string, size_t> funcTypesP = getFuncTypes();
    std::string funcTypes = funcTypesP.first;
    size_t funcTypesSize = funcTypesP.second;

    sourceCreator_.println(
        std::format("std::string {} = \"{}\";", funcNameVar, funcName));
    sourceCreator_.println(std::format("std::array<Type*, {}> {} = {{{}}};",
                                       funcTypesSize, argsArrVar, funcTypes));
    sourceCreator_.println(
        std::format("ArrayRef<Type*> {} = {};", argsVar, argsArrVar));
    sourceCreator_.println(std::format(
        "FunctionType* {} = FunctionType::get(voidType, {}, false);",
        funcTypeVar, argsVar));
    sourceCreator_.println(
        std::format("Function::LinkageTypes {} = {};", linkageVar,
                    funcName == "app" ? "Function::ExternalLinkage"
                                      : "Function::InternalLinkage"));
    sourceCreator_.println(std::format(
        "Function* {} = Function::Create({}, {}, {}, module.get());", funcVar,
        funcTypeVar, linkageVar, funcNameVar));
    sourceCreator_.println();
  }

  void createBasicBlocks() {
    size_t paramCount = getParamCount();
    std::string funcName = getFuncName();
    std::string funcVar = funcName + "Func";

    auto doCreate = [this, &funcVar](size_t num) {
      sourceCreator_.println(std::format(
          "BasicBlock* bb{} = BasicBlock::Create(context, \"\", {});", num,
          funcVar));
    };

    doCreate(paramCount);
    for (const auto& line : currentBody) {
      size_t colonPos = line.find(':');
      if (colonPos == std::string::npos) {
        continue;
      }
      size_t blockNum = std::atoll(line.substr(0, colonPos).c_str());
      doCreate(blockNum);
    }

    sourceCreator_.println();
  }

  void createBr(const std::string& instr) {
    size_t i1Pos = instr.find("i1");

    std::smatch match;

    if (i1Pos == std::string::npos) {
      size_t percentPos = instr.find('%');
      std::string rest = instr.substr(percentPos);

      std::regex_search(rest, match, NUM);

      std::string bbNum = match.str();
      sourceCreator_.println(std::format("builder.CreateBr(bb{});", bbNum));

      return;
    }

    std::string rest = instr.substr(i1Pos + 2);
    std::regex_search(rest, match, NUM);
    std::string val = match.str();
    rest = match.suffix();
    std::regex_search(rest, match, NUM);
    std::string bb0 = match.str();
    rest = match.suffix();
    std::regex_search(rest, match, NUM);
    std::string bb1 = match.str();
    rest = match.suffix();

    sourceCreator_.println(
        std::format("builder.CreateCondBr(val{}, bb{}, bb{});", val, bb0, bb1));
  }

  void changeInsertPoint(const std::string& line) {
    size_t pos = line.find(':');
    size_t bbNum = std::atoll(line.substr(0, pos).c_str());

    changeInsertPoint(bbNum);
  }

  void changeInsertPoint(size_t num) {
    sourceCreator_.println(std::format("builder.SetInsertPoint(bb{});", num));
  }

  void createRet() { sourceCreator_.println("builder.CreateRetVoid();"); }

  std::string getConstantGetter(const std::string& type) {
    if (type == "i32") {
      return "builder.getInt32";
    }

    if (type == "i1") {
      return "builder.getInt1";
    }

    return "";
  }

  std::string getVarRefOrConstant(const std::string& input,
                                  const std::string& type) {
    std::smatch match;
    if (std::regex_search(input, match, VAR)) {
      return "val" + match.str().substr(1);
    } else if (std::regex_search(input, match, NUM) ||
               std::regex_search(input, match, BOOL)) {
      return std::format("{}({})", getConstantGetter(type), match.str());
    }

    return "";
  }

  void createPhi(const std::string& line) {
    std::string rest = line;
    std::smatch match;

    std::regex_search(line, match, NUM);
    std::string resNum = match.str();
    rest = match.suffix();

    std::regex_search(rest, match, TYPE);
    std::string type = match.str();
    std::string typeGetter = getTypeGetter(type);
    rest = match.suffix();

    std::vector<std::string> incomingStrs;

    while (std::regex_search(rest, match, PHI_INCOMING)) {
      std::string incoming = match.str();
      std::string first = incoming.substr(0, incoming.find(','));
      std::string second = incoming.substr(incoming.find(',') + 1);

      rest = match.suffix();

      std::string value = getVarRefOrConstant(first, type);

      incoming = match.suffix();
      std::regex_search(second, match, VAR);
      std::string bb = "bb" + match.str().substr(1);
      incomingStrs.push_back(
          std::format("val{}->addIncoming({}, {});", resNum, value, bb));
    }

    sourceCreator_.println(
        std::format("PHINode* val{} = builder.CreatePHI({}, {});", resNum,
                    typeGetter, incomingStrs.size()));
    incomings.insert(incomings.end(), std::move_iterator(incomingStrs.begin()),
                     std::move_iterator(incomingStrs.end()));
  }

  void dumpIncomings() {
    for (const auto& incoming : incomings) {
      sourceCreator_.println(incoming);
    }

    incomings.clear();
  }

  std::string getTypeGetter(const std::string& type) {
    if (type == "i32") {
      return "Type::getInt32Ty(context)";
    }

    if (type == "ptr") {
      return "PointerType::get(context, 0)";
    }

    if (type == "i1") {
      return "Type::getInt1Ty(context)";
    }

    return "";
  }

  void createBinary(const std::string& line) {
    std::string rest = line;
    std::smatch match;

    std::regex_search(rest, match, VAR);
    std::string valNum = match.str().substr(1);
    rest = match.suffix();

    std::regex_search(rest, match, BINARY_INSTR);
    std::string name = match.str();
    rest = match.suffix();

    std::string creator;
    if (name == "add") {
      creator = "CreateAdd";
    } else if (name == "sub") {
      creator = "CreateSub";
    } else if (name == "mul") {
      creator = "CreateMul";
    } else if (name == "shl") {
      creator = "CreateShl";
    } else if (name == "or") {
      creator = "CreateOr";
    } else if (name == "srem") {
      creator = "CreateSRem";
    } else if (name == "and") {
      creator = "CreateAnd";
    }

    bool hasNuw = false;
    bool hasNsw = false;
    while (std::regex_search(rest, match, BINARY_MODS)) {
      std::string mod = match.str();
      rest = match.suffix();

      hasNsw |= (mod == "nsw");
      hasNuw |= (mod == "nuw");
    }

    std::regex_search(rest, match, TYPE);
    std::string type = match.str();
    rest = match.suffix();

    size_t commaPos = rest.find(',');

    std::array<std::string, 2> inputs;
    inputs[0] = rest.substr(0, commaPos);
    inputs[1] = rest.substr(commaPos + 1);
    std::array<std::string, 2> vals;
    for (size_t i = 0; i < 2; ++i) {
      vals[i] = getVarRefOrConstant(inputs[i], type);
    }

    if (hasNsw || hasNuw) {
      sourceCreator_.println(
          std::format("Value* val{} = builder.{}({}, {}, \"\", {}, {});",
                      valNum, creator, vals[0], vals[1], hasNuw, hasNsw));
    } else {
      sourceCreator_.println(
          std::format("Value* val{} = builder.{}({}, {}, \"\");", valNum,
                      creator, vals[0], vals[1]));
    }
  }

  void createStore(const std::string& line) {
    std::string rest = line;
    std::smatch match;

    std::regex_search(rest, match, TYPE);
    std::string type = match.str();
    rest = match.suffix();

    size_t commaPos = rest.find(',');
    std::string first = rest.substr(0, commaPos);
    std::string second = rest.substr(commaPos + 1);
    std::string val = getVarRefOrConstant(first, type);
    std::string ptr = getVarRefOrConstant(second, "ptr");

    sourceCreator_.println(
        std::format("builder.CreateStore({}, {});", val, ptr));
  }

  std::string getPredicate(std::string pred) {
    std::string res = "CmpInst::Predicate::ICMP_";
    std::transform(pred.begin(), pred.end(), std::back_insert_iterator(res),
                   toupper);
    return res;
  }

  void createIcmp(const std::string& line) {
    std::smatch match;
    std::string rest = line;

    std::regex_search(rest, match, VAR);
    std::string num = match.str().substr(1);
    rest = match.suffix();

    std::regex_search(rest, match, ICMP_P);
    std::string pred = match.str();
    rest = match.suffix();

    std::string convPred = getPredicate(pred);

    std::regex_search(rest, match, TYPE);
    std::string type = match.str();
    rest = match.suffix();

    size_t commaPos = rest.find(',');
    std::string first = rest.substr(0, commaPos);
    std::string second = rest.substr(commaPos + 1);

    std::string left = getVarRefOrConstant(first, type);
    std::string right = getVarRefOrConstant(second, type);
    sourceCreator_.println(
        std::format("Value* val{} = builder.CreateICmp({}, {}, {});", num,
                    convPred, left, right));
  }

  void createLoad(const std::string& line) {
    std::string rest = line;
    std::smatch match;

    std::regex_search(rest, match, VAR);
    std::string num = match.str().substr(1);
    rest = match.suffix();

    std::regex_search(rest, match, TYPE);
    std::string type = match.str();
    rest = match.suffix();

    std::string ptr = getVarRefOrConstant(rest, "ptr");
    sourceCreator_.println(
        std::format("Value* val{} = builder.CreateLoad({}, {});", num,
                    getTypeGetter(type), ptr));
  }

  void createTailCall(const std::string& line) {
    std::smatch match;
    std::string rest = line;
    std::string assignee;

    size_t eqPos = rest.find('=');
    if (eqPos != std::string::npos) {
      std::regex_search(rest, match, VAR);
      assignee = match.str().substr(1);
      rest = match.suffix();
    }

    std::regex_search(rest, match, FUNC_NAME);
    std::string funcName = match.str().substr(1);
    rest = match.suffix();

    std::vector<std::string> args;
    while (std::regex_search(rest, match, TYPE)) {
      std::string type = match.str();
      rest = match.suffix();

      size_t endPos = rest.find(',');
      if (endPos == std::string::npos) {
        endPos = rest.find(')');
      }
      std::string current = rest.substr(0, endPos);
      rest = rest.substr(endPos + 1);

      args.push_back(getVarRefOrConstant(current, type));
    }

    std::string argsFolded =
        std::accumulate(args.begin(), args.end(), std::string(), commaJoin);

    // sourceCreator_.println("ArrayRef<Value*> args{} = {{{}}};", )
    if (assignee.empty()) {
      sourceCreator_.println("{");
      sourceCreator_.indent();

      sourceCreator_.println(
          std::format("CallInst* call = builder.CreateCall({}, {{{}}});",
                      funcName + "Func", argsFolded));
      sourceCreator_.println("call->setTailCall(true);");
      sourceCreator_.outdent();
      sourceCreator_.println("}");
    } else {
      sourceCreator_.println(
          std::format("CallInst* val{} = builder.CreateCall({}, {{{}}});",
                      assignee, funcName + "Func", argsFolded));
      sourceCreator_.println(
          std::format("val{}->setTailCall(true);", assignee));
    }
  }

  void createSelect(const std::string& line) {
    std::string rest = line;
    std::smatch match;

    std::regex_search(rest, match, VAR);
    std::string num = match.str().substr(1);
    rest = match.suffix();

    std::array<std::string, 3> args;
    for (size_t i = 0; i < 3; ++i) {
      std::regex_search(rest, match, TYPE);
      std::string type = match.str();
      rest = match.suffix();

      size_t endPos = rest.find(',');
      std::string current = rest.substr(0, endPos);
      if (endPos != std::string::npos) {
        rest = rest.substr(endPos + 1);
      }

      args[i] = getVarRefOrConstant(current, type);
    }

    std::string argsJoined =
        std::accumulate(args.begin(), args.end(), std::string(), commaJoin);
    sourceCreator_.println(std::format(
        "Value* val{} = builder.CreateSelect({});", num, argsJoined));
  }

  void createSwitch(const std::vector<std::string>& lines) {
    std::string rest = lines.front();
    std::smatch match;

    std::regex_search(rest, match, TYPE);
    std::string type = match.str();
    rest = match.suffix();

    std::regex_search(rest, match, VAR);
    std::string val = match.str().substr(1);
    rest = match.suffix();

    std::regex_search(rest, match, VAR);
    std::string bb = match.str().substr(1);
    rest = match.suffix();

    std::vector<std::string> curCases;
    for (size_t i = 1; i < lines.size(); ++i) {
      rest = lines[i];

      std::regex_search(rest, match, TYPE);
      std::string curType = match.str();
      rest = match.suffix();

      std::regex_search(rest, match, NUM);
      std::string constant = match.str();
      rest = match.suffix();

      std::regex_search(rest, match, VAR);
      std::string label = match.str().substr(1);

      curCases.push_back(std::format("sw{}->addCase({}({}), bb{});",
                                     numSwitches, getConstantGetter(curType),
                                     constant, label));
    }

    sourceCreator_.println(
        std::format("SwitchInst* sw{} = builder.CreateSwitch(val{}, bb{}, {});",
                    numSwitches, val, bb, curCases.size()));

    cases.insert(cases.end(), std::move_iterator(curCases.begin()),
                 std::move_iterator(curCases.end()));
    ++numSwitches;
  }

  void createInstructions() {
    changeInsertPoint(getParamCount());

    for (size_t i = 0; i < currentBody.size(); ++i) {
      const auto& line = currentBody[i];
      if (line.find(':') != std::string::npos) {
        changeInsertPoint(line);
        continue;
      }

      if (line.find("br") != std::string::npos) {
        createBr(line);
        continue;
      }

      if (line.find("ret") != std::string::npos) {
        createRet();
        continue;
      }

      if (line.find("phi") != std::string::npos) {
        createPhi(line);
        continue;
      }

      if (line.find("store") != std::string::npos) {
        createStore(line);
        continue;
      }

      if (line.find("icmp") != std::string::npos) {
        createIcmp(line);
        continue;
      }

      if (line.find("load") != std::string::npos) {
        createLoad(line);
        continue;
      }

      if (line.find("tail call") != std::string::npos) {
        createTailCall(line);
        continue;
      }

      if (line.find("select") != std::string::npos) {
        createSelect(line);
        continue;
      }

      if (line.find("switch") != std::string::npos) {
        std::vector<std::string> lines;
        lines.push_back(line);
        std::string curStr = currentBody[++i];
        while (curStr.find(']') == std::string::npos) {
          lines.push_back(std::move(curStr));
          curStr = currentBody[++i];
        }
        createSwitch(lines);
        continue;
      }

      for (const auto& instr : BINARY_INSTRS) {
        if (line.find(instr) != std::string::npos) {
          createBinary(line);
          break;
        }
      }
    }

    sourceCreator_.println();
  }

  void printArgs() {
    size_t argc = getParamCount();
    std::string funcName = getFuncName();
    for (size_t i = 0; i < argc; ++i) {
      sourceCreator_.println(std::format(
          "Value* val{} = {}Func->args().begin() + {};", i, funcName, i));
    }
  }

  void dumpCases() {
    for (const auto& cs : cases) {
      sourceCreator_.println(cs);
    }

    numSwitches = 0;
    cases.clear();
  }

  void generateFun() {
    sourceCreator_.println(
        std::format("// defining {} function", getFuncName()));
    sourceCreator_.println("{");
    sourceCreator_.indent();

    createBasicBlocks();
    printArgs();
    createInstructions();
    dumpIncomings();
    dumpCases();

    sourceCreator_.outdent();
    sourceCreator_.println("}");
    sourceCreator_.println();
  }

  void readFunc() {
    currentBody.clear();

    std::string cur_line;
    while (std::getline(input, cur_line)) {
      if (cur_line == "}") {
        break;
      }
      currentBody.push_back(std::move(cur_line));
    }
  }

  void generateExecutor() {
    sourceCreator_.println("InitializeNativeTarget();");
    sourceCreator_.println("InitializeNativeTargetAsmPrinter();");
    sourceCreator_.println();

    sourceCreator_.println("if (dumpOnly) {");
    sourceCreator_.indent();
    sourceCreator_.println("std::error_code ec;");
    sourceCreator_.println("raw_fd_stream debugOut(\"generated.ll\", ec);");
    sourceCreator_.println("module->print(debugOut, nullptr);");
    sourceCreator_.println("debugOut.close();");
    sourceCreator_.println("return 0;");
    sourceCreator_.outdent();
    sourceCreator_.println("}");

    sourceCreator_.println(
        "ExecutionEngine* ee = EngineBuilder(std::move(module)).create();");
    sourceCreator_.println(
        "if (!ee) { std::cerr << \"Failed creating execution engine.\" << "
        "std::endl; return 1; }");
    sourceCreator_.println(
        "ee->InstallLazyFunctionCreator([=](const std::string& fn) -> void* {");
    std::array<std::string, 3> fnNames = {"simPutPixel", "simFlush", "simRand"};
    for (size_t i = 0; i < 3; ++i) {
      sourceCreator_.println(std::format(" if (fn == \"{}\") {{", fnNames[i]));
      sourceCreator_.println(
          std::format("   return reinterpret_cast<void*>({});", fnNames[i]));
      sourceCreator_.println(" }");
    }
    sourceCreator_.println(" return nullptr;");
    sourceCreator_.println("});");
    sourceCreator_.println("ee->finalizeObject();");
    sourceCreator_.println();
    sourceCreator_.println("simInit();");
    sourceCreator_.println("GenericValue res = ee->runFunction(appFunc, {});");
    sourceCreator_.println("simExit();");
  }

  void generate() {
    generatePreamble();

    auto forAllDo = [this](const auto& action) {
      std::string cur_line;
      while (std::getline(input, cur_line)) {
        if (cur_line.find("define") != std::string::npos) {
          currentFuncDef = std::move(cur_line);

          action();
        }
      }
    };

    forAllDo([this] { createFun(); });

    input.clear();
    input.seekg(0);

    forAllDo([this] {
      readFunc();
      generateFun();
    });

    generateExecutor();
    generateExit();
  }

  size_t numSwitches = 0;
  std::vector<std::string> cases;
  std::vector<std::string> incomings;
  std::string currentFuncDef;
  std::vector<std::string> currentBody;
  std::ifstream input;
  sourceCreator sourceCreator_;
  size_t cur = 0;
};

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Expected <LL-file> <output-file> arguments provided.";
    return 1;
  }

  std::string input = argv[1];
  std::string output = argv[2];
  generator gen{input, output};
  gen.generate();
  return 0;
}