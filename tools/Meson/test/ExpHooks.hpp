#pragma once

#include "ExpressionBaseListener.h"

class ExprHooks : public ExpressionBaseListener
  {
public:
  /**
   * Ctor.
   */
  ExprHooks();

  /**
   * Dtor.
   */
  virtual ~ExprHooks();

#if 1
  virtual void enterParenthsis(ExpressionParser::ParenthsisContext *ctx);
  virtual void exitParenthsis(ExpressionParser::ParenthsisContext *ctx);

  virtual void enterAdditivive(ExpressionParser::AdditiviveContext *ctx);
  virtual void exitAdditivive(ExpressionParser::AdditiviveContext *ctx);

  virtual void enterId(ExpressionParser::IdContext *ctx);
  virtual void exitId(ExpressionParser::IdContext *ctx);

  virtual void enterMultiplacative(ExpressionParser::MultiplacativeContext *ctx);
  virtual void exitMultiplacative(ExpressionParser::MultiplacativeContext *ctx);

  virtual void enterInt(ExpressionParser::IntContext *ctx);
  virtual void exitInt(ExpressionParser::IntContext *ctx);

  virtual void enterUnariyMinus(ExpressionParser::UnariyMinusContext *ctx);
  virtual void exitUnariyMinus(ExpressionParser::UnariyMinusContext *ctx);
#endif
  };
