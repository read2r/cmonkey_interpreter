#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

void InitializeTokenTypes() {
    TokenTypeStringList[TOKEN_ILLEGAL] = "ILLEGAL";
    TokenTypeStringList[TOKEN_EOF] = "EOF";
    TokenTypeStringList[TOKEN_IDENT] = "IDENT";
    TokenTypeStringList[TOKEN_INT] = "INT";
    TokenTypeStringList[TOKEN_ASSIGN] = "=";
    TokenTypeStringList[TOKEN_PLUS] = "+";
    TokenTypeStringList[TOKEN_MINUS] = "-";
    TokenTypeStringList[TOKEN_BANG] = "!";
    TokenTypeStringList[TOKEN_ASTERISK] = "*";
    TokenTypeStringList[TOKEN_SLASH] = "/";
    TokenTypeStringList[TOKEN_LT] = "<";
    TokenTypeStringList[TOKEN_GT] = ">";
    TokenTypeStringList[TOKEN_EQ] = "==";
    TokenTypeStringList[TOKEN_NOT_EQ] = "!=";
    TokenTypeStringList[TOKEN_COMMA] = ",";
    TokenTypeStringList[TOKEN_SEMICOLON] = ";";
    TokenTypeStringList[TOKEN_LPAREN] = "(";
    TokenTypeStringList[TOKEN_RPAREN] = "";
    TokenTypeStringList[TOKEN_LBRACE] = "{";
    TokenTypeStringList[TOKEN_RBRACE] = "}";
    TokenTypeStringList[TOKEN_FUNCTION] = "FUNCTION";
    TokenTypeStringList[TOKEN_LET] = "LET";
    TokenTypeStringList[TOKEN_TRUE] = "ture";
    TokenTypeStringList[TOKEN_FALSE] = "false";
    TokenTypeStringList[TOKEN_IF] = "if";
    TokenTypeStringList[TOKEN_ELSE] = "else";
    TokenTypeStringList[TOKEN_RETURN] = "return";
}

char* getTokenTypeString(TokenType tokenType) {
    return TokenTypeStringList[tokenType];
}

Keyword* newKeyword(TokenType tokenType, const char* literal) {
    Keyword* temp = (Keyword*)malloc(sizeof(Keyword));
    temp->tokenType = tokenType;
    temp->literal = (char*)malloc(sizeof(char) * STR_SIZE);
    memcpy(temp->literal, literal, STR_SIZE);
    return temp;
}

void appendKeyword(TokenType tokenType, const char* literal) {
    KEYWORDS->arr[KEYWORDS->len] = newKeyword(tokenType, literal);
    KEYWORDS->len++;
}

void InitializeKeywords() {
    KEYWORDS = (Keywords*)malloc(sizeof(Keywords));
    KEYWORDS->len = 0;
    appendKeyword(TOKEN_FUNCTION, "fn");
    appendKeyword(TOKEN_LET, "let");
    appendKeyword(TOKEN_TRUE, "true");
    appendKeyword(TOKEN_FALSE, "false");
    appendKeyword(TOKEN_IF, "if");
    appendKeyword(TOKEN_ELSE, "else");
    appendKeyword(TOKEN_RETURN, "return");
}

TokenType LookupIdent(char* ident) {
    for(int i = 0; i < KEYWORDS->len; i++) {
        Keyword* keyword = KEYWORDS->arr[i];
        if(!strcmp(keyword->literal, ident)) {
            return keyword->tokenType;
        }
    }
    return TOKEN_IDENT;
}

void InitToken() {
    InitializeTokenTypes();
    InitializeKeywords();
}
