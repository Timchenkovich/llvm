#include "isaCreator.h"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <numeric>
#include <stdexcept>

#include "isaParser.h"
#include "sourceCreator.h"

namespace {
std::string joinDelim(const std::vector<std::string>& v, std::string d) {
  return std::accumulate(v.begin(), v.end(), std::string(),
                         [d](std::string cur, const std::string& next) {
                           if (cur.empty()) {
                             return next;
                           }

                           return std::move(cur) + d + next;
                         });
}

};  // namespace

isaCreator::isaCreator(std::string isaName,
                       const std::string& emulatingFnsFName,
                       const std::string& emulatingInstrCreatorFName)
    : isaName_(std::move(isaName)),
      emulatorHeader_(emulatingFnsFName + ".h"),
      emulatorSource_(emulatingFnsFName + ".cpp"),
      visitorHeader_(emulatingInstrCreatorFName + ".h"),
      visitorSource_(emulatingInstrCreatorFName + ".cpp"),
      parserGrammar_(getParserGrammarName() + ".g4"),
      lexerGrammar_(getLexerGrammarName() + ".g4") {}

void isaCreator::enterIsa(isaParser::IsaContext* ctx) {
  generateVisitorPreamble();
  generateEmulatorPreamble();
  generateLexerPreamble();
  generateParserPreamble();
}

void isaCreator::finishVisitor() {
  visitorHeader_.outdent();
  visitorHeader_.println("};");
}

void isaCreator::exitIsa(isaParser::IsaContext* ctx) {
  finishVisitor();
  finishEmulator(ctx);
  finishParser(ctx);
}

void isaCreator::finishEmulator(isaParser::IsaContext* ctx) {
  generateFunctionCreator(ctx);
  generateModuleRegistrator(ctx);

  emulatorHeader_.outdent();
  emulatorHeader_.println("}");
}

void isaCreator::generateModuleRegistrator(isaParser::IsaContext* ctx) {
  emulatorSource_.println(std::format("void {}::{}(moduleContext& ctx) {{",
                                      getEmulatorNamespace(),
                                      getModuleRegistratorName()));
  emulatorSource_.indent();

  emulatorSource_.println("llvm::LLVMContext& llvmCtx = ctx.getContext();");

  for (auto* instruction : ctx->instruction()) {
    auto name = instruction->INSTR_NAME()->getText();

    std::vector<std::string> params;
    for (auto* op : instruction->op()) {
      if (op->IMM() || op->REG()) {
        params.push_back("llvm::Type::getInt64Ty(llvmCtx)");
        continue;
      }

      params.push_back("llvm::Type::getInt1Ty(llvmCtx)");
      params.push_back("llvm::Type::getInt64Ty(llvmCtx)");
    }
    emulatorSource_.println(
        std::format("std::array<llvm::Type*, {}> {}Params = {{{}}};",
                    params.size(), name, joinDelim(params, ", ")));
    emulatorSource_.println(
        std::format("ctx.addExternalFun(llvm::FunctionType::get(llvm::Type::"
                    "getVoidTy(llvmCtx), "
                    "{}Params, false), \"{}\");",
                    name, name));
  }

  emulatorSource_.outdent();
  emulatorSource_.println("}");
}

void isaCreator::exitInstruction(isaParser::InstructionContext* ctx) {
  auto instrName = ctx->INSTR_NAME()->getText();

  appendTokenName(instrName);
  appendParserRule(ctx);
  declareEmulationFunctions(ctx);
  defineEmulationFunction(ctx);

  declareVisitorFunction(instrName);
  defineVisitorFunction(ctx);
}

void isaCreator::generateFunctionCreator(isaParser::IsaContext* ctx) {
  emulatorSource_.println(std::format("void* {}::{}(std::string_view fn) {{",
                                      getEmulatorNamespace(),
                                      getFunctionCreatorName()));
  emulatorSource_.indent();
  emulatorSource_.println("#if defined(__APPLE__) && defined(__MACH__)");
  emulatorSource_.println("// mach o symbols are prefixed with '_'");
  emulatorSource_.println("fn.remove_prefix(1);");
  emulatorSource_.println("#endif");

  for (auto* instruction : ctx->instruction()) {
    emulatorSource_.println(std::format("if (fn == \"{}\") {{",
                                        instruction->INSTR_NAME()->getText()));
    emulatorSource_.indent();

    emulatorSource_.println(std::format(
        "return reinterpret_cast<void*>({});",
        getEmulatorNamespace() + "::" +
            getEmulationFunctionName(instruction->INSTR_NAME()->getText())));

    emulatorSource_.outdent();
    emulatorSource_.println("}");
  }

  emulatorSource_.println("return nullptr;");
  emulatorSource_.outdent();
  emulatorSource_.println("}");
}

