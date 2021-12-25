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
    CODE_LETSTATEMENT,
    CODE_IDENTIFIER,
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

// Statement;
typedef struct _LetStatement {
    NodeType nodeType;
    Token* token; // LET Token;
    Identifier* name;
    Expression* value;
} LetStatement;

Program* newProgram();
Identifier* newIdentifier();
LetStatement* newLetStatement();

char* TokenLiteral(Node* node);
void appendStatement(Program* program, Statement* stmt);

#endif
