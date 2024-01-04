
// Generated from Expression.g4 by ANTLR 4.7.2


#include "ExpressionListener.h"

#include "ExpressionParser.h"


using namespace antlrcpp;
using namespace antlr4;

ExpressionParser::ExpressionParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

ExpressionParser::~ExpressionParser() {
  delete _interpreter;
}

std::string ExpressionParser::getGrammarFileName() const {
  return "Expression.g4";
}

const std::vector<std::string>& ExpressionParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& ExpressionParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ExprContext ------------------------------------------------------------------

ExpressionParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ExpressionParser::ExprContext::getRuleIndex() const {
  return ExpressionParser::RuleExpr;
}

void ExpressionParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ParenthsisContext ------------------------------------------------------------------

ExpressionParser::ExprContext* ExpressionParser::ParenthsisContext::expr() {
  return getRuleContext<ExpressionParser::ExprContext>(0);
}

ExpressionParser::ParenthsisContext::ParenthsisContext(ExprContext *ctx) { copyFrom(ctx); }

void ExpressionParser::ParenthsisContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParenthsis(this);
}
void ExpressionParser::ParenthsisContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParenthsis(this);
}
//----------------- AdditiviveContext ------------------------------------------------------------------

std::vector<ExpressionParser::ExprContext *> ExpressionParser::AdditiviveContext::expr() {
  return getRuleContexts<ExpressionParser::ExprContext>();
}

ExpressionParser::ExprContext* ExpressionParser::AdditiviveContext::expr(size_t i) {
  return getRuleContext<ExpressionParser::ExprContext>(i);
}

ExpressionParser::AdditiviveContext::AdditiviveContext(ExprContext *ctx) { copyFrom(ctx); }

void ExpressionParser::AdditiviveContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAdditivive(this);
}
void ExpressionParser::AdditiviveContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAdditivive(this);
}
//----------------- IdContext ------------------------------------------------------------------

tree::TerminalNode* ExpressionParser::IdContext::ID() {
  return getToken(ExpressionParser::ID, 0);
}

ExpressionParser::IdContext::IdContext(ExprContext *ctx) { copyFrom(ctx); }

void ExpressionParser::IdContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterId(this);
}
void ExpressionParser::IdContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitId(this);
}
//----------------- MultiplacativeContext ------------------------------------------------------------------

std::vector<ExpressionParser::ExprContext *> ExpressionParser::MultiplacativeContext::expr() {
  return getRuleContexts<ExpressionParser::ExprContext>();
}

ExpressionParser::ExprContext* ExpressionParser::MultiplacativeContext::expr(size_t i) {
  return getRuleContext<ExpressionParser::ExprContext>(i);
}

ExpressionParser::MultiplacativeContext::MultiplacativeContext(ExprContext *ctx) { copyFrom(ctx); }

void ExpressionParser::MultiplacativeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMultiplacative(this);
}
void ExpressionParser::MultiplacativeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMultiplacative(this);
}
//----------------- IntContext ------------------------------------------------------------------

tree::TerminalNode* ExpressionParser::IntContext::INT() {
  return getToken(ExpressionParser::INT, 0);
}

ExpressionParser::IntContext::IntContext(ExprContext *ctx) { copyFrom(ctx); }

void ExpressionParser::IntContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInt(this);
}
void ExpressionParser::IntContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInt(this);
}
//----------------- UnariyMinusContext ------------------------------------------------------------------

ExpressionParser::ExprContext* ExpressionParser::UnariyMinusContext::expr() {
  return getRuleContext<ExpressionParser::ExprContext>(0);
}

ExpressionParser::UnariyMinusContext::UnariyMinusContext(ExprContext *ctx) { copyFrom(ctx); }

void ExpressionParser::UnariyMinusContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnariyMinus(this);
}
void ExpressionParser::UnariyMinusContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ExpressionListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnariyMinus(this);
}

ExpressionParser::ExprContext* ExpressionParser::expr() {
   return expr(0);
}

