#pragma once

#include <isaParserBaseListener.h>

#include "isaParser.h"
#include "sourceCreator.h"

class isaCreator : public isaParserBaseListener {
 public:
  isaCreator(std::string isaName, const std::string& emulatorFName,
             const std::string& emulatingInstrCreatorFName);

 public:
  void enterIsa(isaParser::IsaContext* ctx) override;
  void exitIsa(isaParser::IsaContext* ctx) override;

  void exitInstruction(isaParser::InstructionContext* ctx) override;

 private:
  void generateVisitorPreamble();
  void finishVisitor();

  void generateEmulatorPreamble();
  void finishEmulator(isaParser::IsaContext* ctx);
  void generateFunctionCreator(isaParser::IsaContext* ctx);
  void generateModuleRegistrator(isaParser::IsaContext* ctx);
  void finishParser(isaParser::IsaContext* ctx);

  void generateLexerPreamble();
  void generateParserPreamble();

  void appendParserRule(isaParser::InstructionContext* ctx);
  void appendTokenName(std::string instruction);
  void declareEmulationFunctions(isaParser::InstructionContext* ctx);
  void defineEmulationFunction(isaParser::InstructionContext* ctx);
  void declareVisitorFunction(const std::string& instruction);
  void defineVisitorFunction(isaParser::InstructionContext* ctx);

  std::string getEmInstrCreatorClassName() const;
  std::string getBrInstrCreatorClassName() const;
  std::string getEmulatorNamespace() const;
  std::string getRegisterType() const;
  size_t getRegisterCount() const;
  std::string getAsmParserNamespace() const;
  std::string getInstructionContextType(std::string instruction) const;
  std::string getParserGrammarName() const;
  std::string getInstructionsGrammarName() const;
  std::string getLexerGrammarName() const;
  std::string getIsaLexerName() const;
  std::string getEmulationFunctionName(std::string instruction) const;
  std::string getConvertedEmulationFunctionName(std::string instruction) const;
  std::string convertToOpType(isaParser::OpContext* ctx) const;
  std::string getEmulationFunctionParams(
      isaParser::InstructionContext* ctx) const;
  std::string getEmulationFunctionConvertedParams(
      isaParser::InstructionContext* ctx) const;
  std::string convertToEmulationFunctionParam(isaParser::OpContext* ctx) const;
  std::string getRegsVarName() const;
  std::string convertInstructionToAntlrRule(std::string instruction);
  std::string getBuilderGetter() const;
  std::string getFunctionCreatorName() const;
  std::string getModuleRegistratorName() const;
  std::string getTokenName(const std::string& instruction) const;

 private:
  std::string isaName_;

  sourceCreator emulatorHeader_;
  sourceCreator emulatorSource_;
  sourceCreator visitorHeader_;
  sourceCreator visitorSource_;
  sourceCreator parserGrammar_;
  sourceCreator lexerGrammar_;
};