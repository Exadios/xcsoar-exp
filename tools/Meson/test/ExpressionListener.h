
// Generated from Expression.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "ExpressionParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ExpressionParser.
 */
class  ExpressionListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterParenthsis(ExpressionParser::ParenthsisContext *ctx) = 0;
  virtual void exitParenthsis(ExpressionParser::ParenthsisContext *ctx) = 0;

  virtual void enterAdditivive(ExpressionParser::AdditiviveContext *ctx) = 0;
  virtual void exitAdditivive(ExpressionParser::AdditiviveContext *ctx) = 0;

  virtual void enterId(ExpressionParser::IdContext *ctx) = 0;
  virtual void exitId(ExpressionParser::IdContext *ctx) = 0;

  virtual void enterMultiplacative(ExpressionParser::MultiplacativeContext *ctx) = 0;
  virtual void exitMultiplacative(ExpressionParser::MultiplacativeContext *ctx) = 0;

  virtual void enterInt(ExpressionParser::IntContext *ctx) = 0;
  virtual void exitInt(ExpressionParser::IntContext *ctx) = 0;

  virtual void enterUnariyMinus(ExpressionParser::UnariyMinusContext *ctx) = 0;
  virtual void exitUnariyMinus(ExpressionParser::UnariyMinusContext *ctx) = 0;


};

