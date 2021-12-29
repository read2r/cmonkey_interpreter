#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "merr.h"

typedef int TestType;

typedef struct _Test {
    TestType testType;
} Test;

enum {
    TT_TOKEN,
    TT_IDENTIFIER,
};

typedef struct _ExpectedIdentifier {
    TestType testType;
    char* expectedLiteral;
} ExpectedIdentifier;

typedef struct _ExpectedToken {
    TestType testType;
    TokenType expectedType;
    char* expectedLiteral;
} ExpectedToken;

typedef struct _TestList {
    int len;
    Test* arr[100];
} TestList;

TestList* newTestList() {
    TestList* tl = (TestList*)malloc(sizeof(TestList));
    tl->len = 0;
    return tl;
}

ExpectedToken* newExpectedToken(TokenType tokenType, const char* literal) {
    ExpectedToken* et = (ExpectedToken*)malloc(sizeof(ExpectedToken));
    et->testType = TT_TOKEN;
    et->expectedType = tokenType;
    et->expectedLiteral = (char*)malloc(sizeof(char) * (strlen(literal) + 1));
    memcpy(et->expectedLiteral, literal, sizeof(char) * (strlen(literal) + 1));
    return et;
}

ExpectedIdentifier* newExpectedIdentifier(const char* ident) {
    ExpectedIdentifier* ei =(ExpectedIdentifier*)malloc(sizeof(ExpectedIdentifier));
    ei->testType = TT_IDENTIFIER;
    ei->expectedLiteral = (char*)malloc(sizeof(char) * (strlen(ident) + 1));
    memcpy(ei->expectedLiteral, ident, sizeof(char) * (strlen(ident) + 1));
    return ei;
}

void appendTest(TestList* tl, Test* t) {
    tl->arr[tl->len] = t;
    tl->len++;
}

Test* getTestAt(TestList* tl, unsigned int idx) {
    if(idx > tl->len) {
        return NULL;
    }
    return tl->arr[idx];
}

void freeExpectedToken(ExpectedToken* et) {
    free(et->expectedLiteral);
    free(et);
}

void freeExpectedIdentifier(ExpectedIdentifier* ei) {
    free(ei->expectedLiteral);
    free(ei);
}

void freeTest(Test* t) {
    if(t->testType == TT_TOKEN) {
        freeExpectedToken((ExpectedToken*)t);
    } else if(t->testType == TT_IDENTIFIER) {
        freeExpectedIdentifier((ExpectedIdentifier*)t);
    }
}

void checkParserErrors(Parser* p) {
    Errors* errors = p->errors;
    if(errors->len == 0) {
        return;
    }

    printfError("parse has %d errors", errors->len);
    for(int i = 0; i < errors->len; i++) {
        printfError("parser error: %s", errors->arr[i]);
    }

    freeErrors(errors);

    exit(1);
}

void TestNextToken() {
    char* input = "=+(){},;\
                   \
                   let five = 5;\
                   let ten = 10;\
                   \
                   let add = fn(x, y) {\
                       x + y;\
                   };\
                   \
                   let result = add(five, ten);\
                   !-*5/;\
                   5 < 10 > 5;\
                   \
                   if (5 < 10) {\
                       return true;\
                   } else {\
                       return false;\
                   }\
                   \
                   10 == 10;\
                   10 != 9;";
                    

    TestList* tests = newTestList();
    // =+(){},;
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_PLUS], "+"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LPAREN], "("));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RPAREN], ")"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LBRACE], "{"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RBRACE], "}"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_COMMA], ","));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // let five = 5;
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LET], "let"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "five"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // let ten = 10;
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LET], "let"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "ten"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // let add = fn(x, y) {
    //     x + y;
    // };
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LET], "let"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "add"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_FUNCTION], "fn"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LPAREN], "("));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "x"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_COMMA], ","));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "y"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RPAREN], ")"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LBRACE], "{"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "x"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_PLUS], "+"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "y"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RBRACE], "}"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // let result = add(five, ten);
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LET], "let"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "result"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_ASSIGN], "="));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "add"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LPAREN], "("));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "five"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_COMMA], ","));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IDENT], "ten"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RPAREN], ")"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // !-*5/;
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_BANG], "!"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_MINUS], "-"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_ASTERISK], "*"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SLASH], "/"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // 5 < 10 > 5;
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LT], "<"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_GT], ">"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    // if (5 < 10) {
    //     return true;
    // } else {
    //     return false;
    // }
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_IF], "if"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LPAREN], "("));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LT], "<"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RPAREN], ")"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LBRACE], "{"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RETURN], "return"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_TRUE], "true"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RBRACE], "}"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_ELSE], "else"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_LBRACE], "{"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RETURN], "return"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_FALSE], "false"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_RBRACE], "}"));
    // 10 == 10;
    // 10 != 9;
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_EQ], "=="));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_NOT_EQ], "!="));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_INT], "9"));
    appendTest(tests, (Test*)newExpectedToken(TOKENTYPES[CODE_SEMICOLON], ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        ExpectedToken* et = (ExpectedToken*)getTestAt(tests, i);

        // comparing the memory addresses, not the contents of two.
        // same TokenType, same address.
        if(!(tok->type == et->expectedType)) {
            printfError("tests[%d] tokentype wrong. expected=%s, got=%s.\n", 
                    i, et->expectedType, tok->type);
            exit(1);
        }

        if(strcmp(tok->literal, et->expectedLiteral)) {
            printfError("tests[%d] literal wrong. expected=%s, got = %s.\n", 
                    i, et->expectedLiteral, tok->literal);
            exit(1);
        }

        free(tok->literal);
        free(tok);

        freeExpectedToken(et);
    } 

    free(tests);
    printf("test ok\n");
}

