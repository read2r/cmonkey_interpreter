#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

typedef struct _TestList {
    int len;
    Token* arr[100];
} TestList;

typedef Token TestToken;

void appendTest(TestList* tl, TestToken* tt) {
    tl->arr[tl->len] = tt;
    tl->len++;
}

void TestNextToken1() {
    char* input = "=+(){},;";

    TestList* tests = (TestList*)malloc(sizeof(TestList));
    appendTest(tests, newToken(TOKEN_ASSIGN, '='));
    appendTest(tests, newToken(TOKEN_PLUS, '+'));
    appendTest(tests, newToken(TOKEN_LPAREN, '{'));
    appendTest(tests, newToken(TOKEN_RPAREN, '}'));
    appendTest(tests, newToken(TOKEN_LBRACE, '{'));
    appendTest(tests, newToken(TOKEN_RBRACE, '}'));
    appendTest(tests, newToken(TOKEN_COMMA, ','));
    appendTest(tests, newToken(TOKEN_SEMICOLON, ';'));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = tests->arr[i];
        printf("%d, %s\n", tok->type, tok->literal);
        printf("%d, %s\n\n", tt->type, tt->literal);
        free(tok);
    }
}

int main() {
    TestNextToken1();
    return 0;
}
