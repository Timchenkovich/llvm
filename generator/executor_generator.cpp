#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <fstream>
#include <ios>
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

const auto commaJoin = [](auto acc, const auto& current) {
  if (!acc.empty()) {
    acc += ", ";
  }
  acc += current;
  return std::move(acc);
};
}  // namespace

struct generator {
  generator()
      : input("app.ll", std::ios_base::in),
        output("executor.cpp", std::ios_base::out) {}

  void indent() { numWs += 2; }

  void outdent() { numWs -= 2; }

  void println(const std::string& text = "") {
    std::string ws(numWs, ' ');
    output << ws << text << '\n';
  }

  void generatePreamble() {
    println("extern \"C\" {");
    println(" #include \"../sim/sim.h\"");
    println("}");
    println("#include <fstream>");
    println("#include <llvm/ExecutionEngine/GenericValue.h>");
    println("#include <llvm/ExecutionEngine/ExecutionEngine.h>");
    println("#include <llvm/Support/TargetSelect.h>");
    println("#include <llvm/ADT/ArrayRef.h>");
    println("#include <llvm/IR/BasicBlock.h>");
    println("#include <llvm/IR/DerivedTypes.h>");
    println("#include <llvm/IR/Function.h>");
    println("#include <llvm/IR/IRBuilder.h>");
    println("#include <llvm/IR/LLVMContext.h>");
    println("#include <llvm/IR/Module.h>");
    println();
    println("using namespace llvm;");
    println();
    println("int main() {");
    indent();
    println("LLVMContext context;");
    println(
        "std::unique_ptr<Module> module = "
        "std::make_unique<Module>(\"generated\", "
        "context);");
    println("IRBuilder<> builder(context);");
    println("Type* voidType = Type::getVoidTy(context);");
    println();

    printSimFuncs();
  }

  void printSimFuncs() {
    println("Type* intType = Type::getInt32Ty(context);");

    println("ArrayRef<Type*> simPutPixelArgs = {intType, intType, intType};");
    println(
        "FunctionType* simPutPixelType = FunctionType::get(voidType, "
        "simPutPixelArgs, false);");
    println(
        "FunctionCallee simPutPixelFunc = "
        "module->getOrInsertFunction(\"simPutPixel\", simPutPixelType);");
    println();

    println(
        "FunctionType* simFlushType = FunctionType::get(voidType, "
        "false);");
    println(
        "FunctionCallee simFlushFunc = "
        "module->getOrInsertFunction(\"simFlush\", simFlushType);");
    println();

    println(
        "FunctionType* simRandType = FunctionType::get(intType, "
        "false);");
    println(
        "FunctionCallee simRandFunc = "
        "module->getOrInsertFunction(\"simRand\", simRandType);");
    println();
  }

  void generateExit() {
    println("return 0;");
    outdent();
    println("}");
  }

  size_t getParamCount() {
    return std::count(currentFuncDef.begin(), currentFuncDef.end(), '%');
  }

  std::string getFuncName() {
    size_t startPos = currentFuncDef.find('@') + 1;
    size_t endPos = currentFuncDef.find('(');
    return currentFuncDef.substr(startPos, endPos - startPos);
  }

  std::string getFuncTypes() {
    size_t argsStart = currentFuncDef.find('(') + 1;
    size_t argsEnd = currentFuncDef.find(')');

    std::string args = currentFuncDef.substr(argsStart, argsEnd);
    std::vector<std::string> res;

    size_t cur_pos = 0;
    while (true) {
      size_t next_pos = args.find(',', cur_pos);
      if (next_pos == std::string::npos) {
        next_pos = args.find(')', cur_pos);
      }
      if (next_pos == std::string::npos) {
        return std::accumulate(res.begin(), res.end(), std::string(),
                               commaJoin);
      }

      std::string cur_arg = args.substr(cur_pos, next_pos - cur_pos);

      std::smatch match;
      std::regex_search(cur_arg, match, TYPE);
      if (!match.empty()) {
        res.push_back(getTypeGetter(match.str()));
      }

      cur_pos = next_pos + 1;
    }
  }

  void createFun() {
    std::string funcName = getFuncName();

    std::string funcNameVar = funcName + "FuncName";
    std::string argsVar = funcName + "Args";
    std::string funcTypeVar = funcName + "FuncType";
    std::string linkageVar = funcName + "Linkage";
    std::string funcVar = funcName + "Func";
    std::string funcTypes = getFuncTypes();

    println(std::format("std::string {} = \"{}\";", funcNameVar, funcName));
    println(std::format("ArrayRef<Type*> {} = {{{}}};", argsVar, funcTypes));
    println(std::format(
        "FunctionType* {} = FunctionType::get(voidType, {}, false);",
        funcTypeVar, argsVar));
    println(std::format("Function::LinkageTypes {} = {};", linkageVar,
                        funcName == "app" ? "Function::ExternalLinkage"
                                          : "Function::InternalLinkage"));
    println(std::format(
        "Function* {} = Function::Create({}, {}, {}, module.get());", funcVar,
        funcTypeVar, linkageVar, funcNameVar));
    println();
  }

