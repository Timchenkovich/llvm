#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <vector>

using namespace llvm;

struct MyModPass : public PassInfoMixin<MyModPass> {
  // PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
  //   outs() << "[Module] " << M.getName() << "\n";
  //   bool changed = false;
  //   for (auto &F : M) {
  //     outs() << "[Function] " << F.getName() << " (arg_size: " <<
  //     F.arg_size()
  //            << ")\n";
  //     if (F.isDeclaration()) {
  //       continue;
  //     }
  //     F.print(outs());

  //     for (auto &B : F) {
  //       for (auto &I : B) {
  //         if (auto *op = dyn_cast<BinaryOperator>(&I)) {
  //           outs() << "Modified instruction:\n";
  //           I.print(outs(), true);
  //           outs() << "\n";
  //           // Insert at the point where the instruction `op` appears.
  //           IRBuilder<> builder(op);

  //           // Make a sub with the same operands as `op`.
  //           Value *lhs = op->getOperand(0);
  //           Value *rhs = op->getOperand(1);
  //           Value *sub = builder.CreateSub(lhs, rhs);

  //           // Everywhere the old instruction was used as an operand, use our
  //           // new sub instruction instead.
  //           for (auto &U : op->uses()) {
  //             User *user = U.getUser(); // A User is anything with operands.
  //             user->setOperand(U.getOperandNo(), sub);
  //           }
  //           changed = true;
  //         }
  //       }
  //     }
  //   }
  //   return changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
  // };

  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    bool changed = false;
    for (auto &F : M) {
      if (F.isDeclaration()) {
        continue;
      }

      LLVMContext &Ctx = F.getContext();
      IRBuilder<> builder(Ctx);
      Type *retType = Type::getVoidTy(Ctx);

      ArrayRef<Type *> funcStartParamTypes = {
          builder.getInt8Ty()->getPointerTo(), // instr name
          builder.getInt64Ty(),                // instr address
          builder.getInt8Ty()
              ->getPointerTo()
              ->getPointerTo(),                 // used instr names
          builder.getInt64Ty()->getPointerTo(), // addresses
          builder.getInt32Ty(),                 // count
      }; // used instr names}
      FunctionType *funcStartLogFuncType =
          FunctionType::get(retType, funcStartParamTypes, false);
      FunctionCallee funcStartLogFunc =
          M.getOrInsertFunction("printUses", funcStartLogFuncType);

      for (auto &B : F) {
        for (auto &I : B) {
          auto operands = I.operands();
          std::vector<Value *> op_names;
          int32_t num_ops = 0;
          for (const auto &op : operands) {
            if (auto instr = dyn_cast<Instruction>(op)) {
              op_names.push_back(builder.CreateGlobalString(instr->getName()));
              ++num_ops;
            }
          }

          if (num_ops > 0) {
            // Insert before ret
            builder.SetInsertPoint(&I);

            Value *args[] = {
                builder.CreateGlobalString(I.getName()),
                ConstantInt::get(builder.getInt64Ty(), (int64_t)(&I)),
                builder.CreatePo};
          }
        }
      }
    }

    return changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
  }
};

PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](PassBuilder &PB) {
    PB.registerOptimizerLastEPCallback([&](ModulePassManager &MPM, auto) {
      MPM.addPass(MyModPass{});
      return true;
    });
  };

  return {LLVM_PLUGIN_API_VERSION, "MyPlugin", "0.0.1", callback};
};

/* When a plugin is loaded by the driver, it will call this entry point to
obtain information about this plugin and about how to register its passes.
*/
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}