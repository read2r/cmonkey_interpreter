#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

char* TokenLiteralInIdentifier(Identifier* i) {
    return i->token->literal;
}

char* TokenLiteralInLetStatement(LetStatement* ls) {
    return ls->token->literal;
} 

char* TokenLiteralInProgram(Program* p) {
    if(p->len > 0) {
        return TokenLiteral((Node*)(p->statements[0]));
    } else {
        return " ";
    }
}

Program* newProgram() {
    Program* program = (Program*)malloc(sizeof(Program));
    program->nodeType = CODE_PROGRAM; program->len = 0;
    return program;
}

LetStatement* newLetStatement() {
    LetStatement* lstmt = (LetStatement*)malloc(sizeof(LetStatement));
    lstmt->nodeType = CODE_LETSTATEMENT;
    return lstmt;
}

Identifier* newIdentifier() {
    Identifier* ident = (Identifier*)malloc(sizeof(Identifier));
    ident->nodeType = CODE_IDENTIFIER;
    return ident;
}

char* TokenLiteral(Node* node) {
    NodeType nt = node->nodeType;

    if(nt == CODE_PROGRAM) {
        return TokenLiteralInProgram((Program*)node);
    } else if(nt == CODE_LETSTATEMENT) {
        return TokenLiteralInLetStatement((LetStatement*)node);
    } else if(nt == CODE_IDENTIFIER) {
        return TokenLiteralInIdentifier((Identifier*)node);
    }

    return NULL;
}

void appendStatement(Program* program, Statement* stmt) {
    program->statements[program->len] = stmt;
    program->len++;
}
