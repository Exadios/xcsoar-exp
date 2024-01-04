#include <iostream>
#include "antlr4-runtime.h"
#include "ExpressionLexer.h"
#include "ExpressionParser.h"

//------------------------------------------------------------------------------
int main(int argc, const char* argv[])
  {

  // Provide the input text in a stream
  antlr4::ANTLRInputStream input("6*(2+3)");
    
  // Create a lexer from the input
  ExpressionLexer lexer(&input);
    
  // Create a token stream from the lexer
  antlr4::CommonTokenStream tokens(&lexer);
    
  // Create a parser from the token stream
  ExpressionParser parser(&tokens);    

  // Display the parse tree
  std::cout << parser.expr()->toStringTree() << std::endl;
  return 0;
  }
