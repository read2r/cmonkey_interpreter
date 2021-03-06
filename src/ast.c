#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "buffer.h"

char* ToString_Boolean(Node* node) {
    Boolean* boolean = (Boolean*)node;
    return boolean->token->literal;
}

char* ToString_IntegerLiteral(Node* node) {
    Buffer* buf = newBuffer();
    Buffer_writeString(buf, TokenLiteral(node));
    return Buffer_toString(buf);
}

char* ToString_ExpressionStatement(Node* node) {
    ExpressionStatement* es = (ExpressionStatement*)node;

    if(es->expression != NULL) {
        return ToString((Node*)es->expression);
    }

    return " ";
}

char* ToString_ReturnStatement(Node* node) {
    ReturnStatement* rs = (ReturnStatement*)node;
    Buffer* buf = newBuffer();

    Buffer_writeString(buf, TokenLiteral(rs));
    Buffer_writeString(buf, " ");

    if(rs->returnValue != NULL) {
        Buffer_writeString(buf, ToString(rs->returnValue));
    }

    Buffer_writeString(buf, ";");

    return Buffer_toString(buf);
}

char* ToString_Identifier(Node* node) {
    Identifier* i = (Identifier*)node;
    char* temp = (char*)malloc(sizeof(char) * (strlen(i->value) + 1));
    strcpy(temp, i->value);
    return temp;
}

char* ToString_LetStatement(Node* node) {
    LetStatement* ls = (LetStatement*)node;
    Buffer* buf = newBuffer();

    Buffer_writeString(buf, TokenLiteral(node));
    Buffer_writeString(buf, " ");
    Buffer_writeString(buf, ToString(ls->name));
    Buffer_writeString(buf, " = ");

    if(ls->value != NULL) {
        Buffer_writeString(buf, ToString(ls->value));
    }

    Buffer_writeString(buf, ";");

    return Buffer_toString(buf);
}

char* ToString_PrefixExpression(Node* node) {
    PrefixExpression* pe = (PrefixExpression*)node;
    Buffer* buf = newBuffer();

    Buffer_writeString(buf, "(");
    Buffer_writeString(buf, pe->op);
    Buffer_writeString(buf, ToString(pe->right));
    Buffer_writeString(buf, ")");

    return Buffer_toString(buf);
}

char* ToString_InfixExpression(Node* node) {
    InfixExpression* ie = (InfixExpression*)node;
    Buffer* buf = newBuffer();

    Buffer_writeString(buf, "(");
    Buffer_writeString(buf, ToString(ie->left));
    Buffer_writeString(buf, " ");
    Buffer_writeString(buf, ie->op);
    Buffer_writeString(buf, " ");
    Buffer_writeString(buf, ToString(ie->right));
    Buffer_writeString(buf, ")");

    return Buffer_toString(buf);
}

char* ToString_Program(Node* node) {
    Program* p = (Program*)node;
    Buffer* buf = newBuffer();

    for(int i = 0; i < p->len; i++) {
        Buffer_writeString(buf, ToString(p->statements[i]));
    }

    return Buffer_toString(buf);
}

char* TokenLiteral_Boolean(Node* node) {
    Boolean* boolean = (Boolean*)node;
    return boolean->token->literal;
}

char* TokenLiteral_IntergerLiteral(Node* node) {
    IntegerLiteral* il = (IntegerLiteral*)node;
    return il->token->literal;
}

char* TokenLiteral_ExpressionStatement(Node* node) {
    ExpressionStatement* es = (ExpressionStatement*)node;
    return es->token->literal;
}

char* TokenLiteral_ReturnStatement(Node* node) {
    ReturnStatement* rs = (ReturnStatement*)node;
    return rs->token->literal;
}

char* TokenLiteral_Identifier(Node* node) {
    Identifier* i = (Identifier*)node;
    return i->token->literal;
}

char* TokenLiteral_LetStatement(Node* node) {
    LetStatement* ls = (LetStatement*)node;
    return ls->token->literal;
} 

char* TokenLiteral_PrefixExpression(Node* node) {
    PrefixExpression* pe = (PrefixExpression*)node;
    return pe->token->literal;
}

char* TokenLiteral_InfixExpression(Node* node) {
    InfixExpression* pe = (InfixExpression*)node;
    return pe->token->literal;
}

char* TokenLiteral_Program(Node* node) {
    Program* p = (Program*)node;

    if(p->len > 0) {
        return TokenLiteral((Node*)p->statements[0]);
    } else {
        return " ";
    }
}

Program* newProgram() {
    Program* program = (Program*)malloc(sizeof(Program));
    program->nodeType = NC_PROGRAM; 
    program->len = 0;
    return program;
}

LetStatement* newLetStatement() {
    LetStatement* lstmt = (LetStatement*)malloc(sizeof(LetStatement));
    lstmt->nodeType = NC_LET_STATEMENT;
    lstmt->token = NULL;
    lstmt->name = NULL;
    lstmt->value = NULL;
    return lstmt;
}

Identifier* newIdentifier() {
    Identifier* ident = (Identifier*)malloc(sizeof(Identifier));
    ident->nodeType = NC_IDENTIFIER;
    ident->token = NULL;
    ident->value = NULL;
    return ident;
}

