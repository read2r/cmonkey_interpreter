#include <stdlib.h>
#include "parser.h"
#include "ast.h"

Parser* newParser(Lexer* l) {
    Parser* p = (Parser*)malloc(sizeof(Parser));
    p->l = l;
    p->curToken = NULL;
    p->peekToken= NULL;

    parseNextToken(p);
    parseNextToken(p);

    return p;
}

void parseNextToken(Parser* p) {
    p->curToken = p->peekToken;
    p->peekToken = nextToken(p->l);
}

Program* ParseProgram(Parser* p) {
    return NULL;
}
