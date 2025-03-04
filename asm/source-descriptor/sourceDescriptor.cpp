#include "sourceDescriptor.h"

#include <cassert>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "moduleContext.h"

using namespace llvm;

sourceDescriptor::sourceDescriptor(moduleContext& moduleCtx)
    : moduleCtx_(moduleCtx) {}

funDescriptor::funDescriptor(Function* f, moduleContext& moduleCtx)
    : f_(f), moduleCtx_(moduleCtx) {
  addBB(std::string(ENTRY));
}

void funDescriptor::addBB(std::string label) {
  assert(labelToBB_.find(label) == labelToBB_.end());

  size_t idx = counter_;

  auto* bb = createBB();

  labelToBB_.insert({label, bb});
  labelToIdx_.insert({label, idx});
  idxToLabel_.push_back(std::move(label));

  ++counter_;
}

std::string funDescriptor::getEntryLabel() const { return std::string(ENTRY); }

// void funDescriptor::addBranchNextLabel(std::string label) {
//   addBB(label);

//   branchIdxToLabel_.push_back(std::move(label));
// }

void sourceDescriptor::addFun(std::string funName) {
  assert(funToDescriptor_.find(funName) == funToDescriptor_.end());

  std::string varName = funName;
  funDescriptor desc{createFun(funName), moduleCtx_};

  funToDescriptor_.insert({std::move(funName), std::move(desc)});
}

BasicBlock* funDescriptor::getBB(const std::string& label) const {
  return labelToBB_.at(label);
}

funDescriptor& sourceDescriptor::getDescriptor(const std::string& funName) {
  return funToDescriptor_.at(funName);
}

const funDescriptor& sourceDescriptor::getDescriptor(
    const std::string& funName) const {
  return funToDescriptor_.at(funName);
}

std::optional<std::string> funDescriptor::getNextLabel(
    const std::string& label) const {
  size_t cur = labelToIdx_.at(label);
  if (cur == idxToLabel_.size() - 1) {
    return std::nullopt;
  }

  return idxToLabel_[cur + 1];
}

// const std::string& funDescriptor::getBranchNextLabel(size_t idx) const {
//   return branchIdxToLabel_[idx];
// }

Function* sourceDescriptor::createFun(const std::string& funName) {
  Function::LinkageTypes linkage = Function::ExternalLinkage;
  return Function::Create(moduleCtx_.getFunType(), linkage, funName,
                          moduleCtx_.getModule());
}

llvm::BasicBlock* funDescriptor::createBB() {
  return BasicBlock::Create(moduleCtx_.getContext(), "", f_);
}

llvm::Function* funDescriptor::getFunction() const { return f_; }