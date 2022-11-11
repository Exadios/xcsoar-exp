#include <iostream>
#include "antlr4-runtime.h"
#include "MakeLexer.h"
#include "MakeParser.h"
#include "MakeHooks.hpp"

int main(int argc, const char* argv[])
  {

  // Provide the input text in a stream
  antlr4::ANTLRFileStream input("test-input.txt");
    
  // Create a lexer from the input
  MakeLexer lexer(&input);
    
  // Create a token stream from the lexer
  antlr4::CommonTokenStream tokens(&lexer);
    
  // Create a parser from the token stream
  MakeParser parser(&tokens);    

  // Display the parse tree
//  std::cout << parser.project()->toStringTree() << std::endl;

  antlr4::tree::ParseTree* tree = parser.project();
  MakeHooks* hooks = new MakeHooks();
  antlr4::tree::ParseTreeWalker* walker = new antlr4::tree::ParseTreeWalker();
  walker->walk(hooks, tree);

  MakeParser::ProjectContext* project = parser.project();

//  std::cout << "\nresult = " << hooks->result << std::endl;

  return 0;
  }
