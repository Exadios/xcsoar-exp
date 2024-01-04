
// Generated from Expression.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "ExpressionListener.h"


/**
 * This class provides an empty implementation of ExpressionListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ExpressionBaseListener : public ExpressionListener {
public:

  virtual void enterParenthsis(ExpressionParser::ParenthsisContext * /*ctx*/) override { }
  virtual void exitParenthsis(ExpressionParser::ParenthsisContext * /*ctx*/) override { }

  virtual void enterAdditivive(ExpressionParser::AdditiviveContext * /*ctx*/) override { }
  virtual void exitAdditivive(ExpressionParser::AdditiviveContext * /*ctx*/) override { }

  virtual void enterId(ExpressionParser::IdContext * /*ctx*/) override { }
  virtual void exitId(ExpressionParser::IdContext * /*ctx*/) override { }

  virtual void enterMultiplacative(ExpressionParser::MultiplacativeContext * /*ctx*/) override { }
  virtual void exitMultiplacative(ExpressionParser::MultiplacativeContext * /*ctx*/) override { }

  virtual void enterInt(ExpressionParser::IntContext * /*ctx*/) override { }
  virtual void exitInt(ExpressionParser::IntContext * /*ctx*/) override { }

  virtual void enterUnariyMinus(ExpressionParser::UnariyMinusContext * /*ctx*/) override { }
  virtual void exitUnariyMinus(ExpressionParser::UnariyMinusContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

