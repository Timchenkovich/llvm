#include "moduleContext.h"

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Module.h"

using namespace llvm;

moduleContext::moduleContext(const std::string& moduleId,
                             llvm::GlobalVariable& regFile,
                             llvm::LLVMContext& ctx, llvm::IRBuilder<>& builder,
                             llvm::Module& module, size_t regFileSize)
    : ctx_(ctx),
      builder_(builder),
      module_(module),
      regFile_(regFile),
      regFileSize_(regFileSize) {}

void moduleContext::addExternalFun(FunctionType* type,
                                   const std::string& name) {
  nameToCallee_.insert({name, module_.getOrInsertFunction(name, type)});
}

llvm::FunctionCallee moduleContext::getExternalFun(const std::string& name) {
  return nameToCallee_.at(name);
}

Module& moduleContext::getModule() { return module_; }
LLVMContext& moduleContext::getContext() { return ctx_; }
IRBuilder<>& moduleContext::getBuilder() { return builder_; }
GlobalVariable& moduleContext::getRegFile() { return regFile_; }

size_t moduleContext::getRegFileSize() const { return regFileSize_; }

FunctionType* moduleContext::getFunType() {
  Type* voidType = Type::getVoidTy(ctx_);
  ArrayRef<Type*> args;
  return FunctionType::get(voidType, args, false);
}