void isaCreator::appendTokenName(std::string instruction) {
  lexerGrammar_.println(
      std::format("{}: '{}';", getTokenName(instruction), instruction));
}

void isaCreator::generateLexerPreamble() {
  lexerGrammar_.println(
      std::format("lexer grammar {};", getLexerGrammarName()));
}

void isaCreator::generateVisitorPreamble() {
  auto emInstrCreatorClassName = getEmInstrCreatorClassName();
  auto brInstrCreatorClassName = getBrInstrCreatorClassName();

  visitorHeader_.println("#include <branchInstructionCreator.h>");
  visitorHeader_.println("#include <asmParser.h>");
  visitorHeader_.println("#include \"emulator.h\"");
  visitorHeader_.println();

  visitorHeader_.println(std::format("class {} : public {} {{",
                                     emInstrCreatorClassName,
                                     brInstrCreatorClassName));
  visitorHeader_.indent();

  visitorHeader_.println(std::format("using {}::{};", brInstrCreatorClassName,
                                     brInstrCreatorClassName));
  visitorHeader_.println("public:");

  visitorSource_.println(
      std::format("#include \"{}\"", emInstrCreatorClassName + ".h"));
}

void isaCreator::generateEmulatorPreamble() {
  emulatorHeader_.println("#include <array>");
  emulatorHeader_.println("#include <moduleContext.h>");

  emulatorHeader_.println(
      std::format("namespace {} {{", getEmulatorNamespace()));

  emulatorHeader_.indent();

  emulatorHeader_.println(std::format("extern std::array<{}, {}> {};",
                                      getRegisterType(), getRegisterCount(),
                                      getRegsVarName()));

  emulatorHeader_.println(
      std::format("void* {}(std::string_view fn);", getFunctionCreatorName()));

  emulatorHeader_.println(std::format("void {}(moduleContext& module);",
                                      getModuleRegistratorName()));

  emulatorSource_.println("#include \"emulator.h\"");
}

std::string isaCreator::getModuleRegistratorName() const {
  return "registerFunctions";
}

std::string isaCreator::getFunctionCreatorName() const {
  return "emulatorCaller";
}

std::string isaCreator::getEmulatorNamespace() const {
  return isaName_ + "Emulator";
}

std::string isaCreator::getEmInstrCreatorClassName() const {
  return "emulatingInstructionCreator";
}

std::string isaCreator::getBrInstrCreatorClassName() const {
  return "branchInstructionCreator";
}

std::string isaCreator::getRegisterType() const { return "int64_t"; }
size_t isaCreator::getRegisterCount() const { return 32; }

std::string isaCreator::getAsmParserNamespace() const {
  return isaName_ + "Parser";
}

std::string isaCreator::getInstructionContextType(
    std::string instruction) const {
  instruction.front() = std::toupper(instruction.front());
  return getAsmParserNamespace() + "::" + std::move(instruction) + "Context";
}

void isaCreator::generateParserPreamble() {
  parserGrammar_.println(
      std::format("parser grammar {};", getParserGrammarName()));

  //   parserGrammar_.println(std::format("import {}Lexer;", isaName_));
  parserGrammar_.println("options {");
  parserGrammar_.indent();
  parserGrammar_.println(std::format("tokenVocab = {};", getIsaLexerName()));
  parserGrammar_.outdent();
  parserGrammar_.println("}");

  parserGrammar_.println("regOrImm: REG | MINUS? IMM;");
}

std::string isaCreator::getInstructionsGrammarName() const {
  return isaName_ + "Instruction";
}

std::string isaCreator::getParserGrammarName() const {
  return getInstructionsGrammarName() + "Parser";
}

std::string isaCreator::getIsaLexerName() const { return isaName_ + "Lexer"; }

std::string isaCreator::getLexerGrammarName() const {
  return getInstructionsGrammarName() + "Lexer";
}

