#include "branchInstructionCreator.h"

#include <utils.h>

#include <cassert>
#include <format>
#include <stdexcept>
#include <string>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CmpPredicate.h"
#include "llvm/IR/DerivedTypes.h"
#include "moduleContext.h"
#include "sourceDescriptor.h"

using namespace llvm;

branchInstructionCreator::branchInstructionCreator(moduleContext& moduleCtx,
                                                   const sourceDescriptor& desc)
    : moduleCtx_(moduleCtx), desc_(desc) {}

std::any branchInstructionCreator::visitFunction(
    asmParser::FunctionContext* ctx) {
  if (!curLabel_.empty() && !bbTerminated_) {
    throw std::runtime_error("Unterminate basic block from previous function.");
  }

  curFun_ = ctx->FUN()->getText();
  changeBB(desc_.getDescriptor(curFun_).getEntryLabel());
  return defaultResult();
}

std::any branchInstructionCreator::visitLabel(asmParser::LabelContext* ctx) {
  changeBB(ctx->ID()->getText());
  return defaultResult();
}

std::any branchInstructionCreator::visitBranch(asmParser::BranchContext* ctx) {
  bbTerminated_ = true;

  BasicBlock* destBB = desc_.getDescriptor(curFun_).getBB(ctx->ID()->getText());
  std::optional<std::string> nextLabel =
      desc_.getDescriptor(curFun_).getNextLabel(curLabel_);

  if (ctx->JMP()) {
    moduleCtx_.getBuilder().CreateBr(destBB);

    if (nextLabel) {
      changeBB(nextLabel.value());
    }

    return defaultResult();
  }

  if (ctx->RET()) {
    moduleCtx_.getBuilder().CreateRetVoid();

    if (nextLabel) {
      changeBB(nextLabel.value());
    }

    return defaultResult();
  }

  if (ctx->CJ()) {
    assert(nextLabel);

    auto* nextBB = desc_.getDescriptor(curFun_).getBB(nextLabel.value());
    size_t regIdx = getRegisterIdx(ctx->REG()->getText());

    moduleCtx_.getBuilder().CreateCondBr(getRegisterValue(regIdx), destBB,
                                         nextBB);

    changeBB(nextLabel.value());
    return defaultResult();
  }

  if (ctx->JNPOS()) {
    assert(nextLabel);

    size_t regIdx = getRegisterIdx(ctx->REG()->getText());
    Value* cmp = moduleCtx_.getBuilder().CreateICmp(
        CmpInst::Predicate::ICMP_SLE, getRegisterValue(regIdx),
        ConstantInt::get(Type::getInt64Ty(moduleCtx_.getContext()), 0));

    BasicBlock* nextBB = desc_.getDescriptor(curFun_).getBB(nextLabel.value());

    moduleCtx_.getBuilder().CreateCondBr(cmp, destBB, nextBB);

    changeBB(nextLabel.value());
    return defaultResult();
  }

  if (ctx->INCJNEQ()) {
    assert(nextLabel);

    size_t regIdx = getRegisterIdx(ctx->REG()->getText());
    Value* inc = moduleCtx_.getBuilder().CreateAdd(
        getRegisterValue(regIdx),
        ConstantInt::get(Type::getInt64Ty(moduleCtx_.getContext()), 1));

    int64_t imm = std::stoll(ctx->IMM()->getText());
    Value* cmp = moduleCtx_.getBuilder().CreateICmp(
        CmpInst::Predicate::ICMP_SLE, getRegisterValue(regIdx),
        ConstantInt::get(Type::getInt64Ty(moduleCtx_.getContext()), imm));

    BasicBlock* nextBB = desc_.getDescriptor(curFun_).getBB(nextLabel.value());

    moduleCtx_.getBuilder().CreateCondBr(cmp, destBB, nextBB);

    changeBB(nextLabel.value());
    return defaultResult();
  }

  throw std::runtime_error("Unhandled branch.");
}

llvm::Value* branchInstructionCreator::getRegisterPointer(size_t idx) {
  return moduleCtx_.getBuilder().CreateConstGEP2_64(
      moduleCtx_.getRegFile().getValueType(), &moduleCtx_.getRegFile(), 0, idx);
}

Value* branchInstructionCreator::getRegisterValue(size_t idx) {
  Value* gep = getRegisterPointer(idx);
  return moduleCtx_.getBuilder().CreateLoad(
      Type::getInt64Ty(moduleCtx_.getContext()), gep);
}

void branchInstructionCreator::changeBB(const std::string& label) {
  const funDescriptor& desc = desc_.getDescriptor(curFun_);
  auto* bb = desc.getBB(label);

  curLabel_ = label;
  moduleCtx_.getBuilder().SetInsertPoint(bb);

  if (!bbTerminated_) {
    moduleCtx_.getBuilder().CreateBr(bb);
  }
  bbTerminated_ = false;
}

std::any branchInstructionCreator::visitAsm(asmParser::AsmContext* ctx) {
  return visitChildren(ctx);
}

std::any branchInstructionCreator::visitLine(asmParser::LineContext* ctx) {
  return visitChildren(ctx);
}
std::any branchInstructionCreator::visitInstruction(
    asmParser::InstructionContext* ctx) {
  return visitChildren(ctx);
}

std::any branchInstructionCreator::visitRegOrImm(
    asmParser::RegOrImmContext* ctx) {
  return visitChildren(ctx);
}

std::any branchInstructionCreator::visitCall(asmParser::CallContext* context) {
  Function* f = desc_.getDescriptor(context->ID()->getText()).getFunction();
  moduleCtx_.getBuilder().CreateCall(f);

  return defaultResult();
}

llvm::Value* branchInstructionCreator::getRegOrImmValue(OpType opType,
                                                        int64_t val) {
  return static_cast<bool>(opType) ? getRegisterValue(val)
                                   : moduleCtx_.getBuilder().getInt64(val);
}

llvm::Value* branchInstructionCreator::getBoolValueFromOpType(OpType opType) {
  return ConstantInt::getBool(moduleCtx_.getContext(),
                              static_cast<bool>(opType));
}