  void createBasicBlocks() {
    size_t paramCount = getParamCount();
    std::string funcName = getFuncName();
    std::string funcVar = funcName + "Func";

    auto doCreate = [this, &funcVar](size_t num) {
      println(std::format(
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

    println();
  }

  void createBr(const std::string& instr) {
    size_t i1Pos = instr.find("i1");

    std::smatch match;

    if (i1Pos == std::string::npos) {
      size_t percentPos = instr.find('%');
      std::string rest = instr.substr(percentPos);

      std::regex_search(rest, match, NUM);

      std::string bbNum = match.str();
      println(std::format("builder.CreateBr(bb{});", bbNum));

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

    println(
        std::format("builder.CreateCondBr(val{}, bb{}, bb{});", val, bb0, bb1));
  }

  void changeInsertPoint(const std::string& line) {
    size_t pos = line.find(':');
    size_t bbNum = std::atoll(line.substr(0, pos).c_str());

    changeInsertPoint(bbNum);
  }

  void changeInsertPoint(size_t num) {
    println(std::format("builder.SetInsertPoint(bb{});", num));
  }

  void createRet() { println("builder.CreateRetVoid();"); }

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
    } else if (std::regex_search(input, match, NUM)) {
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

    println(std::format("PHINode* val{} = builder.CreatePHI({}, {});", resNum,
                        typeGetter, incomingStrs.size()));
    incomings.insert(incomings.end(), std::move_iterator(incomingStrs.begin()),
                     std::move_iterator(incomingStrs.end()));
  }

  void dumpIncomings() {
    for (const auto& incoming : incomings) {
      println(incoming);
    }

    incomings.clear();
  }

  std::string getTypeGetter(const std::string& type) {
    if (type == "i32") {
      return "Type::getInt32Ty(context)";
    }

    if (type == "ptr") {
      return "Type::getInt64Ty(context)->getPointerTo()";
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
      println(std::format("Value* val{} = builder.{}({}, {}, \"\", {}, {});",
                          valNum, creator, vals[0], vals[1], hasNuw, hasNsw));
    } else {
      println(std::format("Value* val{} = builder.{}({}, {}, \"\");", valNum,
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

    println(std::format("builder.CreateStore({}, {});", val, ptr));
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
    println(std::format("Value* val{} = builder.CreateICmp({}, {}, {});", num,
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
    println(std::format("Value* val{} = builder.CreateLoad({}, {});", num,
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

    // println("ArrayRef<Value*> args{} = {{{}}};", )
    if (assignee.empty()) {
      println("{");
      indent();

      println(std::format("CallInst* call = builder.CreateCall({}, {{{}}});",
                          funcName + "Func", argsFolded));
      println("call->setTailCall(true);");
      outdent();
      println("}");
    } else {
      println(std::format("CallInst* val{} = builder.CreateCall({}, {{{}}});",
                          assignee, funcName + "Func", argsFolded));
      println(std::format("val{}->setTailCall(true);", assignee));
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
    println(std::format("Value* val{} = builder.CreateSelect({});", num,
                        argsJoined));
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

    println(
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

    println();
  }

  void printArgs() {
    size_t argc = getParamCount();
    std::string funcName = getFuncName();
    for (size_t i = 0; i < argc; ++i) {
      println(std::format("Value* val{} = {}Func->args().begin() + {};", i,
                          funcName, i));
    }
  }

  void dumpCases() {
    for (const auto& cs : cases) {
      println(cs);
    }

    numSwitches = 0;
    cases.clear();
  }

  void generateFun() {
    println(std::format("// defining {} function", getFuncName()));
    println("{");
    indent();

    createBasicBlocks();
    printArgs();
    createInstructions();
    dumpIncomings();
    dumpCases();

    outdent();
    println("}");
    println();
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
    println("InitializeNativeTarget();");
    println("InitializeNativeTargetAsmPrinter();");
    println();
    println("std::error_code ec;");
    println("raw_fd_stream debugOut(\"generated.ll\", ec);");
    println("module->print(debugOut, nullptr);");
    println("debugOut.close();");
    println("ExecutionEngine* ee = EngineBuilder(std::move(module)).create();");
    println(
        "ee->InstallLazyFunctionCreator([=](const std::string& fn) -> void* {");
    std::array<std::string, 3> fnNames = {"simPutPixel", "simFlush", "simRand"};
    for (size_t i = 0; i < 3; ++i) {
      println(std::format(" if (fn == \"{}\") {{", fnNames[i]));
      println(
          std::format("   return reinterpret_cast<void*>({});", fnNames[i]));
      println(" }");
    }
    println(" return nullptr;");
    println("});");
    println("ee->finalizeObject();");
    println();
    println("simInit();");
    println("GenericValue res = ee->runFunction(appFunc, {});");
    println("simExit();");
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
  size_t numWs = 0;
  std::ifstream input;
  std::ofstream output;
  size_t cur = 0;
};

int main() {
  generator gen;
  gen.generate();
  return 0;
}