void isaCreator::appendParserRule(isaParser::InstructionContext* ctx) {
  auto instrName = ctx->INSTR_NAME()->getText();
  std::vector<std::string> ops;
  for (auto* op : ctx->op()) {
    if (op->IMM()) {
      ops.push_back("IMM");
      continue;
    }

    if (op->REG()) {
      ops.push_back("REG");
      continue;
    }

    ops.push_back("regOrImm");
  }

  parserGrammar_.println(std::format("{}: {} {};", instrName,
                                     getTokenName(instrName),
                                     joinDelim(ops, " COMMA ")));
}

void isaCreator::declareEmulationFunctions(isaParser::InstructionContext* ctx) {
  auto instrName = ctx->INSTR_NAME()->getText();
  auto emulationFunctionName = getEmulationFunctionName(instrName);
  auto convertedFunctionName = getConvertedEmulationFunctionName(instrName);

  auto params = getEmulationFunctionParams(ctx);
  auto convertedParams = getEmulationFunctionConvertedParams(ctx);
  emulatorHeader_.println(
      std::format("void {}({});", emulationFunctionName, params));
  emulatorHeader_.println(
      std::format("void {}({});", convertedFunctionName, convertedParams));
}

std::string isaCreator::getEmulationFunctionName(
    std::string instruction) const {
  return std::string("_") +
         getConvertedEmulationFunctionName(std::move(instruction));
}

std::string isaCreator::convertToOpType(isaParser::OpContext* ctx) const {
  if (ctx->IMM()) {
    return "IMM";
  }

  if (ctx->REG()) {
    return "REG";
  }

  if (ctx->RIM()) {
    return "regOrImm";
  }

  throw std::runtime_error("Expected valid opContext.");
}

std::string isaCreator::getEmulationFunctionParams(
    isaParser::InstructionContext* ctx) const {
  size_t counter = 0;
  std::vector<std::string> res;

  for (auto* op : ctx->op()) {
    if (op->IMM()) {
      res.push_back(std::format("{} op{}", getRegisterType(), counter++));
      continue;
    }

    if (op->REG()) {
      res.push_back(std::format("size_t op{}", counter++));
      continue;
    }

    res.push_back(std::format("bool isImm{}, int64_t op{}", counter, counter));
    ++counter;
  }

  return joinDelim(res, ", ");
}

std::string isaCreator::convertToEmulationFunctionParam(
    isaParser::OpContext* ctx) const {
  if (ctx->IMM()) {
    return "int64_t";
  }

  if (ctx->REG()) {
    return "REG";
  }

  if (ctx->RIM()) {
    return "regOrImm";
  }

  throw std::runtime_error("Expected valid opContext.");
}

std::string isaCreator::getEmulationFunctionConvertedParams(
    isaParser::InstructionContext* ctx) const {
  std::vector<std::string> res;
  auto ops = ctx->op();
  res.reserve(ops.size());

  for (size_t i = 0; i < ops.size(); ++i) {
    res.push_back(std::string(ops[i]->REG() ? "size_t" : "int64_t") + " op" +
                  std::to_string(i));
  }

  return joinDelim(res, ", ");
}

std::string isaCreator::getConvertedEmulationFunctionName(
    std::string instruction) const {
  return std::move(instruction) + '_';
}

void isaCreator::defineEmulationFunction(isaParser::InstructionContext* ctx) {
  auto emulationFName = getEmulationFunctionName(ctx->INSTR_NAME()->getText());
  auto params = getEmulationFunctionParams(ctx);

  emulatorSource_.println(std::format(
      "void {}::{}({}) {{", getEmulatorNamespace(), emulationFName, params));
  emulatorSource_.indent();

  size_t counter = 0;

  std::vector<std::string> args;
  args.reserve(ctx->op().size());

  for (auto* op : ctx->op()) {
    if (op->RIM()) {
      emulatorSource_.println(
          std::format("int64_t val{} = isImm{} ? op{} : {}[op{}];", counter,
                      counter, counter, getRegsVarName(), counter));
      args.push_back(std::string("val") + std::to_string(counter));
      ++counter;
      continue;
    }

    args.push_back(std::string("op") + std::to_string(counter));

    ++counter;
  }

  emulatorSource_.println(std::format(
      "{}({});",
      getConvertedEmulationFunctionName(ctx->INSTR_NAME()->getText()),
      joinDelim(args, ", ")));
  emulatorSource_.outdent();
  emulatorSource_.println("}");
}

