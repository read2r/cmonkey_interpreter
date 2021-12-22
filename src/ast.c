#include <stdlib.h>
#include <string.h>
#include "ast.h"

char* TokenLiteralWithIdentifier(Identifier* e) {
    return e->token->literal;
}

char* TokenLiteralWithLetStatement(LetStatement* ls) {
    return ls->token->literal;
}

char* TokenLiteralWithProgram(Program* p) {
    if(p->len > 0) {
        return TokenLiteral((Node*)(p->statements[0]));
    } else {
        return " ";
    }
}

char* TokenLiteral(Node* node) {
    NodeType nt = node->nodeType;

    if(nt == CODE_PROGRAM) {
        TokenLiteralWithProgram((Program*)node);
    } else if(nt == CODE_LETSTATEMENT) {
        TokenLiteralWithLetStatement((LetStatement*)node);
    } else if(nt == CODE_IDENTIFIER) {
        TokenLiteralWithIdentifier((Identifier*)node);
    }

    return NULL;
}
