#include <stdlib.h>
#include "merr.h"
#include "parser.h"
#include "ast.h"
#include "token.h"

Parser* newParser(Lexer* l) {
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->l = l;
    p->curToken = NULL;
    p->peekToken= NULL;
    p->errors = newErrors();

    parseNextToken(p);
    parseNextToken(p);

    return p;
}

void parseNextToken(Parser* p) {
    p->curToken = p->peekToken;
    p->peekToken = nextToken(p->l);
}

int curTokenIs(Parser* p, TokenType tokenType) {
    return p->curToken->type == tokenType;
}

int peekTokenIs(Parser* p, TokenType tokenType) {
    return p->peekToken->type == tokenType;
}

int expectPeek(Parser* p, TokenType tokenType) {
    if(peekTokenIs(p, tokenType)) {
        parseNextToken(p);
        return 1;
    } else {
        peekError(p->errors, p->peekToken, tokenType);
        return 0;
    }
}

LetStatement* parseLetStatement(Parser* p) {
    LetStatement* lstmt = newLetStatement();
    lstmt->token = p->curToken;

    if(!expectPeek(p, TOKENTYPES[CODE_IDENT])) {
        return NULL;
    }

    Identifier* ident = newIdentifier();
    ident->token = p->curToken;
    ident->value = p->curToken->literal;
    lstmt->name = ident;

    while(!curTokenIs(p, TOKENTYPES[CODE_SEMICOLON])) {
        parseNextToken(p);
    }

    return lstmt;
}

Statement* parseStatement(Parser* p) {
    Statement* stmt = NULL;

    if(p->curToken->type == TOKENTYPES[CODE_LET]) {
        stmt = (Statement*)parseLetStatement(p);
    }

    return stmt;
}

Program* parseProgram(Parser* p) {
    Program* program = newProgram();
    Statement* stmt;

    while(p->curToken->type != TOKENTYPES[CODE_EOF]) {
        stmt = parseStatement(p);
        if(stmt != NULL) {
            appendStatement(program, stmt);
        }
        parseNextToken(p);
    }

    return program;
}
