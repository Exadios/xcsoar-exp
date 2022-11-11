#pragma once

#include "MakeListener.h"

class MakeHooks : public MakeListener
  {
public:
  /**
   * Ctor.
   */
  MakeHooks();

  /**
   * Dtor.
   */
  virtual ~MakeHooks();

#if 1
  virtual void visitTerminal(antlr4::tree::TerminalNode *node) override;
  virtual void visitErrorNode(antlr4::tree::ErrorNode *node) override;
  virtual void enterEveryRule(antlr4::ParserRuleContext *ctx) override;
  virtual void exitEveryRule(antlr4::ParserRuleContext *ctx) override;

  virtual void enterProject(MakeParser::ProjectContext *ctx) override;
  virtual void exitProject(MakeParser::ProjectContext *ctx) override;

  virtual void enterSentences(MakeParser::SentencesContext *ctx) override;
  virtual void exitSentences(MakeParser::SentencesContext *ctx) override;


 virtual void enterTypeAssignment(MakeParser::TypeAssignmentContext *ctx) override;
  virtual void exitTypeAssignment(MakeParser::TypeAssignmentContext *ctx) override;



  virtual void enterAssignment(MakeParser::AssignmentContext *ctx) override;
  virtual void exitAssignment(MakeParser::AssignmentContext *ctx) override;

  virtual void enterVar(MakeParser::VarContext *ctx) override;
  virtual void exitVar(MakeParser::VarContext *ctx) override;
#endif
  };
