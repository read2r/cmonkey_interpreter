#include <stdlib.h>
#include "merr.h"
#include "parser.h"
#include "ast.h"
#include "token.h"

void registerPrefix(Parser* p, TokenType tokenType, prefixParseFn fn) {
    p->prefixParseFns[tokenType] = fn;
}

void registerInfix(Parser* p, TokenType tokenType, prefixParseFn fn) {
    p->infixParseFns[tokenType] = fn;
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

Expression* parseIntegerLiteral(Parser* p) {
    IntegerLiteral* il = newIntegerLiteral();
    il->token = p->curToken;
    il->value = atoi(p->curToken->literal);
    return (Expression*)il;
}

Expression* parseIdentifier(Parser* p) {
    Identifier* ident = newIdentifier();
    ident->nodeType = NC_IDENTIFIER;
    ident->token = p->curToken;
    ident->value = p->curToken->literal;
    return (Expression*)ident;
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


void noPrefixParseFnError(Parser* p, TokenType t) {
    Error err = newError("no prefix parse function for %s found", getTokenTypeString(t));
    appendError(p->errors, err);
}

Expression* parseExpression(Parser* p, Precedence precedence) {
    prefixParseFn prefix = p->prefixParseFns[p->curToken->tokenType];

    if(prefix == NULL) {
        noPrefixParseFnError(p, p->curToken->tokenType);
        return NULL;
    }
    Expression* leftExp = prefix(p);

    return leftExp;
}

Expression* parsePrefixExpression(Parser* p) {
    PrefixExpression* pe = newPrefixExpression();
    pe->token = p->curToken;
    pe->op = p->curToken->literal;

    parseNextToken(p);
    
    pe->right = parseExpression(p, PR_PREFIX);

    return (Expression*)pe;
}

ExpressionStatement* parseExpressionStatement(Parser* p) {
    ExpressionStatement* estmt = newExpressionStatement();
    estmt->token = p->curToken;
    estmt->expression = parseExpression(p, PR_LOWEST);

    if(peekTokenIs(p, TOKEN_SEMICOLON)) {
        parseNextToken(p);
    }

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

Parser* newParser(Lexer* l) {
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->l = l;
    p->errors = newErrors();

    p->curToken = NULL;
    p->peekToken= NULL;

    parseNextToken(p);
    parseNextToken(p);

    for(int i = 0; i < 100; i++) {
        p->prefixParseFns[i] = NULL;
        p->infixParseFns[i] = NULL;
    }

    registerPrefix(p, TOKEN_IDENT, parseIdentifier);
    registerPrefix(p, TOKEN_INT, parseIntegerLiteral);
    registerPrefix(p, TOKEN_BANG, parsePrefixExpression);
    registerPrefix(p, TOKEN_MINUS, parsePrefixExpression);

    return p;
}
