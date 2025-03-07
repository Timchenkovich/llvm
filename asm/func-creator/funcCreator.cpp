
#include "funcCreator.h"

#include <cassert>
#include <string>

funcCreator::funcCreator(sourceDescriptor& srcDesc) : desc_(srcDesc) {}

std::any funcCreator::visitFunction(asmParser::FunctionContext* ctx) {
  metBranch_ = false;

  std::string name = ctx->ID()->getText();

  lastFun_ = name;

  desc_.addFun(name);
  lastBB_ = desc_.getDescriptor(lastFun_).getEntryLabel();

  return defaultResult();
}

std::any funcCreator::visitLabel(asmParser::LabelContext* ctx) {
  assert(!lastFun_.empty());

  metBranch_ = false;
  auto label = ctx->ID()->getText();

  funDescriptor& des = desc_.getDescriptor(lastFun_);

  des.addBB(label);
  lastBB_ = std::move(label);

  return defaultResult();
};

std::any funcCreator::visitBranch(asmParser::BranchContext* ctx) {
  assert(!lastFun_.empty());

  metBranch_ = true;
  return defaultResult();
}

std::any funcCreator::visitInstruction(asmParser::InstructionContext* ctx) {
  assert(!lastFun_.empty());

  if (metBranch_) {
    metBranch_ = false;
    desc_.getDescriptor(lastFun_).addBB(generateCondBrNextLabel());
  }

  return visitChildren(ctx);
}

std::string funcCreator::generateCondBrNextLabel() {
  return std::to_string(++condBranchIdx_);
}