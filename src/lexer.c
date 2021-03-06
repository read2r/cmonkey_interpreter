#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

Lexer* newLexer(const char* input) {
    Lexer* nl = (Lexer*)malloc(sizeof(Lexer));
    nl->input = (char*)malloc(sizeof(char) * (strlen(input) + 1));
    memcpy(nl->input, input, sizeof(char) * (strlen(input) + 1));
    nl->position = 0;
    nl->readPosition = 0;
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

char peekChar(Lexer* l) {
    if(l->readPosition >= strlen(l->input)) {
        return 0;
    } else {
        return l->input[l->readPosition];
    }
}

Token* nextToken(Lexer* l) {
    Token* tok = NULL;

    skipWhitespace(l);

    switch(l->ch) {
    case '=':
        if(peekChar(l) == '=') {
            tok = (Token*)malloc(sizeof(Token));
            readChar(l);
            tok->tokenType = TOKEN_EQ;
            tok->literal = (char*)malloc(sizeof(char) * 3);
            strcpy(tok->literal, "==");
        } else {
            tok = newToken(TOKEN_ASSIGN, l->ch);
        }
        break;
    case '+':
        tok = newToken(TOKEN_PLUS, l->ch);
        break;
    case '-':
        tok = newToken(TOKEN_MINUS, l->ch);
        break;
    case '!':
        if(peekChar(l) == '=') {
            tok = (Token*)malloc(sizeof(Token));
            readChar(l);
            tok->tokenType = TOKEN_NOT_EQ;
            tok->literal = (char*)malloc(sizeof(char) * 3);
            strcpy(tok->literal, "!=");
        } else {
            tok = newToken(TOKEN_BANG, l->ch);
        }
        break;
    case '/':
        tok = newToken(TOKEN_SLASH, l->ch);
        break;
    case '*':
        tok = newToken(TOKEN_ASTERISK, l->ch);
        break;
    case '<':
        tok = newToken(TOKEN_LT, l->ch);
        break;
    case '>':
        tok = newToken(TOKEN_GT, l->ch);
        break;
    case ';':
        tok = newToken(TOKEN_SEMICOLON, l->ch);
        break;
    case '(':
        tok = newToken(TOKEN_LPAREN, l->ch);
        break;
    case ')':
        tok = newToken(TOKEN_RPAREN, l->ch);
        break;
    case ',':
        tok = newToken(TOKEN_COMMA, l->ch);
        break;
    case '{':
        tok = newToken(TOKEN_LBRACE, l->ch);
        break;
    case '}':
        tok = newToken(TOKEN_RBRACE, l->ch);
        break;
    case 0:
        tok = newToken(TOKEN_EOF, ' ');
        break;
    default:
        if(isLetter(l->ch)) {
            tok = (Token*)malloc(sizeof(Token));
            tok->literal = readIdentifier(l);
            tok->tokenType = LookupIdent(tok->literal);
            return tok;
        } else if(isDigit(l->ch)) {
            tok = (Token*)malloc(sizeof(Token));
            tok->tokenType = TOKEN_INT;
            tok->literal = readNumber(l);
            return tok;
        } else {
            tok = newToken(TOKEN_ILLEGAL, l->ch);
        }
    }

    readChar(l);
    return tok;
}

Token* newToken(TokenType tokenType, char ch) {
    Token* nt = (Token*)malloc(sizeof(Token));
    nt->tokenType = tokenType;
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

char* readIdentifier(Lexer* l) {
    int start, end, len;
    start = l->position;
    while(isLetter(l->ch)) {
        readChar(l);
    }
    end = l->position;
    len = end - start;

    char* ident = (char*)malloc(sizeof(char) * len + 1);
    memcpy(ident, (l->input + start), sizeof(char) * len);
    ident[len] = '\0';
    return ident;
}

int isLetter(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

char* readNumber(Lexer* l) {
    int start, end, len;
    start = l->position;
    while(isDigit(l->ch)) {
        readChar(l);
    }
    end = l->position;
    len = end - start;

    char* number = (char*)malloc(sizeof(char) * len + 1);
    memcpy(number, (l->input + start), sizeof(char) * len);
    number[len] = '\0';
    return number;
}

int isDigit(char ch) {
    return '0' <= ch && ch <= '9';
}
