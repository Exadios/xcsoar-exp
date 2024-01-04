#include <iostream>
#include <string>
#include "MakeListener.h"
#include "MakeHooks.hpp"


//------------------------------------------------------------------------------
MakeHooks::MakeHooks()
  {
  }

//------------------------------------------------------------------------------
MakeHooks::~MakeHooks()
  {
  }

#if 0
//------------------------------------------------------------------------------
void
MakeHooks::visitTerminal(antlr4::tree::TerminalNode *node)
  {
//  std::cerr << "**** " << __FILE__ << ", " << __LINE__ << " ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::visitErrorNode(antlr4::tree::ErrorNode *node)
  {
  std::cerr << "**** Error Node ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::enterEveryRule(antlr4::ParserRuleContext *ctx)
  {
//  std::cerr << "**** " << __FILE__ << ", " << __LINE__ << " ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::exitEveryRule(antlr4::ParserRuleContext *ctx)
  {
//  std::cerr << "**** " << __FILE__ << ", " << __LINE__ << " ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::enterProject(MakeParser::ProjectContext *ctx) 
  {
  std::cerr << "**** enterProject() ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::exitProject(MakeParser::ProjectContext *ctx)
  {
  std::cerr << "**** exitProject() ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::enterSentences(MakeParser::SentencesContext *ctx)
  {
  std::cerr << "**** enterSentences() ****" << std::endl;
  }
  
//------------------------------------------------------------------------------
void
MakeHooks::exitSentences(MakeParser::SentencesContext *ctx) 
  {
  std::cerr << "**** exitSentences() ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::enterTypeAssignment(MakeParser::TypeAssignmentContext *ctx)
  {
  std::cerr << "**** enterTypeAssignment() ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::exitTypeAssignment(MakeParser::TypeAssignmentContext *ctx)
  {
  std::cerr << "**** exitTypeAssignment() ****" << std::endl;
  }

//------------------------------------------------------------------------------
void
MakeHooks::enterAssignment(MakeParser::AssignmentContext *ctx) 
  {
  std::cerr << "**** enterAssignment() ****" << std::endl;
  }

//------------------------------------------------------------------------------
void MakeHooks::exitAssignment(MakeParser::AssignmentContext *ctx)
  {
  std::cerr << "**** exitAssignment() ****" << std::endl;
  std::string name = ctx->NAME()->getSymbol()->getText();
  std::cerr << "Name = " << name << std::endl;
  }

//------------------------------------------------------------------------------
void MakeHooks::enterVar(MakeParser::VarContext *ctx) 
  {
  std::cerr << "**** enterVar() ****" << std::endl;
  }

//------------------------------------------------------------------------------
void MakeHooks::exitVar(MakeParser::VarContext *ctx) 
  {
  std::cerr << "**** exitVar() ****" << std::endl;
  }

#endif
