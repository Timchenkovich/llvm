#include "sourceCreator.h"

#include <ios>

sourceCreator::sourceCreator(const std::string& fileName)
    : out_(fileName, std::ios_base::out) {}

void sourceCreator::indent() { numWs_ += 2; }

void sourceCreator::outdent() { numWs_ -= 2; }

void sourceCreator::println(const std::string& str) {
  std::string ws(numWs_, ' ');
  out_ << ws << str << std::endl;
}