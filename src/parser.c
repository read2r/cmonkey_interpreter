#include <stdlib.h>
#include "merr.h"
#include "parser.h"
#include "ast.h"
#include "token.h"

Parser* newParser(Lexer* l) {
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->l = l;
    p->errors = newErrors();

    p->curToken = NULL;
    p->peekToken= NULL;

    for(int i = 0; i < 100; i++) {
        p->prefixParseFns[i] = NULL;
        p->infixParseFns[i] = NULL;
    }

    parseNextToken(p);
    parseNextToken(p);

    return p;
}

void parseNextToken(Parser* p) {
    p->curToken = p->peekToken;
    p->peekToken = nextToken(p->l);
}

int curTokenIs(Parser* p, TokenType tokenType) {
    return p->curToken->tokenType == tokenType;
}

int peekTokenIs(Parser* p, TokenType tokenType) {
    return p->peekToken->tokenType == tokenType;
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

    if(!expectPeek(p, TOKEN_IDENT)) {
        return NULL;
    }

    Identifier* ident = newIdentifier();
    ident->token = p->curToken;
    ident->value = p->curToken->literal;
    lstmt->name = ident;

    while(!curTokenIs(p, TOKEN_SEMICOLON)) {
        parseNextToken(p);
    }

    return lstmt;
}

ReturnStatement* parseReturnStatement(Parser* p) {
    ReturnStatement* rstmt = newReturnStatement();
    rstmt->nodeType = NC_RETURN_STATEMENT;
    rstmt->token = p->curToken;

    parseNextToken(p);

    while(!curTokenIs(p, TOKEN_SEMICOLON)) {
        parseNextToken(p);
    }

    return rstmt;
}

Expression* parseExpression(Parser* p, Precedence precedence) {
    Expression* e;
    return e;
}

ExpressionStatement* parseExpressionStatement(Parser* p) {
    ExpressionStatement* estmt = newExpressionStatement();
    estmt->token = p->curToken;
    estmt->expression = parseExpression(p, LOWEST);
    return estmt;
}

Statement* parseStatement(Parser* p) {
    Statement* stmt = NULL;

    if(p->curToken->tokenType == TOKEN_LET) {
        stmt = (Statement*)parseLetStatement(p);
    } else if(p->curToken->tokenType == TOKEN_RETURN) {
        stmt = (Statement*)parseReturnStatement(p);
    } else {
        stmt = (Statement*)parseExpressionStatement(p);
    }

    return stmt;
}

Program* parseProgram(Parser* p) {
    Program* program = newProgram();
    Statement* stmt;

    while(p->curToken->tokenType != TOKEN_EOF) {
        stmt = parseStatement(p);
        if(stmt != NULL) {
            appendStatement(program, stmt);
        }
        parseNextToken(p);
    }

    return program;
}

void registerPreifix(Parser* p, TokenType tokenType, prefixParseFn fn) {
    p->prefixParseFns[tokenType] = fn;
}

void registerInfix(Parser* p, TokenType tokenType, prefixParseFn fn) {
    p->infixParseFns[tokenType] = fn;
}
