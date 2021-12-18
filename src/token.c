#include <stdlib.h>
#include <string.h>
#include "token.h"

void InitializeToken() {
    for(int i = 0; i < 14; i++) {
        TOKENTYPES[i] = (TokenType)malloc(BASIC_STR_SIZE);
    }

    // ILLEGAL
    memcpy(TOKENTYPES[TAG_ILLEGAL], "ILLEGAL", BASIC_STR_SIZE);
    // EOF
    memcpy(TOKENTYPES[TAG_EOF], "EOF", BASIC_STR_SIZE);
    // IDENT
    memcpy(TOKENTYPES[TAG_IDENT], "IDENT", BASIC_STR_SIZE);
    // INT
    memcpy(TOKENTYPES[TAG_INT], "INT", BASIC_STR_SIZE);
    // ASSIGN
    memcpy(TOKENTYPES[TAG_ASSIGN], "=", BASIC_STR_SIZE);
    // PLUS
    memcpy(TOKENTYPES[TAG_PLUS], "+", BASIC_STR_SIZE);
    // COMMA
    memcpy(TOKENTYPES[TAG_COMMA], ",", BASIC_STR_SIZE);
    // SEMICOLON
    memcpy(TOKENTYPES[TAG_SEMICOLON], ";", BASIC_STR_SIZE);
    // LPAREN
    memcpy(TOKENTYPES[TAG_LPAREN], "(", BASIC_STR_SIZE);
    // RPAREN
    memcpy(TOKENTYPES[TAG_RPAREN], ")", BASIC_STR_SIZE);
    // LBRACE
    memcpy(TOKENTYPES[TAG_LBRACE], "{", BASIC_STR_SIZE);
    // RBRACE
    memcpy(TOKENTYPES[TAG_RBRACE], "}", BASIC_STR_SIZE);
    // FUNCTION
    memcpy(TOKENTYPES[TAG_FUNCTION], "FUNCTION", BASIC_STR_SIZE);
    // LET
    memcpy(TOKENTYPES[TAG_LET], "LET", BASIC_STR_SIZE);
}
