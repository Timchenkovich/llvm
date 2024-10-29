#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <vector>

using namespace llvm;

struct UseTracePass : public PassInfoMixin<UseTracePass> {
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    for (auto &F : M) {
      if (F.isDeclaration()) {
        continue;
      }

      LLVMContext &Ctx = F.getContext();
      IRBuilder<> builder(Ctx);
      Type *retType = Type::getVoidTy(Ctx);

      ArrayRef<Type *> funcPrintUseParamTypes = {
          builder.getInt8Ty()->getPointerTo(), builder.getInt64Ty(),
          builder.getInt8Ty()->getPointerTo(), builder.getInt64Ty()};
      FunctionType *funcPrintUseFuncType =
          FunctionType::get(retType, funcPrintUseParamTypes, false);
      FunctionCallee funcPrintUse =
          M.getOrInsertFunction("printUse", funcPrintUseFuncType);

      for (auto &B : F) {
        for (auto &I : B) {
          if (dyn_cast<PHINode>(&I)) {
            continue;
          }

          auto operands = I.operands();

          std::vector<Value *> op_names;
          std::vector<ConstantInt *> op_addresses;

          builder.SetInsertPoint(&I);

          auto *userName =
              builder.CreateGlobalStringPtr(StringRef(I.getOpcodeName()));
          auto *userAddress =
              ConstantInt::get(builder.getInt64Ty(), (int64_t)(&I));

          int num_instrs = 0;

          for (const auto &op : operands) {
            if (auto instr = dyn_cast<Instruction>(&op);
                instr && !dyn_cast<PHINode>(instr)) {
              op_names.push_back(builder.CreateGlobalStringPtr(
                  StringRef(instr->getOpcodeName())));
              op_addresses.push_back(
                  ConstantInt::get(builder.getInt64Ty(), (int64_t)(instr)));

              num_instrs++;
            }
          }

          for (int32_t i = 0; i < num_instrs; ++i) {
            builder.SetInsertPoint(&I);

            Value *args[] = {userName, userAddress, op_names[i],
                             op_addresses[i]};
            builder.CreateCall(funcPrintUse, args);
          }
        }
      }
    }

    return PreservedAnalyses::none();
  }
};

PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](PassBuilder &PB) {
    PB.registerOptimizerLastEPCallback([&](ModulePassManager &MPM, auto) {
      MPM.addPass(UseTracePass{});
      return true;
    });
  };

  return {LLVM_PLUGIN_API_VERSION, "UseTracer", "0.0.1", callback};
};

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}