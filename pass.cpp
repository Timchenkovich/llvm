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
    for (auto &F : M) {
      if (F.isDeclaration()) {
        continue;
      }

      outs() << "[Function]: " << F.getName() << '\n';

      LLVMContext &Ctx = F.getContext();
      IRBuilder<> builder(Ctx);
      Type *retType = Type::getVoidTy(Ctx);

      // ArrayRef<Type *> funcStartParamTypes = {
      //     builder.getInt8Ty()->getPointerTo(), // instr name
      //     builder.getInt64Ty(),                // instr address
      //     builder.getInt8Ty()
      //         ->getPointerTo()
      //         ->getPointerTo(),                 // used instr names
      //     builder.getInt64Ty()->getPointerTo(), // addresses
      //     builder.getInt32Ty(),                 // count
      // }; // used instr names}
      ArrayRef<Type *> funcPrintUseParamTypes = {
          builder.getInt8Ty()->getPointerTo(), builder.getInt64Ty(),
          builder.getInt8Ty()->getPointerTo(), builder.getInt64Ty()};
      FunctionType *funcPrintUseFuncType =
          FunctionType::get(retType, funcPrintUseParamTypes, false);
      FunctionCallee funcPrintUse =
          M.getOrInsertFunction("printUse", funcPrintUseFuncType);

      for (auto &B : F) {
        for (auto &I : B) {
          auto operands = I.operands();
          outs() << "[Instruction]: ";
          I.print(outs());
          outs() << '\n';
          outs() << "OpCode name: " << I.getOpcodeName() << '\n';
          outs() << "[OPERANDS]:\n";
          for (auto &op : operands) {
            op->print(outs());
            outs() << '\n';
            op->printAsOperand(outs());
            outs() << '\n';
          }

          outs() << "getName: " << I.getName() << '\n';

          std::vector<Value *> op_names;
          std::vector<ConstantInt *> op_addresses;
          // auto *userName =

          builder.SetInsertPoint(&I);

          auto *userName =
              builder.CreateGlobalStringPtr(StringRef(I.getOpcodeName()));
          auto *userAddress =
              ConstantInt::get(builder.getInt64Ty(), (int64_t)(&I));

          int num_instrs = 0;
          outs() << "Starting to iterate operands" << '\n';

          for (const auto &op : operands) {
            if (auto instr = dyn_cast<Instruction>(&op)) {
              op_names.push_back(builder.CreateGlobalStringPtr(
                  StringRef(instr->getOpcodeName())));
              op_addresses.push_back(
                  ConstantInt::get(builder.getInt64Ty(), (int64_t)(instr)));

              num_instrs++;
            }
          }

          outs() << "Finished iterating operands\n";
          outs() << "Starting to create calls\n";

          for (int32_t i = 0; i < num_instrs; ++i) {
            outs() << "Creating " << op_names[i] << " Call\n";
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