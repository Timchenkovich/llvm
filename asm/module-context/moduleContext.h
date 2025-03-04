#pragma once

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

class moduleContext {
 public:
  moduleContext(const std::string& moduleId, size_t regFileSize);

 public:
  llvm::Module& getModule();
  llvm::LLVMContext& getContext();
  llvm::IRBuilder<>& getBuilder();
  llvm::GlobalVariable& getRegFile();

  void addExternalFun(llvm::FunctionType* type, const std::string& name);
  llvm::FunctionCallee getExternalFun(const std::string& name);

  size_t getRegFileSize() const;

  llvm::FunctionType* getFunType();

 private:
  llvm::Module module_;
  llvm::LLVMContext ctx_;
  llvm::IRBuilder<> builder_;

  std::unordered_map<std::string, llvm::FunctionCallee> nameToCallee_;

  llvm::GlobalVariable regFile_;
  size_t regFileSize_;
};