std::string isaCreator::getRegsVarName() const { return "REGS"; }
std::string isaCreator::convertInstructionToAntlrRule(std::string instruction) {
  instruction.front() = std::toupper(instruction.front());
  return instruction;
}

void isaCreator::declareVisitorFunction(const std::string& instruction) {
  auto ruleName = convertInstructionToAntlrRule(instruction);

  visitorHeader_.println(std::format("std::any visit{}({}* ctx) override;",
                                     ruleName,
                                     getInstructionContextType(instruction)));
}

void isaCreator::defineVisitorFunction(isaParser::InstructionContext* ctx) {
  auto instrName = ctx->INSTR_NAME()->getText();
  visitorSource_.println(std::format("std::any {}::visit{}({}* ctx) {{",
                                     getEmInstrCreatorClassName(),
                                     convertInstructionToAntlrRule(instrName),
                                     getInstructionContextType(instrName)));

  visitorSource_.indent();
  auto builder = getBuilderGetter();
  auto ops = ctx->op();

  size_t totalReg =
      std::count_if(ops.begin(), ops.end(), [](auto* op) { return op->REG(); });
  size_t totalImm =
      std::count_if(ops.begin(), ops.end(), [](auto* op) { return op->IMM(); });
  size_t totalRim =
      std::count_if(ops.begin(), ops.end(), [](auto* op) { return op->RIM(); });

  size_t regCnt = 0;
  size_t immCnt = 0;
  size_t rimCnt = 0;

  for (auto* op : ctx->op()) {
    size_t cnt = regCnt + immCnt + rimCnt;

    if (op->IMM()) {
      visitorSource_.println(
          std::format("llvm::Value* op{} = "
                      "{}.getInt64(std::stoll(ctx->IMM()[{}]{}getText()));",
                      cnt, builder, immCnt, totalImm > 1 ? "->" : "."));
      ++immCnt;
      continue;
    }

    if (op->REG()) {
      visitorSource_.println(
          std::format("llvm::Value* op{} = "
                      "{}.getInt64(getRegisterIdx(ctx->REG()[{}]{}getText()));",
                      cnt, builder, regCnt, totalReg > 1 ? "->" : "."));
      ++regCnt;
      continue;
    }

    visitorSource_.println(std::format(
        "auto op{}Unpacked = unpackRegOrImm({}ctx->regOrImm()[{}]);", cnt,
        totalRim > 1 ? "" : "&", rimCnt));
    visitorSource_.println(
        std::format("auto op{} = std::pair<llvm::Value*, "
                    "llvm::Value*>(getBoolValueFromOpType(op{}Unpacked.first), "
                    "{}.getInt64(op{}Unpacked.second));",
                    cnt, cnt, builder, cnt));
    ++rimCnt;
  }

  size_t argsSize = regCnt + immCnt + rimCnt * 2;

  std::vector<std::string> args;
  args.reserve(argsSize);

  size_t counter = 0;
  for (auto* op : ctx->op()) {
    if (op->RIM()) {
      args.push_back(std::format("op{}.first", counter));
      args.push_back(std::format("op{}.second", counter));
      ++counter;
      continue;
    }

    args.push_back(std::format("op{}", counter++));
  }

  visitorSource_.println(
      std::format("std::array<llvm::Value*, {}> args = {{{}}};", argsSize,
                  joinDelim(args, ", ")));

  visitorSource_.println(
      std::format("{}.CreateCall(moduleCtx_.getExternalFun(\"{}\"), args);",
                  builder, instrName));
  visitorSource_.println("return defaultResult();");
  visitorSource_.outdent();
  visitorSource_.println("}");
}

std::string isaCreator::getBuilderGetter() const {
  return "moduleCtx_.getBuilder()";
}

void isaCreator::finishParser(isaParser::IsaContext* ctx) {
  std::vector<std::string> instrs;
  instrs.reserve(ctx->instruction().size());
  for (auto* instr : ctx->instruction()) {
    instrs.push_back(instr->INSTR_NAME()->getText());
  }

  parserGrammar_.println(
      std::format("simpleInstruction: {};", joinDelim(instrs, " | ")));
}

std::string isaCreator::getTokenName(const std::string& instruction) const {
  std::string upper;
  upper.reserve(instruction.size());
  std::transform(instruction.begin(), instruction.end(),
                 std::back_insert_iterator<std::string>(upper),
                 [](char c) { return std::toupper(c); });
  return upper;
}