#include "llvm-c/Target.h"
extern "C" {
#include <sim.h>
};

#include <antlr4-runtime.h>
#include <asmLexer.h>
#include <asmParser.h>
#include <emulatingInstructionCreator.h>
#include <funcCreator.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <moduleContext.h>
#include <translatingInstructionCreator.h>

#include <cxxopts.hpp>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <string_view>
#include <type_traits>
#include <variant>

#include "source-descriptor/sourceDescriptor.h"

namespace {
constexpr size_t REG_FILE_SIZE = 32;
std::array<int64_t, REG_FILE_SIZE> regs = {0};

constexpr std::string_view ASM_PATH = "asm-path";
constexpr std::string_view MODE = "mode";
constexpr std::string_view DUMP_SHORT = "d";
constexpr std::string_view DUMP = "dump";
constexpr std::string_view MODULE_NAME = "lifted";
constexpr std::string_view REG_FILE_NAME = "regFile";

void printHelp(std::ostream& out) {
  out << "Usage: lift <path-to-asm> <mode>." << std::endl;
  out << "Where mode is either \"emulation\" or \"translation\"." << std::endl;
}

cxxopts::Options createOptions() {
  cxxopts::Options opts("lift");
  opts.add_options()(std::string(ASM_PATH), "Path to asm file",
                     cxxopts::value<std::string>())(
      std::string(MODE), "Mode selection for emulation or translation.",
      cxxopts::value<std::string>())(
      std::string(DUMP_SHORT) + "," + DUMP.data(), "Dump only.");
  return opts;
}

}  // namespace

int main(int argc, char** argv) {
  auto opts = createOptions();
  opts.parse_positional({std::string(ASM_PATH), std::string(MODE)});
  auto parsedOpts = opts.parse(argc, argv);

  std::string mode = parsedOpts[std::string(MODE)].as<std::string>();
  std::string filePath = parsedOpts[std::string(ASM_PATH)].as<std::string>();
  bool dump = static_cast<bool>(parsedOpts.count(std::string(DUMP)));

  if (mode != "emulation" && mode != "translation") {
    std::cerr << "Invalid mode." << std::endl;
    printHelp(std::cerr);
    return 1;
  }

  bool emulating = mode == "emulation";

  std::ifstream in(filePath, std::ios_base::in);
  if (!in) {
    std::cerr << "Failed opening input file." << std::endl;
    return 1;
  }

  llvm::LLVMContext moduleCtx;
  llvm::IRBuilder<> builder(moduleCtx);
  std::unique_ptr<llvm::Module> llvmModule =
      std::make_unique<llvm::Module>(MODULE_NAME, moduleCtx);

  std::variant<std::unique_ptr<llvm::GlobalVariable>, llvm::GlobalVariable*>
      regFileHolder;

  auto regFileGetter = [](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, llvm::GlobalVariable*>) {
      return arg;
    } else {
      return arg.get();
    }
  };

  llvm::ArrayType* regFileType =
      llvm::ArrayType::get(builder.getInt64Ty(), REG_FILE_SIZE);
  if (emulating) {
    llvmModule->getOrInsertGlobal(REG_FILE_NAME, regFileType);
    regFileHolder = llvmModule->getNamedGlobal(REG_FILE_NAME);
  } else {
    regFileHolder = std::make_unique<llvm::GlobalVariable>(
        *llvmModule, regFileType, false, llvm::GlobalValue::InternalLinkage,
        llvm::ConstantAggregateZero::get(llvm::Type::getInt64Ty(moduleCtx)));
  };

  moduleContext ctx("asmGenerated", *std::visit(regFileGetter, regFileHolder),
                    moduleCtx, builder, *llvmModule, REG_FILE_SIZE);

  asmEmulator::registerFunctions(ctx);

  antlr4::ANTLRInputStream antlr_in(in);
  asmLexer lexer(&antlr_in);
  antlr4::CommonTokenStream ts(&lexer);

  asmParser parser(&ts);
  auto* tree = parser.asm_();

  sourceDescriptor sourceDesc(ctx);
  funcCreator fCreator(sourceDesc);

  fCreator.visit(tree);

  if (emulating) {
    emulatingInstructionCreator instrCreator(ctx, sourceDesc);
    instrCreator.visit(tree);
  } else {
    translatingInstructionCreator instrCreator(ctx, sourceDesc);
    instrCreator.visit(tree);
  }

  if (dump) {
    std::error_code ec;
    llvm::raw_fd_stream debugOut("generated.ll", ec);
    ctx.getModule().print(debugOut, nullptr);
    debugOut.close();
    return 0;
  }

  if (llvm::verifyModule(*llvmModule, &llvm::errs())) {
    ctx.getModule().print(llvm::errs(), nullptr);
    return 1;
  }

  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();

  llvm::ExecutionEngine* ee =
      llvm::EngineBuilder(std::move(llvmModule)).create();
  if (!ee) {
    std::cerr << "Failed creating execution engine." << std::endl;
    return 1;
  }

  ee->InstallLazyFunctionCreator(asmEmulator::emulatorCaller);
  ee->finalizeObject();

  ee->addGlobalMapping(&ctx.getRegFile(), reinterpret_cast<void*>(regs.data()));

  simInit();
  llvm::GenericValue res =
      ee->runFunction(sourceDesc.getDescriptor("main").getFunction(), {});
  simExit();

  return 0;
}