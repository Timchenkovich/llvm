#include "utils.h"

std::pair<OpType, int64_t> unpackRegOrImm(asmParser::RegOrImmContext* ctx) {
  if (ctx->IMM()) {
    return {OpType::IMM, std::stoll(ctx->IMM()->getText())};
  }

  return {OpType::REG, getRegisterIdx(ctx->REG()->getText())};
}

size_t getRegisterIdx(const std::string& regStr) {
  return static_cast<size_t>(std::stoull(regStr.substr(1)));
}
