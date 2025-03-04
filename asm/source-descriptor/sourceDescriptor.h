#pragma once

#include <moduleContext.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

class funDescriptor {
 private:
  static constexpr std::string_view ENTRY = "";

 public:
  funDescriptor(llvm::Function* f, moduleContext& moduleCtx);

 public:
  void addBB(std::string label);

  std::string getEntryLabel() const;

  llvm::BasicBlock* getBB(const std::string& label) const;
  llvm::Function* getFunction() const;
  std::optional<std::string> getNextLabel(const std::string& label) const;
  // const std::string& getBranchNextLabel(size_t idx) const;

 private:
  llvm::BasicBlock* createBB();

 private:
  llvm::Function* f_;
  moduleContext& moduleCtx_;

  std::unordered_map<std::string, llvm::BasicBlock*> labelToBB_;
  std::unordered_map<std::string, size_t> labelToIdx_;
  std::vector<std::string> idxToLabel_;

  size_t counter_;
};

class sourceDescriptor {
 public:
  sourceDescriptor(moduleContext& moduleCtx);

 public:
  void addFun(std::string funName);

  funDescriptor& getDescriptor(const std::string& funName);
  const funDescriptor& getDescriptor(const std::string& funName) const;

 private:
  llvm::Function* createFun(const std::string& funName);

 private:
  moduleContext& moduleCtx_;
  std::unordered_map<std::string, funDescriptor> funToDescriptor_;
};