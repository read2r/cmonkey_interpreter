#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

unsigned int getStringLen(Buffer* buf) {
    unsigned int stringLen = 0;
    for(int i = 0; i < buf->len; i++) {
        stringLen += strlen(buf->arr[i]);
    }
    return stringLen;
}

char* ToString_Buffer(Buffer* buf) {
    unsigned int stringLen = getStringLen(buf);
    char* string = (char*)malloc(sizeof(char) * (stringLen + 1));
    memset(string, 0, sizeof(char) * (stringLen + 1));
    for(int i = 0; i < buf->len; i++) {
        strcat(string, buf->arr[i]);
    }
    return string;
}

Buffer* newBuffer() {
    Buffer* buf = (Buffer*)malloc(sizeof(Buffer));
    buf->len = 0;
    return buf;
}

void freeBuffer(Buffer* buf) {
    for(int i = 0; i < buf->len; i++) {
        free(buf->arr[i]);
        buf->arr[i] = NULL;
    }
    free(buf);
    buf = NULL;
}

void writeString(Buffer* buf, const char* str) {
    char* temp = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(temp, str);
    buf->arr[buf->len] = temp;
    buf->len++;
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

    writeString(buf, TokenLiteral((Node*)rs));
    writeString(buf, " ");

    if(rs->returnValue != NULL) {
        writeString(buf, ToString((Node*)rs->returnValue));
    }

    writeString(buf, ";");

    return ToString_Buffer(buf);
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

    writeString(buf, TokenLiteral(node));
    writeString(buf, " ");
    writeString(buf, ToString((Node*)ls->name));
    writeString(buf, " = ");

    if(ls->value != NULL) {
        writeString(buf, ToString((Node*)ls->value));
    }

    writeString(buf, ";");

    return ToString_Buffer(buf);
}

char* ToString_Program(Node* node) {
    Program* p = (Program*)node;
    Buffer* buf = newBuffer();

    for(int i = 0; i < p->len; i++) {
        writeString(buf, ToString((Node*)p->statements[i]));
    }

    return ToString_Buffer(buf);
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
    program->nodeType = CODE_PROGRAM; 
    program->len = 0;
    return program;
}

LetStatement* newLetStatement() {
    LetStatement* lstmt = (LetStatement*)malloc(sizeof(LetStatement));
    lstmt->nodeType = CODE_LET_STATEMENT;
    lstmt->token = NULL;
    lstmt->name = NULL;
    lstmt->value = NULL;
    return lstmt;
}

Identifier* newIdentifier() {
    Identifier* ident = (Identifier*)malloc(sizeof(Identifier));
    ident->nodeType = CODE_IDENTIFIER;
    ident->token = NULL;
    ident->value = NULL;
    return ident;
}

ReturnStatement* newReturnStatement() {
    ReturnStatement* rstmt = (ReturnStatement*)malloc(sizeof(ReturnStatement));
    rstmt->nodeType = CODE_RETURN_STATEMENT;
    rstmt->token = NULL;
    rstmt->returnValue = NULL;
    return rstmt;
}

void InitTokenLiteralList() {
    for(int i = 0; i < 0; i++) {
        TokenLiteralList[i] = NULL;
        NodeTypeString[i] = NULL;
        ToStringList[i] = NULL;
    }

    TokenLiteralList[CODE_PROGRAM] = TokenLiteral_Program; 
    TokenLiteralList[CODE_LET_STATEMENT] = TokenLiteral_LetStatement;
    TokenLiteralList[CODE_IDENTIFIER] = TokenLiteral_Identifier;
    TokenLiteralList[CODE_RETURN_STATEMENT] = TokenLiteral_ReturnStatement;
    TokenLiteralList[CODE_EXPRESSION] = TokenLiteral_ExpressionStatement;

    NodeTypeString[CODE_PROGRAM] = "Program";
    NodeTypeString[CODE_LET_STATEMENT] = "LetStatement";
    NodeTypeString[CODE_IDENTIFIER] = "Identifier";
    NodeTypeString[CODE_RETURN_STATEMENT] = "ReturnStatement";

    ToStringList[CODE_PROGRAM] = ToString_Program;
    ToStringList[CODE_LET_STATEMENT] = ToString_LetStatement;
    ToStringList[CODE_IDENTIFIER] = ToString_Identifier;
    ToStringList[CODE_RETURN_STATEMENT] = ToString_ReturnStatement;
    ToStringList[CODE_EXPRESSION] = ToString_ExpressionStatement;
}

char* TokenLiteral(Node* node) {
    NodeType nt = node->nodeType;
    if(TokenLiteralList[nt] != NULL) {
        return TokenLiteralList[nt](node);
    }
    return NULL;
}

char* ToString(Node* node) {
    NodeType nt = node->nodeType;
    return ToStringList[nt](node);
}

void appendStatement(Program* program, Statement* stmt) {
    program->statements[program->len] = stmt;
    program->len++;
};
