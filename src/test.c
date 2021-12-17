#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "token.h"
#include "lexer.h"

typedef struct _TestToken {
    TokenType expectedType;
    char* expectedLiteral;
} TestToken;

typedef struct _TestList {
    int len;
    TestToken* arr[100];
} TestList;

TestToken* newTestToken(TokenType tokenType, const char* literal) {
    TestToken* tt = (TestToken*)malloc(sizeof(TestToken));
    tt->expectedType = tokenType;
    tt->expectedLiteral = (char*)malloc(sizeof(char) * (strlen(literal) + 1));
    memcpy(tt->expectedLiteral, literal, sizeof(char) * (strlen(literal) + 1));
    return tt;
}

void appendTest(TestList* tl, TestToken* tt) {
    tl->arr[tl->len] = tt;
    tl->len++;
}

TestToken* getTestAt(TestList* tl, int idx) {
    return tl->arr[idx];
}

void TestNextToken1() {
    char* input = "=+(){},;";

    TestList* tests = (TestList*)malloc(sizeof(TestList));
    appendTest(tests, newTestToken(TOKEN_ASSIGN, "="));
    appendTest(tests, newTestToken(TOKEN_PLUS, "+"));
    appendTest(tests, newTestToken(TOKEN_LPAREN, "("));
    appendTest(tests, newTestToken(TOKEN_RPAREN, ")"));
    appendTest(tests, newTestToken(TOKEN_LBRACE, "{"));
    appendTest(tests, newTestToken(TOKEN_RBRACE, "}"));
    appendTest(tests, newTestToken(TOKEN_COMMA, ","));
    appendTest(tests, newTestToken(TOKEN_SEMICOLON, ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = getTestAt(tests, i);

        if(!(tok->type == tt->expectedType)) {
            printf("tests[%d] tokentype wrong. expected=%d, got=%d.\n", i, tt->expectedType, tok->type);
            exit(1);
        }

        if(strcmp(tok->literal, tt->expectedLiteral)) {
            printf("tests[%d] literal wrong. expected=%s, got = %s.\n", i, tt->expectedLiteral, tok->literal);
            exit(1);
        }
    }

    printf("test ok\n");
}

int main() {
    TestNextToken1();
    return 0;
}
