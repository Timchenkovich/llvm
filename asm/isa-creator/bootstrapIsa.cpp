#include <filesystem>

#include "ANTLRInputStream.h"
#include "CommonTokenStream.h"
#include "isaCreator.h"
#include "isaLexer.h"

namespace {
constexpr std::string_view EMULATOR_FNAME = "emulator";
constexpr std::string_view INSTRUCTION_CREATOR_FNAME =
    "emulatingInstructionCreator";
}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Expected .isa file path argument" << std::endl;
    return 1;
  }

  std::string path = argv[1];

  if (!path.ends_with(".isa")) {
    std::cerr << "Expected .isa file" << std::endl;
    return 1;
  }

  std::string isaName =
      std::filesystem::path(path).filename().replace_extension("").string();

  isaCreator creator(isaName, std::string(EMULATOR_FNAME),
                     std::string(INSTRUCTION_CREATOR_FNAME));

  std::ifstream in(path);
  if (!in) {
    std::cerr << "Unable to open isa file" << std::endl;
    return 1;
  }

  antlr4::ANTLRInputStream antlr_in(in);
  isaLexer lexer(&antlr_in);
  antlr4::CommonTokenStream ts(&lexer);
  isaParser parser(&ts);

  parser.addParseListener(&creator);
  auto* parsed = parser.isa();
  return 0;
}