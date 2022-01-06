#include <stdio.h>
#include <stdlib.h>

#include "merr.h"
#include "parser.h"
#include "ast.h"
#include "token.h"

int precedences[PRECEDENCE_LEN];

int peekPrecedence(Parser* p) {
    if(precedences[p->peekToken->tokenType] != -1) {
        return precedences[p->peekToken->tokenType];
    }
    return PR_LOWEST;
}

int curPrecedence(Parser* p) {
    if(precedences[p->curToken->tokenType] != -1) {
        return precedences[p->curToken->tokenType];
    }
    return PR_LOWEST;
}

void registerPrefix(Parser* p, TokenType tokenType, prefixParseFn fn) {
    p->prefixParseFns[tokenType] = fn;
}

void registerInfix(Parser* p, TokenType tokenType, infixParseFn fn) {
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

    while(!peekTokenIs(p, TOKEN_SEMICOLON) && precedence < peekPrecedence(p)) {
        infixParseFn infix = p->infixParseFns[p->peekToken->tokenType];
        
        if(infix == NULL) {
            return leftExp;
        }

        parseNextToken(p);

        leftExp = infix(p, leftExp);
    }

    return leftExp;
} 

Expression* parseBoolean(Parser* p) {
    Boolean* boolean = newBoolean();
    boolean->token = p->curToken;
    boolean->value = curTokenIs(p, TOKEN_TRUE);
    return (Expression*)boolean;
}

Expression* parseInfixExpression(Parser* p, Expression* left) {
    InfixExpression* ie = newInfixExpression();
    ie->token = p->curToken;
    ie->op = p->curToken->literal;
    ie->left = left;

    int precedence = curPrecedence(p);
    parseNextToken(p);
    ie->right = parseExpression(p, precedence);

    return (Expression*)ie;
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

    for(int i = 0; i < PREFIX_PARSE_FN_LEN; i++) {
        p->prefixParseFns[i] = NULL;
        p->infixParseFns[i] = NULL;
    }

    registerPrefix(p, TOKEN_IDENT, (prefixParseFn)parseIdentifier);
    registerPrefix(p, TOKEN_INT, (prefixParseFn)parseIntegerLiteral);
    registerPrefix(p, TOKEN_BANG, (prefixParseFn)parsePrefixExpression);
    registerPrefix(p, TOKEN_MINUS, (prefixParseFn)parsePrefixExpression);
    registerPrefix(p, TOKEN_TRUE, (prefixParseFn)parseBoolean);
    registerPrefix(p, TOKEN_FALSE, (prefixParseFn)parseBoolean);

    registerInfix(p, TOKEN_PLUS, (infixParseFn)parseInfixExpression);
    registerInfix(p, TOKEN_MINUS, (infixParseFn)parseInfixExpression);
    registerInfix(p, TOKEN_SLASH, (infixParseFn)parseInfixExpression);
    registerInfix(p, TOKEN_ASTERISK, (infixParseFn)parseInfixExpression);
    registerInfix(p, TOKEN_EQ, (infixParseFn)parseInfixExpression);
    registerInfix(p, TOKEN_NOT_EQ, (infixParseFn)parseInfixExpression);
    registerInfix(p, TOKEN_LT, (infixParseFn)parseInfixExpression);
    registerInfix(p, TOKEN_GT, (infixParseFn)parseInfixExpression);

    return p;
}

void InitPrecedences() {
    precedences[TOKEN_EQ] = PR_EQUALS;
    precedences[TOKEN_NOT_EQ] = PR_EQUALS;
    precedences[TOKEN_LT] = PR_LESSGREATER;
    precedences[TOKEN_GT] = PR_LESSGREATER;
    precedences[TOKEN_PLUS] = PR_SUM;
    precedences[TOKEN_MINUS] = PR_SUM;
    precedences[TOKEN_SLASH] = PR_PRODUCT;
    precedences[TOKEN_ASTERISK] = PR_PRODUCT;
}

void InitParser() {
    InitPrecedences();
}