ExpressionParser::ExprContext* ExpressionParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  ExpressionParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  ExpressionParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 0;
  enterRecursionRule(_localctx, 0, ExpressionParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(11);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ExpressionParser::T__0: {
        _localctx = _tracker.createInstance<UnariyMinusContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(3);
        match(ExpressionParser::T__0);
        setState(4);
        expr(6);
        break;
      }

      case ExpressionParser::T__4: {
        _localctx = _tracker.createInstance<ParenthsisContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(5);
        match(ExpressionParser::T__4);
        setState(6);
        expr(0);
        setState(7);
        match(ExpressionParser::T__5);
        break;
      }

      case ExpressionParser::INT: {
        _localctx = _tracker.createInstance<IntContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(9);
        match(ExpressionParser::INT);
        break;
      }

      case ExpressionParser::ID: {
        _localctx = _tracker.createInstance<IdContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(10);
        match(ExpressionParser::ID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(21);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(19);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MultiplacativeContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(13);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(14);
          _la = _input->LA(1);
          if (!(_la == ExpressionParser::T__1

          || _la == ExpressionParser::T__2)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(15);
          expr(6);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<AdditiviveContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(16);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(17);
          _la = _input->LA(1);
          if (!(_la == ExpressionParser::T__0

          || _la == ExpressionParser::T__3)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(18);
          expr(5);
          break;
        }

        } 
      }
      setState(23);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool ExpressionParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 0: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool ExpressionParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 5);
    case 1: return precpred(_ctx, 4);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> ExpressionParser::_decisionToDFA;
atn::PredictionContextCache ExpressionParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN ExpressionParser::_atn;
std::vector<uint16_t> ExpressionParser::_serializedATN;

std::vector<std::string> ExpressionParser::_ruleNames = {
  "expr"
};

std::vector<std::string> ExpressionParser::_literalNames = {
  "", "'-'", "'*'", "'/'", "'+'", "'('", "')'"
};

std::vector<std::string> ExpressionParser::_symbolicNames = {
  "", "", "", "", "", "", "", "INT", "ID", "WS"
};

dfa::Vocabulary ExpressionParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> ExpressionParser::_tokenNames;

ExpressionParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0xb, 0x1b, 0x4, 0x2, 0x9, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0xe, 
    0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x7, 0x2, 0x16, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x19, 0xb, 0x2, 0x3, 0x2, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x2, 0x4, 0x3, 0x2, 0x4, 0x5, 0x4, 0x2, 0x3, 
    0x3, 0x6, 0x6, 0x2, 0x1e, 0x2, 0xd, 0x3, 0x2, 0x2, 0x2, 0x4, 0x5, 0x8, 
    0x2, 0x1, 0x2, 0x5, 0x6, 0x7, 0x3, 0x2, 0x2, 0x6, 0xe, 0x5, 0x2, 0x2, 
    0x8, 0x7, 0x8, 0x7, 0x7, 0x2, 0x2, 0x8, 0x9, 0x5, 0x2, 0x2, 0x2, 0x9, 
    0xa, 0x7, 0x8, 0x2, 0x2, 0xa, 0xe, 0x3, 0x2, 0x2, 0x2, 0xb, 0xe, 0x7, 
    0x9, 0x2, 0x2, 0xc, 0xe, 0x7, 0xa, 0x2, 0x2, 0xd, 0x4, 0x3, 0x2, 0x2, 
    0x2, 0xd, 0x7, 0x3, 0x2, 0x2, 0x2, 0xd, 0xb, 0x3, 0x2, 0x2, 0x2, 0xd, 
    0xc, 0x3, 0x2, 0x2, 0x2, 0xe, 0x17, 0x3, 0x2, 0x2, 0x2, 0xf, 0x10, 0xc, 
    0x7, 0x2, 0x2, 0x10, 0x11, 0x9, 0x2, 0x2, 0x2, 0x11, 0x16, 0x5, 0x2, 
    0x2, 0x8, 0x12, 0x13, 0xc, 0x6, 0x2, 0x2, 0x13, 0x14, 0x9, 0x3, 0x2, 
    0x2, 0x14, 0x16, 0x5, 0x2, 0x2, 0x7, 0x15, 0xf, 0x3, 0x2, 0x2, 0x2, 
    0x15, 0x12, 0x3, 0x2, 0x2, 0x2, 0x16, 0x19, 0x3, 0x2, 0x2, 0x2, 0x17, 
    0x15, 0x3, 0x2, 0x2, 0x2, 0x17, 0x18, 0x3, 0x2, 0x2, 0x2, 0x18, 0x3, 
    0x3, 0x2, 0x2, 0x2, 0x19, 0x17, 0x3, 0x2, 0x2, 0x2, 0x5, 0xd, 0x15, 
    0x17, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

ExpressionParser::Initializer ExpressionParser::_init;
