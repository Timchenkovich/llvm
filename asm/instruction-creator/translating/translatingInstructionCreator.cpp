#include "translatingInstructionCreator.h"

#include "utils.h"

using namespace llvm;

std::any translatingInstructionCreator::visitAdd(
    asmParser::AddContext* context) {
  size_t resIdx = getRegisterIdx(context->REG()->getText());
  auto op2 = unpackRegOrImm(context->regOrImm().front());
  auto op3 = unpackRegOrImm(context->regOrImm().back());

  Value* ep1 = getRegisterPointer(resIdx);
  Value* val2 = getRegOrImmValue(op2.first, op2.second);
  Value* val3 = getRegOrImmValue(op3.first, op3.second);

  Value* res = moduleCtx_.getBuilder().CreateAdd(val2, val3);
  moduleCtx_.getBuilder().CreateStore(res, ep1);

  return defaultResult();
}