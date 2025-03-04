#include "emulatingInstructionCreator.h"

using namespace llvm;

std::any emulatingInstructionCreator::visitAdd(asmParser::AddContext* context) {
  size_t resIdx = getRegisterIdx(context->REG()->getText());
  auto op2 = unpackRegOrImm(context->regOrImm().front());
  auto op3 = unpackRegOrImm(context->regOrImm().back());

  std::array<Value*, 5> args = {
      getRegisterValue(resIdx),
      getBoolValueFromOpType(op2.first),
      moduleCtx_.getBuilder().getInt64(op2.second),
      getBoolValueFromOpType(op3.first),
      moduleCtx_.getBuilder().getInt64(op3.second),
  };

  moduleCtx_.getBuilder().CreateCall(moduleCtx_.getExternalFun("add"), args);
  return defaultResult();
}
