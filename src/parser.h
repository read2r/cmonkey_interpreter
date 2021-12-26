#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include "ast.h"
#include "merr.h"

typedef struct Parser {
    Lexer* l;
    Token* curToken;
    Token* peekToken;
    Errors* errors;
} Parser;

Parser* newParser(Lexer* l);
void parseNextToken(Parser* p);
Program* parseProgram(Parser* p);

#endif
