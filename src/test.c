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

typedef enum _ParamType {
    PT_INT,
    PT_STRING,
    PT_BOOL,
} ParamType;

typedef struct _TestParam {
    ParamType paramType;
    union {
        int integer;
        int boolean;
        char *string;
    };
} TestParam;

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

int testIntegerLiteral(Expression* e, int value) {
    IntegerLiteral* il = (IntegerLiteral*)e;
    if(il->nodeType != NC_INTEGER_LITERAL) {
        printfError("il not IntegerLiteral. got=%d", il->nodeType);
        return 0;
    }

    if(il->value != value) {
        printfError("il->value not %d. got=%d", value, il->value);
        return 0;
    }

    if(atoi(TokenLiteral(il)) != value) {
        printfError("il.TokenLiteral not %d. got=%s", value, TokenLiteral(il));
        return 0;
    }

    return 1;
}

int testIdentifier(Expression* exp, const char* value) {
    Identifier* ident = (Identifier*)exp;
    if(ident->nodeType != NC_IDENTIFIER) {
        fprintf(stderr, "exp not Identifier. got=%d\n", ident->nodeType);
        return 0;
    }

    if(strcmp(ident->value, value)) {
        fprintf(stderr, "ident.Value not %s. got=%s\n", value, ident->value); 
        return 0;
    }

    if(strcmp(TokenLiteral(ident), value)) {
        fprintf(stderr, "ident.TokenLiteral not %s. got=%s\n", value, TokenLiteral(ident));
        return 0;
    }

    return 1;
}

int testBooleanLiteral(Expression* exp, int value) {
    Boolean* bo = (Boolean*)exp;
    if(bo->nodeType != NC_BOOLEAN) {
        fprintf(stderr, "exp not Boolean. got=%d\n", exp->nodeType);
        return 0;
    }

    if(bo->value != value) {
        fprintf(stderr, "bo.Value not %d. got=%d\n", value, bo->value);
        return 0;
    }

    char buf[8];
    if(value)
        sprintf(buf, "true");
    else
        sprintf(buf, "false");

    if(strcmp(TokenLiteral(bo), buf)) {
        fprintf(stderr, "bo.TokenLiteral not %s. got=%s\n", buf, TokenLiteral(bo));
        return 0;
    }

    return 1;
}

int testLiteralExpression(Expression *exp, TestParam *param) {
    switch(param->paramType) {
    case PT_INT:
        return testIntegerLiteral(exp, param->integer);
    case PT_STRING:
        return testIdentifier(exp, param->string);
    case PT_BOOL:
        return testBooleanLiteral(exp, param->boolean);
    }
    fprintf(stderr, "type of exp not handled. got=%d\n", exp->nodeType);
    return 0;
}

