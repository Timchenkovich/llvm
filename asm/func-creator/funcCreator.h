#pragma once

#include <sourceDescriptor.h>

#include <string>

#include "asmParserBaseVisitor.h"
#include "asmParserVisitor.h"
#include "llvm/IR/LLVMContext.h"

class funcCreator : public asmParserBaseVisitor {
 public:
  funcCreator(sourceDescriptor& srcDesc);

 public:
  std::any visitFunction(asmParser::FunctionContext* ctx) override;
  std::any visitLabel(asmParser::LabelContext* ctx) override;
  std::any visitBranch(asmParser::BranchContext* ctx) override;
  std::any visitInstruction(asmParser::InstructionContext* ctx) override;

 private:
  std::string generateCondBrNextLabel();

 private:
  std::string lastFun_;
  std::string lastBB_;

  sourceDescriptor& desc_;

  bool metBranch_ = false;
  size_t condBranchIdx_ = 0;
};
