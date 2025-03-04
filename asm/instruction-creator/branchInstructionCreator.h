#pragma once

#include <moduleContext.h>
#include <sourceDescriptor.h>

#include <unordered_map>

#include "asmParser.h"
#include "asmParserBaseVisitor.h"
#include "asmParserVisitor.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
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

  std::any visitFunction(asmParser::FunctionContext* ctx) override;
  std::any visitLabel(asmParser::LabelContext* ctx) override;
  std::any visitBranch(asmParser::BranchContext* ctx) override;

  std::any visitCall(asmParser::CallContext* context) override;

 protected:
  llvm::Value* getRegisterPointer(size_t idx);
  llvm::Value* getRegisterValue(size_t idx);
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