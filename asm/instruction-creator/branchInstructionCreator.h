#pragma once

#include <moduleContext.h>
#include <sourceDescriptor.h>

#include "asmParser.h"
#include "asmParserVisitor.h"
#include "utils.h"

class branchInstructionCreator : public asmParserVisitor {
 public:
  branchInstructionCreator(moduleContext& moduleCtx,
                           const sourceDescriptor& desc);

 public:
  std::any visitAsm(asmParser::AsmContext* ctx) override;
  std::any visitLine(asmParser::LineContext* ctx) override;
  std::any visitInstruction(asmParser::InstructionContext* ctx) override;
  std::any visitRegOrImm(asmParser::RegOrImmContext* ctx) override;

  std::any visitFlush(asmParser::FlushContext* context) override;
  std::any visitRnd(asmParser::RndContext* context) override;
  std::any visitPutpxl(asmParser::PutpxlContext* context) override;
  std::any visitRndrng(asmParser::RndrngContext* context) override;

  std::any visitFunction(asmParser::FunctionContext* ctx) override;
  std::any visitLabel(asmParser::LabelContext* ctx) override;
  std::any visitBranch(asmParser::BranchContext* ctx) override;

  std::any visitSimpleInstruction(
      asmParser::SimpleInstructionContext* ctx) override;
  std::any visitCall(asmParser::CallContext* context) override;

 protected:
  llvm::Value* getRegisterPointer(size_t idx);
  llvm::Value* getRegisterValue(size_t idx);
  llvm::Value* getRegisterBoolValue(size_t idx);
  llvm::Value* getRegOrImmValue(OpType opType, int64_t val);
  llvm::Value* getBoolValueFromOpType(OpType);

 private:
  void changeBB(const std::string& label);

 protected:
  moduleContext& moduleCtx_;
  const sourceDescriptor& desc_;

 private:
  std::string curFun_;
  std::string curLabel_;

  bool bbTerminated_ = false;
};