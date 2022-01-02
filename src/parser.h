#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include "ast.h"
#include "merr.h"

#define PREFIX_PARSE_FN_LEN 32
#define INFIX_PARSE_FN_LEN 32
#define PRECEDENCE_LEN 16

typedef Expression* (*prefixParseFn)(void*);
typedef Expression* (*infixParseFn)(void*, Expression*);

typedef struct Parser {
    Lexer* l;
    Errors* errors;

    Token* curToken;
    Token* peekToken;

    prefixParseFn prefixParseFns[PREFIX_PARSE_FN_LEN];
    infixParseFn infixParseFns[INFIX_PARSE_FN_LEN];
} Parser;

typedef enum _Precedence {
    PR_LOWEST,
    PR_EQUALS,
    PR_LESSGREATER,
    PR_SUM,
    PR_PRODUCT,
    PR_PREFIX,
    PR_CALL,
} Precedence;

extern int precedences[PRECEDENCE_LEN];

Parser* newParser(Lexer* l);
void parseNextToken(Parser* p);
Program* parseProgram(Parser* p);
void InitParser();
void InitPrecedences();

#endif
