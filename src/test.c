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
    appendTest(tests, newTestToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_PLUS], "+"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LPAREN], "("));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RPAREN], ")"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LBRACE], "{"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RBRACE], "}"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_COMMA], ","));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = getTestAt(tests, i);

        // comparing the memory addresses, not the contents of two.
        // same TokenType, same address.
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
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LET], "let"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "five"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // let ten = 10;
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LET], "let"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "ten"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // let add = fn(x, y) {
    //     x + y;
    // };
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LET], "let"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "add"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_FUNCTION], "fn"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LPAREN], "("));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "x"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_COMMA], ","));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "y"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RPAREN], ")"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LBRACE], "{"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "x"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_PLUS], "+"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "y"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RBRACE], "}"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // let result = add(five, ten);
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LET], "let"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "result"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "add"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LPAREN], "("));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "five"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_COMMA], ","));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IDENT], "ten"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RPAREN], ")"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = getTestAt(tests, i);

        // comparing the memory addresses, not the contents of two.
        // same TokenType, same address.
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

void TestNextToken3() {
    char* input = "!-/*5;\
                   5 < 10 > 5;";

    TestList* tests = (TestList*)malloc(sizeof(TestList));
    // !-/*5;
    appendTest(tests, newTestToken(TOKENTYPES[CODE_BANG], "!"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_MINUS], "-"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SLASH], "/"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_ASTERISK], "*"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // 5 < 10 > 5;
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LT], "<"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_GT], ">"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = getTestAt(tests, i);

        // comparing the memory addresses, not the contents of two.
        // same TokenType, same address.
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

void TestNextToken4() {
    char* input = "if (5 < 10) {\
                       return true;\
                   } else {\
                       return false;\
                   }";

    TestList* tests = (TestList*)malloc(sizeof(TestList));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_IF], "if"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LPAREN], "("));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LT], "<"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RPAREN], ")"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LBRACE], "{"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RETURN], "return"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_TRUE], "true"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RBRACE], "}"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_ELSE], "else"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_LBRACE], "{"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RETURN], "return"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_FALSE], "false"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_RBRACE], "}"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = getTestAt(tests, i);

        // comparing the memory addresses, not the contents of two.
        // same TokenType, same address.
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

void TestNextToken5() {
    char* input = "10 == 10;\
                   10 != 9;";

    TestList* tests = (TestList*)malloc(sizeof(TestList));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_EQ], "=="));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_NOT_EQ], "!="));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_INT], "9"));
    appendTest(tests, newTestToken(TOKENTYPES[CODE_SEMICOLON], ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        TestToken* tt = getTestAt(tests, i);

        // comparing the memory addresses, not the contents of two.
        // same TokenType, same address.
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

void Init() {
    InitializeTokenTypes();
    InitializeKeywords();
}

int main() {
    Init();
    TestNextToken1();
    TestNextToken2();
    TestNextToken3();
    TestNextToken4();
    TestNextToken5();
    return 0;
}
