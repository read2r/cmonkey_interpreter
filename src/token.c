#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

void appendTokenType(TokenTypeCode typeCode, const char* literal) {
    TOKENTYPES[typeCode] = (TokenType)malloc(sizeof(char) * STR_SIZE);
    memcpy(TOKENTYPES[typeCode], literal, sizeof(char) * STR_SIZE);
}

void InitializeTokenTypes() {
    appendTokenType(CODE_ILLEGAL, "ILLEGAL");
    appendTokenType(CODE_EOF, "EOF");
    appendTokenType(CODE_IDENT, "IDENT");
    appendTokenType(CODE_INT, "INT");
    appendTokenType(CODE_ASSIGN, "=");
    appendTokenType(CODE_PLUS, "+");
    appendTokenType(CODE_MINUS, "-");
    appendTokenType(CODE_BANG, "!");
    appendTokenType(CODE_ASTERISK, "*");
    appendTokenType(CODE_SLASH, "/");
    appendTokenType(CODE_LT, "<");
    appendTokenType(CODE_GT, ">");
    appendTokenType(CODE_EQ, "==");
    appendTokenType(CODE_NOT_EQ, "!=");
    appendTokenType(CODE_COMMA, ",");
    appendTokenType(CODE_SEMICOLON, ";");
    appendTokenType(CODE_LPAREN, "(");
    appendTokenType(CODE_RPAREN, ")");
    appendTokenType(CODE_LBRACE, "{");
    appendTokenType(CODE_RBRACE, "}");
    appendTokenType(CODE_FUNCTION, "FUNCTION");
    appendTokenType(CODE_LET, "LET");
    appendTokenType(CODE_TRUE, "ture");
    appendTokenType(CODE_FALSE, "false");
    appendTokenType(CODE_IF, "if");
    appendTokenType(CODE_ELSE, "else");
    appendTokenType(CODE_RETURN, "return");
}

Keyword* newKeyword(TokenTypeCode typeCode, const char* literal) {
    Keyword* temp = (Keyword*)malloc(sizeof(Keyword));
    temp->typeCode = typeCode;
    temp->literal = (char*)malloc(sizeof(char) * STR_SIZE);
    memcpy(temp->literal, literal, STR_SIZE);
    return temp;
}

void appendKeyword(TokenTypeCode typeCode, const char* literal) {
    KEYWORDS->arr[KEYWORDS->len] = newKeyword(typeCode, literal);
    KEYWORDS->len++;
}

void InitializeKeywords() {
    KEYWORDS = (Keywords*)malloc(sizeof(Keywords));
    KEYWORDS->len = 0;
    appendKeyword(CODE_FUNCTION, "fn");
    appendKeyword(CODE_LET, "let");
    appendKeyword(CODE_TRUE, "true");
    appendKeyword(CODE_FALSE, "false");
    appendKeyword(CODE_IF, "if");
    appendKeyword(CODE_ELSE, "else");
    appendKeyword(CODE_RETURN, "return");
}

TokenType LookupIdent(char* ident) {
    for(int i = 0; i < KEYWORDS->len; i++) {
        Keyword* keyword = KEYWORDS->arr[i];
        if(!strcmp(keyword->literal, ident)) {
            return TOKENTYPES[keyword->typeCode];
        }
    }
    return TOKENTYPES[CODE_IDENT];
}
