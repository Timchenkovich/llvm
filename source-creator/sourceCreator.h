#pragma once

#include <format>
#include <fstream>
#include <string>

class sourceCreator {
 public:
  sourceCreator(const std::string& fileName);

 public:
  void indent();
  void outdent();
  void println(const std::string& str = "");

 private:
  size_t numWs_ = 0;
  std::ofstream out_;
};