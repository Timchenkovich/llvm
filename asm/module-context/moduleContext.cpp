#include "moduleContext.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Module.h"

using namespace llvm;

moduleContext::moduleContext(const std::string& moduleId, size_t regFileSize)
    : module_(moduleId, ctx_),
      builder_(ctx_),
      regFile_(module_, ArrayType::get(Type::getInt64Ty(ctx_), regFileSize),
               false, llvm::GlobalValue::InternalLinkage,
               ConstantAggregateZero::get(Type::getInt64Ty(ctx_))),
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