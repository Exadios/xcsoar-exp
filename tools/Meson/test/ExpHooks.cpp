#include <iostream>
#include "ExpHooks.hpp"

//------------------------------------------------------------------------------
ExprHooks::ExprHooks()
  {
  }

//------------------------------------------------------------------------------
ExprHooks::~ExprHooks()
  {
  }

#if 1
//------------------------------------------------------------------------------
void
ExprHooks::enterParenthsis(ExpressionParser::ParenthsisContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::exitParenthsis(ExpressionParser::ParenthsisContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::enterAdditivive(ExpressionParser::AdditiviveContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::exitAdditivive(ExpressionParser::AdditiviveContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::enterId(ExpressionParser::IdContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::exitId(ExpressionParser::IdContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::enterMultiplacative(ExpressionParser::MultiplacativeContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::exitMultiplacative(ExpressionParser::MultiplacativeContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::enterInt(ExpressionParser::IntContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::exitInt(ExpressionParser::IntContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::enterUnariyMinus(ExpressionParser::UnariyMinusContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }

//------------------------------------------------------------------------------
void
ExprHooks::exitUnariyMinus(ExpressionParser::UnariyMinusContext *ctx)
  {
  std::cerr << __FILE__ << ", " << __LINE__ << std::endl;
  }
#endif
