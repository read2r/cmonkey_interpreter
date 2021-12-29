#ifndef __AST_H__
#define __AST_H__

#include "token.h"

#define STATEMENT_LEN 100

typedef int NodeType;

enum {
    CODE_NODE,
    CODE_STATEMENT,
    CODE_EXPRESSION,
    CODE_PROGRAM,
    CODE_LET_STATEMENT,
    CODE_IDENTIFIER,
    CODE_RETURN_STATEMENT,
};

typedef struct _Node {
    NodeType nodeType;
} Node;

typedef struct _Statement {
    NodeType nodeType;
} Statement;

typedef struct _Expression {
    NodeType nodeType;
} Expression;

typedef struct _Program {
    NodeType nodeType;
    int len;
    Statement* statements[STATEMENT_LEN];
} Program;

// Expression
typedef struct _Identifier {
    NodeType nodeType;
    Token* token; // IDENT Token;
    char* value;
} Identifier;

// Statement
typedef struct _LetStatement {
    NodeType nodeType;
    Token* token; // LET Token;
    Identifier* name;
    Expression* value;
} LetStatement;

// Statement
typedef struct _ReturnStatement {
    NodeType nodeType;
    Token* token;
    Expression* returnValue;
} ReturnStatement;

// Statement
typedef struct _ExpressionStatement {
    Token* token;
    Expression* expression;
} ExpressionStatement;


typedef struct _Buffer {
    int len;
    char* arr[512];
} Buffer;

typedef char* (*fptrTokenLiteral)(Node* node);
typedef char* (*fptrToString)(Node* node);

Program* newProgram();
Identifier* newIdentifier();
LetStatement* newLetStatement();
ReturnStatement* newReturnStatement();

fptrTokenLiteral TokenLiteralList[100];
fptrToString ToStringList[100];
char* NodeTypeString[100];

void InitToStringList();
char* ToString(Node* node);

void InitTokenLiteralList();
char* TokenLiteral(Node* node);
void appendStatement(Program* program, Statement* stmt);

#endif