int TestLetStatement(Statement* s, const char* name) {
    if(strcmp(TokenLiteral((Node*)s), "let")) {
        printfError("1s.TokenLiteral not 'let'. got=%s\n", 
                TokenLiteral((Node*)s));
        return 0;
    }

    LetStatement* letStmt = (LetStatement*)s;
    if(letStmt->nodeType != CODE_LET_STATEMENT) {
        printfError("s not LetStatement. got=%d\n", s->nodeType);
        return 0;
    }

    if(strcmp(letStmt->name->value, name)) {
        printfError("letStmt->name->value not '%s'. got=%s\n", 
                name, letStmt->name->value);
        return 0;
    }

    if(strcmp(TokenLiteral((Node*)(letStmt->name)), name)) {
        printfError("TokenLiteral(letStmt->name) not '%s'. got=%s\n", 
                name, TokenLiteral((Node*)(letStmt->name)));
        return 0;
    }

    return 1;
}

void TestLetStatements() {
    char* input = "let x = 5;\
                   let y = 10;\
                   let foobar = 838383;";

    Lexer* l = newLexer(input);
    Parser* p = newParser(l);

    Program* program = parseProgram(p);
    checkParserErrors(p);

    if(program == NULL) {
        printfError("ParseProgram() return NULL\n");
        exit(1);
    }

    if(program->len != 3) {
        printfError("program.statements does not contain 3 statements. got=%d\n", 
                program->len);
        exit(1);
    }

    TestList* tests = newTestList();
    appendTest(tests, (Test*)newExpectedIdentifier("x"));
    appendTest(tests, (Test*)newExpectedIdentifier("y"));
    appendTest(tests, (Test*)newExpectedIdentifier("foobar"));

    for(int i = 0; i < tests->len; i++) {
        ExpectedIdentifier* ei = (ExpectedIdentifier*)getTestAt(tests, i);
        Statement* stmt = program->statements[i];

        if(!TestLetStatement(stmt, ei->expectedLiteral)) {
            return;
        }
    }

    printf("test ok\n");
}

void TestReturnStatements() {
    char* input = "return 5;\
                   return 10;\
                   return 993322;";

    Lexer* l = newLexer(input);
    Parser* p = newParser(l);

    Program* program = parseProgram(p);
    checkParserErrors(p);

    if(program->len != 3) {
        printfError("program.statements does not contain 3 statements. got=%d", 
                program->len);
        exit(1);
    }
    
    for(int i = 0; i < program->len; i++) {
        ReturnStatement* returnStmt = (ReturnStatement*)program->statements[i];
        NodeType nt = returnStmt->nodeType;
        if(nt != CODE_RETURN_STATEMENT) {
            printfError("stmt not ReturnStatement. got=%s\n", 
                    NodeTypeString[returnStmt->nodeType]);
            continue;
        }

        if(strcmp(TokenLiteral((Node*)returnStmt), "return")) {
            printfError("returnStmt.TokenLiteral not 'return', got=%s\n",
                    TokenLiteral((Node*)returnStmt));
            exit(1);
        }
    }

    printf("test ok\n");
}

void TestString() {
    char* input = "let myVar = 123;";

    Lexer* l = newLexer(input);
    Parser* p = newParser(l);

    Program* program = parseProgram(p);
    checkParserErrors(p);
    
    printf("%s\n", ToString((Node*)program));
    printf("%s\n", input);
    if(strcmp(ToString((Node*)program), input)) {
        printfError("program.String() wrong. get=%s", ToString((Node*)program));
        exit(1);
    }

    printf("test ok\n");
}

void Init() {
    InitializeTokenTypes();
    InitializeKeywords();
    InitTokenLiteralList();
}

int main() {
    Init();
    TestNextToken();
    TestLetStatements();
    TestReturnStatements();
    TestString();
    return 0;
}
