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
    appendTest(tests, newTestToken(TOKENTYPES[TAG_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_PLUS], "+"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LPAREN], "("));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_RPAREN], ")"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LBRACE], "{"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_RBRACE], "}"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_COMMA], ","));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_SEMICOLON], ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = getTestAt(tests, i);

        if(!(tok->type == tt->expectedType)) {
            printf("tests[%d] tokentype wrong. expected=%s, got=%s.\n", i, tt->expectedType, tok->type);
            exit(1);
        }

        if(strcmp(tok->literal, tt->expectedLiteral)) {
            printf("tests[%d] literal wrong. expected=%s, got = %s.\n", i, tt->expectedLiteral, tok->literal);
            exit(1);
        }

        free(tok->literal);
        free(tok);

        free(tt->expectedLiteral);
        free(tt);
    } 

    free(tests);
    printf("test ok\n");
}

void TestNextToken2() {
    char* input = "let five = 5;\
                   let ten = 10;\
                   \
                   let add = fn(x, y) {\
                       x + y;\
                   };\
                   let result = add(five, ten);";

    TestList* tests = (TestList*)malloc(sizeof(TestList));
    // let five = 5;
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LET], "let"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "five"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_INT], "5"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_SEMICOLON], ";"));
    // let ten = 10;
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LET], "let"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "ten"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_INT], "10"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_SEMICOLON], ";"));
    // let add = fn(x, y) {
    //     x + y;
    // };
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LET], "let"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "add"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_FUNCTION], "fn"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LPAREN], "("));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "x"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_COMMA], ","));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "y"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_RPAREN], ")"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LBRACE], "{"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "x"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_PLUS], "+"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "y"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_SEMICOLON], ";"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_RBRACE], "}"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_SEMICOLON], ";"));
    // let result = add(five, ten);
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LET], "let"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "result"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "add"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_LPAREN], "("));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "five"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_COMMA], ","));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_IDENT], "ten"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_RPAREN], ")"));
    appendTest(tests, newTestToken(TOKENTYPES[TAG_SEMICOLON], ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = getTestAt(tests, i);

        if(!(tok->type == tt->expectedType)) {
            printf("tests[%d] tokentype wrong. expected=%s, got=%s.\n", i, tt->expectedType, tok->type);
            exit(1);
        }

        if(strcmp(tok->literal, tt->expectedLiteral)) {
            printf("tests[%d] literal wrong. expected=%s, got=%s.\n", i, tt->expectedLiteral, tok->literal);
            exit(1);
        }

        free(tok->literal);
        free(tok);

        free(tt->expectedLiteral);
        free(tt);
    }

    free(tests);
    printf("test ok\n");
}

int main() {
    InitializeToken();
    TestNextToken1();
    TestNextToken2();
    return 0;
}
