#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include "ast.h"
#include "merr.h"

typedef Expression* (*prefixParseFn)();
typedef Expression* (*infixParseFn)(Expression*);

typedef struct Parser {
    Lexer* l;
    Errors* errors;

    Token* curToken;
    Token* peekToken;

    prefixParseFn prefixParseFns[100];
    infixParseFn infixParseFns[100];
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

Parser* newParser(Lexer* l);
void parseNextToken(Parser* p);
Program* parseProgram(Parser* p);

#endif
