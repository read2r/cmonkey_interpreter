#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

Lexer* newLexer(const char* input) {
    Lexer* nl = (Lexer*)malloc(sizeof(Lexer));
    nl->input = (char*)malloc(sizeof(char) * (strlen(input) + 1));
    memcpy(nl->input, input, sizeof(char) * (strlen(input) + 1));
    readChar(nl);
    return nl;
}

void readChar(Lexer* l) {
    if(l->readPosition >= strlen(l->input)) {
        l->ch = 0;
    } else {
        l->ch = l->input[l->readPosition];
    }
    l->position = l->readPosition;
    l->readPosition++;
}

Token* nextToken(Lexer* l) {
    Token* tok = NULL;

    skipWhitespace(l);

    switch(l->ch) {
    case '=':
        tok = newToken(TOKEN_ASSIGN, l->ch);
        break;
    case';':
        tok = newToken(TOKEN_SEMICOLON, l->ch);
        break;
    case'(':
        tok = newToken(TOKEN_LPAREN, l->ch);
        break;
    case')':
        tok = newToken(TOKEN_RPAREN, l->ch);
        break;
    case',':
        tok = newToken(TOKEN_COMMA, l->ch);
        break;
    case'+':
        tok = newToken(TOKEN_PLUS, l->ch);
        break;
    case'{':
        tok = newToken(TOKEN_LBRACE, l->ch);
        break;
    case'}':
        tok = newToken(TOKEN_RBRACE, l->ch);
        break;
    case 0:
        tok = newToken(TOKEN_EOF, ' ');
        break;
    }

    readChar(l);
    return tok;
}

Token* newToken(TokenType tokenType, char ch) {
    Token* nt = (Token*)malloc(sizeof(Token));
    nt->type = tokenType;
    nt->literal = (char*)malloc(sizeof(char) * 2);
    nt->literal[0] = ch;
    nt->literal[1] = '\0';
    return nt;
}

void skipWhitespace(Lexer *l) {
    while(l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
        readChar(l);
    }
}
