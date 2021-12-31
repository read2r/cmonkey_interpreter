#ifndef __AST_H__
#define __AST_H__

#include "token.h"

#define STATEMENT_LEN 100

typedef enum _NodeCode {
    NC_NODE,
    NC_STATEMENT,
    NC_EXPRESSION,
    NC_PROGRAM,
    NC_LET_STATEMENT,
    NC_IDENTIFIER,
    NC_RETURN_STATEMENT,
    NC_EXPRESSION_STATEMENT,
    NC_INTEGER_LITERAL,
} NodeType;

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

// Expression
typedef struct _IntegerLiteral {
    NodeType nodeType;
    Token* token;
    int value;
} IntegerLiteral;

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
    NodeType nodeType;
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
ExpressionStatement* newExpressionStatement();
IntegerLiteral* newIntegerLiteral();

fptrTokenLiteral TokenLiteralList[100];
fptrToString ToStringList[100];
char* NodeTypeString[100];

void InitToStringList();
char* ToString(Node* node);

void InitTokenLiteralList();
char* TokenLiteral(Node* node);
void appendStatement(Program* program, Statement* stmt);

char* getNodeTypeString(NodeType);

#endif
