#ifndef __TOKEN_H__
#define __TOKEN_H__
#define BASIC_STR_SIZE 128 

typedef char* TokenType;

typedef struct _Token {
    TokenType type;
    char* literal;
} Token;

enum {
    TAG_ILLEGAL = 0,
    TAG_EOF,

    TAG_IDENT,
    TAG_INT,

    TAG_ASSIGN,
    TAG_PLUS,

    TAG_COMMA,
    TAG_SEMICOLON,

    TAG_LPAREN,
    TAG_RPAREN,
    TAG_LBRACE,
    TAG_RBRACE,

    TAG_FUNCTION,
    TAG_LET,
};

TokenType TOKENTYPES[100];
void InitializeToken();

#endif
