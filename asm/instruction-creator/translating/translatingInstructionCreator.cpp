#include "translatingInstructionCreator.h"

#include "llvm/IR/InstrTypes.h"
#include "moduleContext.h"
#include "utils.h"

using namespace llvm;

std::any translatingInstructionCreator::visitAdd(
    asmParser::AddContext* context) {
  createBinaryOperation(
      [this](llvm::Value* left, llvm::Value* right) {
        return moduleCtx_.getBuilder().CreateAdd(left, right);
      },
      context->REG()->getText(), context->regOrImm().front(),
      context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitSub(
    asmParser::SubContext* context) {
  createBinaryOperation(
      [this](llvm::Value* left, llvm::Value* right) {
        return moduleCtx_.getBuilder().CreateAdd(left, right);
      },
      context->REG()->getText(), context->regOrImm().front(),
      context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitShl(
    asmParser::ShlContext* context) {
  size_t resIdx = getRegisterIdx(context->REG().front()->getText());
  size_t srcIdx = getRegisterIdx(context->REG().back()->getText());
  int64_t op = std::stoll(context->IMM()->getText());

  Value* op1 = getRegisterValue(srcIdx);
  Value* op2 = moduleCtx_.getBuilder().getInt64(op);
  moduleCtx_.getBuilder().CreateStore(
      moduleCtx_.getBuilder().CreateShl(op1, op2), getRegisterPointer(resIdx));

  return defaultResult();
}

void translatingInstructionCreator::createBinaryOperation(
    std::function<llvm::Value*(llvm::Value*, llvm::Value*)> opCreator,
    const std::string& resRegStr, asmParser::RegOrImmContext* op2Ctx,
    asmParser::RegOrImmContext* op3Ctx) {
  size_t resIdx = getRegisterIdx(resRegStr);
  auto op2 = unpackRegOrImm(op2Ctx);
  auto op3 = unpackRegOrImm(op3Ctx);

  Value* ep1 = getRegisterPointer(resIdx);
  Value* val2 = getRegOrImmValue(op2.first, op2.second);
  Value* val3 = getRegOrImmValue(op3.first, op3.second);

  Value* res = moduleCtx_.getBuilder().CreateAdd(val2, val3);
  moduleCtx_.getBuilder().CreateStore(res, ep1);
}

void translatingInstructionCreator::createICmp(
    CmpInst::Predicate p, const std::string& resRegStr,
    asmParser::RegOrImmContext* op2Ctx, asmParser::RegOrImmContext* op3Ctx) {
  createBinaryOperation(
      [this, p](Value* left, Value* right) {
        return moduleCtx_.getBuilder().CreateICmp(p, left, right);
      },
      resRegStr, op2Ctx, op3Ctx);
}

std::any translatingInstructionCreator::visitCmpgt(
    asmParser::CmpgtContext* context) {
  createICmp(CmpInst::Predicate::ICMP_SGT, context->REG()->getText(),
             context->regOrImm().front(), context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitCmpgte(
    asmParser::CmpgteContext* context) {
  createICmp(CmpInst::Predicate::ICMP_SGE, context->REG()->getText(),
             context->regOrImm().front(), context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitCmplt(
    asmParser::CmpltContext* context) {
  createICmp(CmpInst::Predicate::ICMP_SLT, context->REG()->getText(),
             context->regOrImm().front(), context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitCmpneq(
    asmParser::CmpneqContext* context) {
  createICmp(CmpInst::Predicate::ICMP_NE, context->REG()->getText(),
             context->regOrImm().front(), context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitOr(asmParser::OrContext* context) {
  createBinaryOperation(
      [this](Value* left, Value* right) {
        return moduleCtx_.getBuilder().CreateOr(left, right);
      },
      context->REG()->getText(), context->regOrImm().front(),
      context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitAnd(
    asmParser::AndContext* context) {
  createBinaryOperation(
      [this](Value* left, Value* right) {
        return moduleCtx_.getBuilder().CreateAnd(left, right);
      },
      context->REG()->getText(), context->regOrImm().front(),
      context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitSlctev(
    asmParser::SlctevContext* context) {
  size_t resIdx = getRegisterIdx(context->REG().front()->getText());
  size_t srcIdx = getRegisterIdx(context->REG().back()->getText());

  auto op2 = unpackRegOrImm(context->regOrImm().front());
  auto op3 = unpackRegOrImm(context->regOrImm().back());

  Value* val2 = getRegOrImmValue(op2.first, op2.second);
  Value* val3 = getRegOrImmValue(op3.first, op3.second);

  Value* srcVal = getRegisterValue(srcIdx);
  Value* rem = moduleCtx_.getBuilder().CreateAnd(
      srcVal, moduleCtx_.getBuilder().getInt64(1));

  moduleCtx_.getBuilder().CreateStore(createSelect(rem, val2, val3),
                                      getRegisterPointer(resIdx));
  return defaultResult();
}

llvm::Value* translatingInstructionCreator::createSelect(llvm::Value* selector,
                                                         llvm::Value* op1,
                                                         llvm::Value* op2) {
  Value* cmp =
      moduleCtx_.getBuilder().CreateICmp(CmpInst::Predicate::ICMP_EQ, selector,
                                         moduleCtx_.getBuilder().getInt64(0));
  return moduleCtx_.getBuilder().CreateSelect(cmp, op1, op2);
}

std::any translatingInstructionCreator::visitMul(
    asmParser::MulContext* context) {
  createBinaryOperation(
      [this](Value* left, Value* right) {
        return moduleCtx_.getBuilder().CreateMul(left, right);
      },
      context->REG()->getText(), context->regOrImm().front(),
      context->regOrImm().back());
  return defaultResult();
}

std::any translatingInstructionCreator::visitMuladd(
    asmParser::MuladdContext* context) {
  size_t resIdx = getRegisterIdx(context->REG().front()->getText());
  size_t op1Idx = getRegisterIdx(context->REG().back()->getText());

  auto op2 = unpackRegOrImm(context->regOrImm().front());
  auto op3 = unpackRegOrImm(context->regOrImm().back());

  Value* val1 = getRegisterValue(op1Idx);
  Value* val2 = getRegOrImmValue(op2.first, op2.second);
  Value* val3 = getRegOrImmValue(op3.first, op3.second);
  Value* mul = moduleCtx_.getBuilder().CreateMul(val1, val2);
  Value* add = moduleCtx_.getBuilder().CreateAdd(mul, val3);
  moduleCtx_.getBuilder().CreateStore(add, getRegisterPointer(op1Idx));

  return defaultResult();
}

std::any translatingInstructionCreator::visitMov(
    asmParser::MovContext* context) {
  size_t regIdx = getRegisterIdx(context->REG()->getText());
  auto op = unpackRegOrImm(context->regOrImm());

  Value* val = getRegOrImmValue(op.first, op.second);
  moduleCtx_.getBuilder().CreateStore(val, getRegisterPointer(regIdx));

  return defaultResult();
}

std::any translatingInstructionCreator::visitSelect(
    asmParser::SelectContext* context) {
  size_t resIdx = getRegisterIdx(context->REG().front()->getText());
  size_t srcIdx = getRegisterIdx(context->REG().back()->getText());

  auto op2 = unpackRegOrImm(context->regOrImm().front());
  auto op3 = unpackRegOrImm(context->regOrImm().back());

  Value* val2 = getRegOrImmValue(op2.first, op2.second);
  Value* val3 = getRegOrImmValue(op3.first, op3.second);

  moduleCtx_.getBuilder().CreateStore(
      createSelect(getRegisterValue(srcIdx), val2, val3),
      getRegisterPointer(resIdx));
  return defaultResult();
}
