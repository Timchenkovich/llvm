#pragma once

#include <branchInstructionCreator.h>
#include <utils.h>

#include "asmParser.h"

class emulatingInstructionCreator final : public branchInstructionCreator {
 public:
  std::any visitAdd(asmParser::AddContext* context) override;
};