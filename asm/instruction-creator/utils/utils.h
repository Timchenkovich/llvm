#pragma once
#include <asmParser.h>

enum class OpType : bool { REG, IMM };

std::pair<OpType, int64_t> unpackRegOrImm(asmParser::RegOrImmContext* ctx);
size_t getRegisterIdx(const std::string& regStr);