int testInfixExpression(Expression *exp, TestParam *left, char *operator, TestParam *right) {
    InfixExpression *ie = (InfixExpression*)exp;
    if(ie->nodeType != NC_INFIX_EXPRESSION) {
        fprintf(stderr, "exp is not InfixExpression. got=%d\n", exp->nodeType);
        return 0;
    }

    if(testLiteralExpression(ie->left, left)) {
        return 0;
    }

    if(strcmp(ie->op, operator)) {
        fprintf(stderr, "exp.Operator is not '%s' got=%s\n", operator, ie->op);
        return 0;
    }

    if(testLiteralExpression(ie->right, right)) {
        return 0;
    }

    return 1;
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
    appendTest(tests, (Test*)newExpectedToken(TOKEN_ASSIGN, "="));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_PLUS, "+"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LPAREN, "("));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RPAREN, ")"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LBRACE, "{"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RBRACE, "}"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_COMMA, ","));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    // let five = 5;
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LET, "let"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "five"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_ASSIGN, "="));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    // let ten = 10;
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LET, "let"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "ten"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_ASSIGN, "="));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    // let add = fn(x, y) {
    //     x + y;
    // };
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LET, "let"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "add"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_ASSIGN, "="));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_FUNCTION, "fn"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LPAREN, "("));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "x"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_COMMA, ","));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "y"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RPAREN, ")"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LBRACE, "{"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "x"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_PLUS, "+"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "y"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RBRACE, "}"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    // let result = add(five, ten);
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LET, "let"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "result"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_ASSIGN, "="));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "add"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LPAREN, "("));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "five"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_COMMA, ","));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IDENT, "ten"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RPAREN, ")"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    // !-*5/;
    appendTest(tests, (Test*)newExpectedToken(TOKEN_BANG, "!"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_MINUS, "-"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_ASTERISK, "*"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SLASH, "/"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    // 5 < 10 > 5;
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LT, "<"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_GT, ">"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    // if (5 < 10) {
    //     return true;
    // } else {
    //     return false;
    // }
    appendTest(tests, (Test*)newExpectedToken(TOKEN_IF, "if"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LPAREN, "("));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "5"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LT, "<"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RPAREN, ")"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LBRACE, "{"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RETURN, "return"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_TRUE, "true"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RBRACE, "}"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_ELSE, "else"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_LBRACE, "{"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RETURN, "return"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_FALSE, "false"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_RBRACE, "}"));
    // 10 == 10;
    // 10 != 9;
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_EQ, "=="));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "10"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_NOT_EQ, "!="));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_INT, "9"));
    appendTest(tests, (Test*)newExpectedToken(TOKEN_SEMICOLON, ";"));

    Lexer* l = newLexer(input);

    for(int i = 0; i < tests->len; i++) {
        Token* tok = nextToken(l);
        ExpectedToken* et = (ExpectedToken*)getTestAt(tests, i);

        // comparing the memory addresses, not the contents of two.
        // same TokenType, same address.
        if(!(tok->tokenType == et->expectedType)) {
            printfError("tests[%d] tokentype wrong. expected=%s, got=%s.\n", 
                    i, et->expectedType, tok->tokenType);
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
    if(strcmp(TokenLiteral(s), "let")) {
        printfError("1s.TokenLiteral not 'let'. got=%s\n", 
                TokenLiteral(s));
        return 0;
    }

    LetStatement* letStmt = (LetStatement*)s;
    if(letStmt->nodeType != NC_LET_STATEMENT) {
        printfError("s not LetStatement. got=%d\n", s->nodeType);
        return 0;
    }

    if(strcmp(letStmt->name->value, name)) {
        printfError("letStmt->name->value not '%s'. got=%s\n", 
                name, letStmt->name->value);
        return 0;
    }

    if(strcmp(TokenLiteral(letStmt->name), name)) {
        printfError("TokenLiteral(letStmt->name) not '%s'. got=%s\n", 
                name, TokenLiteral(letStmt->name));
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
        if(nt != NC_RETURN_STATEMENT) {
            printfError("stmt not ReturnStatement. got=%s\n", 
                    getNodeTypeString(returnStmt->nodeType));
            continue;
        }

        if(strcmp(TokenLiteral(returnStmt), "return")) {
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
    
    if(strcmp(ToString((Node*)program), input)) {
        printfError("program.String() wrong. get=%s", ToString((Node*)program));
        exit(1);
    }

    printf("test ok\n");
}

void TestIdentifierExpression() {
    char* input = "foobar;";

    Lexer* l = newLexer(input);
    Parser* p = newParser(l);

    Program* program = parseProgram(p);
    checkParserErrors(p);
    
    if(program->len != 1) {
        printfError("program has not enough statements. got=%d", program->len);
        exit(1);
    }

    ExpressionStatement* es = (ExpressionStatement*)program->statements[0];
    if(es->nodeType != NC_EXPRESSION_STATEMENT) {
        printfError("program.Statements[0] is not ExpressionStatement. got=%s",
                getNodeTypeString(es->nodeType));
        exit(1);
    }

    Identifier* ident = (Identifier*)(es->expression);
    if(ident->nodeType != NC_IDENTIFIER) {
        printfError("exp not Identifier, got=%s", ident->nodeType);
        exit(1);
    }

    if(strcmp(ident->value, "foobar")) {
        printfError("ident->TokenLiteral not foobar. got=%s", TokenLiteral(ident));
        exit(1);
    }

    if(strcmp(TokenLiteral(ident), "foobar")) {
        printfError("ident->TokenLiteral not foobar. got=%s", TokenLiteral(ident));
        exit(1);
    }

    printf("test ok\n");
}

void TestIntegerLiteralExpression() {
    char* input = "5;";

    Lexer* l = newLexer(input);
    Parser* p = newParser(l);

    Program* program = parseProgram(p);
    checkParserErrors(p);

    if(program->len != 1) {
        printfError("program has not enough statements. got=%d",
                program->len);
        exit(1);
    }

    ExpressionStatement* stmt = (ExpressionStatement*)(program->statements[0]);
    if(stmt->nodeType != NC_EXPRESSION_STATEMENT) {
        printfError("program.Statements[0] is not ExpressionStatement. got=%s",
                getNodeTypeString(program->statements[0]->nodeType));
        exit(1);
    }

    IntegerLiteral* literal = (IntegerLiteral*)(stmt->expression);
    if(literal->nodeType != NC_INTEGER_LITERAL) {
        printfError("exp not IntegerLiteral. got=%s", 
                getNodeTypeString(literal->nodeType));
        exit(1);
    }

    if(literal->value != 5) {
        printfError("literal.value not %d. got=%d", 5, literal->value);
        exit(1);
    }

    if(strcmp(TokenLiteral(literal), "5")) {
        printf("literal.TokenLiteral not %s. got=%s", "5",
                TokenLiteral(literal));
        exit(1);
    }

    printf("test ok\n");
}


void TestParsingPrefixExpressions() {
    char* input[4] = { "!5;", "-15;", "!true", "!false" };
    char* operator[4] = { "!", "-", "!", "!" };
    int value[4] = { 5, 15, 1, 0};

    for(int i = 0; i < 2; i++) {
        Lexer* l = newLexer(input[i]);
        Parser* p = newParser(l);
        
        Program* program = parseProgram(p);
        checkParserErrors(p);

        if(program->len != 1) {
            printfError("program.Statements does not contain %d statements. got=%d\n",
                    1, program->len);
        }

        ExpressionStatement* stmt = (ExpressionStatement*)(program->statements[0]);
        if(stmt->nodeType != NC_EXPRESSION_STATEMENT) {
            printfError("stmt is not ast.PrefixExpression. got=%s",
                    ToString((Node*)stmt));
        }

        PrefixExpression* exp = (PrefixExpression*)(stmt->expression);
        if(exp->nodeType != NC_PREFIX_EXPRESSION) {
            printfError("stmt is not ast.PrefixExpression. got=%d",
                    exp->nodeType);
        }

        if(strcmp(exp->op, operator[i])) {
            printfError("exp.op is not '%s'. got=%s",
                    operator[i], exp->op);
        }

        if(!testIntegerLiteral(exp->right, value[i])) {
            return;
        }
    }

    printf("test ok\n");
}

void TestParsingInfixExpressions() {
    char* input[11] = {
        "5 + 5;",
        "5 - 5;",
        "5 * 5;",
        "5 / 5;",
        "5 > 5;",
        "5 < 5;",
        "5 == 5;",
        "5 != 5;",
        "true == true",
        "true != false",
        "false == false",
    };
    TestParam leftValue[11];
    leftValue[0] = (TestParam) { .paramType=PT_INT, .integer=5 };
    leftValue[1] = (TestParam) { .paramType=PT_INT, .integer=5 };
    leftValue[2] = (TestParam) { .paramType=PT_INT, .integer=5 };
    leftValue[3] = (TestParam) { .paramType=PT_INT, .integer=5 };
    leftValue[4] = (TestParam) { .paramType=PT_INT, .integer=5 };
    leftValue[5] = (TestParam) { .paramType=PT_INT, .integer=5 };
    leftValue[6] = (TestParam) { .paramType=PT_INT, .integer=5 };
    leftValue[7] = (TestParam) { .paramType=PT_INT, .integer=5 };
    leftValue[8] = (TestParam) { .paramType=PT_BOOL, .boolean=1 };
    leftValue[9] = (TestParam) { .paramType=PT_BOOL, .boolean=1 };
    leftValue[10] = (TestParam) { .paramType=PT_BOOL, .boolean=0 };
    char* operator[11] = {
        "+",
        "-",
        "*",
        "/",
        ">",
        "<",
        "==",
        "!=",
        "==",
        "!=",
        "==",
    };
    TestParam rightValue[11];
    rightValue[0] = (TestParam) { .paramType=PT_INT, .integer=5 };
    rightValue[1] = (TestParam) { .paramType=PT_INT, .integer=5 };
    rightValue[2] = (TestParam) { .paramType=PT_INT, .integer=5 };
    rightValue[3] = (TestParam) { .paramType=PT_INT, .integer=5 };
    rightValue[4] = (TestParam) { .paramType=PT_INT, .integer=5 };
    rightValue[5] = (TestParam) { .paramType=PT_INT, .integer=5 };
    rightValue[6] = (TestParam) { .paramType=PT_INT, .integer=5 };
    rightValue[7] = (TestParam) { .paramType=PT_INT, .integer=5 };
    rightValue[8] = (TestParam) { .paramType=PT_BOOL, .boolean=1 };
    rightValue[9] = (TestParam) { .paramType=PT_BOOL, .boolean=0 };
    rightValue[10] = (TestParam) { .paramType=PT_BOOL, .boolean=0 };

    for(int i = 0; i < 11; i++) {
        Lexer* l = newLexer(input[i]);
        Parser* p = newParser(l);
        Program* program = parseProgram(p);
        checkParserErrors(p);

        if(program->len != 1) {
            printfError("program.statements does not contain %d statments. got=%d\n",
                    1, program->len);
            exit(1);
        }

        ExpressionStatement* stmt = (ExpressionStatement*)program->statements[0];
        if(stmt->nodeType != NC_EXPRESSION_STATEMENT) {
            printfError("program.statements[0] is not ExpressionStatement. got=%s",
                    getNodeTypeString(stmt->nodeType));
            exit(1);
        }

        if(!testInfixExpression(stmt->expression, 
                    &leftValue[i], operator[i], &rightValue[i])) {
        }

    }

    printf("test ok\n");
}

void TestOperatorPrecedenceParsing() {
    char* input[16] = {
        "-a * b",
        "!-a",
        "a + b + c",
        "a + b - c",
        "a * b * c",
        "a * b / c",
        "a + b / c",
        "a + b * c + d / e - f",
        "3 + 4; -5 * 5",
        "5 > 4 == 3 < 4",
        "5 < 4 != 3 > 4",
        "3 + 4 * 5 == 3 * 1 + 4 * 5",
        "true",
        "false",
        "3 > 5 == false",
        "3 < 5 == ture",
    };
    char* result[16] = {
        "((-a) * b)",
        "(!(-a))",
        "((a + b) + c)",
        "((a + b) - c)",
        "((a * b) * c)",
        "((a * b) / c)",
        "(a + (b / c))",
        "(((a + (b * c)) + (d / e)) - f)",
        "(3 + 4)((-5) * 5)",
        "((5 > 4) == (3 < 4))",
        "((5 < 4) != (3 > 4))",
        "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))",
        "true",
        "false",
        "((3 > 5) == false)",
        "((3 < 5) == ture)",
    };

    for(int i = 0; i < 11; i++) {
        Lexer* l = newLexer(input[i]);
        Parser* p = newParser(l);
        Program* program = parseProgram(p);

        char* programString = ToString(program);
        if(strcmp(programString, result[i])) {
            fprintf(stderr, "expected=%s got=%s\n", result[i], programString);
            abort();
        }

        free(programString);
    }

    printf("test ok\n");
}


void Init() {
    InitToken();
    InitAST();
    InitParser();
}

int main() {
    Init();
    //TestNextToken();
    //TestLetStatements();
    //TestReturnStatements();
    //TestString();
    //TestIdentifierExpression();
    //TestIntegerLiteralExpression();
    TestParsingPrefixExpressions();
    TestParsingInfixExpressions();
    TestOperatorPrecedenceParsing();
    return 0;
}