ReturnStatement* newReturnStatement() {
    ReturnStatement* rstmt = (ReturnStatement*)malloc(sizeof(ReturnStatement));
    rstmt->nodeType = NC_RETURN_STATEMENT;
    rstmt->token = NULL;
    rstmt->returnValue = NULL;
    return rstmt;
}

ExpressionStatement* newExpressionStatement() {
    ExpressionStatement* estmt = (ExpressionStatement*)malloc(sizeof(ExpressionStatement));
    estmt->nodeType = NC_EXPRESSION_STATEMENT;
    estmt->token = NULL;
    estmt->expression = NULL;
    return estmt;
}

IntegerLiteral* newIntegerLiteral() {
    IntegerLiteral* il = (IntegerLiteral*)malloc(sizeof(IntegerLiteral));
    il->nodeType = NC_INTEGER_LITERAL;
    il->token = NULL;
    return il;
}

PrefixExpression* newPrefixExpression() {
    PrefixExpression* pe = (PrefixExpression*)malloc(sizeof(PrefixExpression));
    pe->nodeType = NC_PREFIX_EXPRESSION;
    pe->op = NULL;
    pe->right = NULL;
    return pe;
}

InfixExpression* newInfixExpression() {
    InfixExpression* ie = (InfixExpression*)malloc(sizeof(InfixExpression));
    ie->nodeType = NC_INFIX_EXPRESSION;
    ie->left = NULL;
    ie->op = NULL;
    ie->right = NULL;
    return ie;
}

Boolean* newBoolean() {
    Boolean* boolean = (Boolean*)malloc(sizeof(Boolean));
    boolean->nodeType = NC_BOOLEAN;
    boolean->token = NULL;
    return boolean;
}

char* getNodeTypeString(NodeType nodeType) {
    return NodeTypeString[nodeType];
}

void InitTokenLiteralList() {
    for(int i = 0; i < 100; i++)
        TokenLiteralList[i] = NULL;
    TokenLiteralList[NC_PROGRAM] = TokenLiteral_Program; 
    TokenLiteralList[NC_LET_STATEMENT] = TokenLiteral_LetStatement;
    TokenLiteralList[NC_IDENTIFIER] = TokenLiteral_Identifier;
    TokenLiteralList[NC_RETURN_STATEMENT] = TokenLiteral_ReturnStatement;
    TokenLiteralList[NC_EXPRESSION_STATEMENT] = TokenLiteral_ExpressionStatement;
    TokenLiteralList[NC_INTEGER_LITERAL] = TokenLiteral_IntergerLiteral;
    TokenLiteralList[NC_PREFIX_EXPRESSION] = TokenLiteral_PrefixExpression;
    TokenLiteralList[NC_INFIX_EXPRESSION] = TokenLiteral_InfixExpression;
    TokenLiteralList[NC_BOOLEAN] = TokenLiteral_Boolean;
}

void InitNodetypeString() {
    for(int i = 0; i < 100; i++)
        NodeTypeString[i] = NULL;
    NodeTypeString[NC_PROGRAM] = "Program";
    NodeTypeString[NC_LET_STATEMENT] = "LetStatement";
    NodeTypeString[NC_IDENTIFIER] = "Identifier";
    NodeTypeString[NC_RETURN_STATEMENT] = "ReturnStatement";
    NodeTypeString[NC_EXPRESSION_STATEMENT] = "ExpressionStatement";
    NodeTypeString[NC_INTEGER_LITERAL] = "IntegerLiteral";
    NodeTypeString[NC_PREFIX_EXPRESSION] = "PrefixExpression";
    NodeTypeString[NC_INFIX_EXPRESSION] = "InfixExpression";
    NodeTypeString[NC_BOOLEAN] = "Boolean";
}

void InitToStringList() {
    for(int i = 0; i < 100; i++)
        ToStringList[i] = NULL;
    ToStringList[NC_PROGRAM] = ToString_Program;
    ToStringList[NC_LET_STATEMENT] = ToString_LetStatement;
    ToStringList[NC_IDENTIFIER] = ToString_Identifier;
    ToStringList[NC_RETURN_STATEMENT] = ToString_ReturnStatement;
    ToStringList[NC_EXPRESSION_STATEMENT] = ToString_ExpressionStatement;
    ToStringList[NC_INTEGER_LITERAL] = ToString_IntegerLiteral;
    ToStringList[NC_PREFIX_EXPRESSION] = ToString_PrefixExpression;
    ToStringList[NC_INFIX_EXPRESSION] = ToString_InfixExpression;
    ToStringList[NC_BOOLEAN] = ToString_Boolean;
}

void InitAST() {
    InitTokenLiteralList();
    InitNodetypeString();
    InitToStringList();
}

char* _TokenLiteral(Node* node) {
    NodeType nt = node->nodeType;
    return TokenLiteralList[nt](node);
}

char* _ToString(Node* node) {
    NodeType nt = node->nodeType;
    return ToStringList[nt](node);
}

void appendStatement(Program* program, Statement* stmt) {
    program->statements[program->len] = stmt;
    program->len++;
};
