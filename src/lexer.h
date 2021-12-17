#ifndef __LEXER_H__
#define __LEXER_H__
#include "token.h"

typedef struct _Lexer {
    char* input;
    int position;
    int readPosition;
    char ch;
} Lexer;

Lexer* newLexer(const char* input);
void readChar(Lexer* l);
Token* nextToken(Lexer* l);
Token* newToken(TokenType tokenType, char ch);
void skipWhitespace(Lexer* l);

#endif
