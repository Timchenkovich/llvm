#pragma once

#include <branchInstructionCreator.h>
#include <utils.h>

#include "asmParser.h"
#include "llvm/IR/InstrTypes.h"

class translatingInstructionCreator final : public branchInstructionCreator {
  using branchInstructionCreator::branchInstructionCreator;

 public:
  std::any visitAdd(asmParser::AddContext* context) override;
  std::any visitSub(asmParser::SubContext* context) override;
  std::any visitShl(asmParser::ShlContext* context) override;
  std::any visitCmpgt(asmParser::CmpgtContext* context) override;
  std::any visitCmpgte(asmParser::CmpgteContext* context) override;
  std::any visitCmplt(asmParser::CmpltContext* context) override;
  std::any visitCmpneq(asmParser::CmpneqContext* context) override;
  std::any visitOr(asmParser::OrContext* context) override;
  std::any visitAnd(asmParser::AndContext* context) override;
  std::any visitSlctev(asmParser::SlctevContext* context) override;
  std::any visitMul(asmParser::MulContext* context) override;
  std::any visitMuladd(asmParser::MuladdContext* context) override;
  std::any visitMov(asmParser::MovContext* context) override;
  std::any visitSelect(asmParser::SelectContext* context) override;

 private:
  void createBinaryOperation(
      std::function<llvm::Value*(llvm::Value*, llvm::Value*)> opCreator,
      const std::string& resRegStr, asmParser::RegOrImmContext* op2Ctx,
      asmParser::RegOrImmContext* op3Ctx);

  void createICmp(llvm::CmpInst::Predicate p, const std::string& resRegStr,
                  asmParser::RegOrImmContext* op2Ctx,
                  asmParser::RegOrImmContext* op3Ctx);

  llvm::Value* createSelect(llvm::Value* selector, llvm::Value* op1,
                            llvm::Value* op2);
};