#ifndef __AST_H__
#define __AST_H__

#include "token.h"

#define TokenLiteral(x) _TokenLiteral((Node*)x)
#define ToString(x) _ToString((Node*)x)

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
    NC_PREFIX_EXPRESSION,
    NC_INFIX_EXPRESSION,
    NC_BOOLEAN,
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
    Statement* statements[100];
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

// Expression
typedef struct _PrefixExpression {
    NodeType nodeType;
    Token* token;
    char* op;
    Expression* right;
} PrefixExpression;

// Expression
typedef struct _InfixExpression {
    NodeType nodeType;
    Token* token;
    Expression* left;
    char* op;
    Expression* right;
} InfixExpression;

// Expression
typedef struct _Boolean {
    NodeType nodeType;
    Token* token;
    int value;
} Boolean;

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
    Token* token; // RETURN Token;
    Expression* returnValue;
} ReturnStatement;

// Statement
typedef struct _ExpressionStatement {
    NodeType nodeType;
    Token* token;
    Expression* expression;
} ExpressionStatement;

typedef char* (*fptrTokenLiteral)(Node* node);
typedef char* (*fptrToString)(Node* node);

Program* newProgram();
Identifier* newIdentifier();
LetStatement* newLetStatement();
ReturnStatement* newReturnStatement();
ExpressionStatement* newExpressionStatement();
IntegerLiteral* newIntegerLiteral();
PrefixExpression* newPrefixExpression();
InfixExpression* newInfixExpression();
Boolean* newBoolean();

fptrTokenLiteral TokenLiteralList[100];
fptrToString ToStringList[100];

char* NodeTypeString[100];

void InitAST();

void appendStatement(Program* program, Statement* stmt);

char* _TokenLiteral(Node* node);
char* getNodeTypeString(NodeType);
char* _ToString(Node* node);

#endif
