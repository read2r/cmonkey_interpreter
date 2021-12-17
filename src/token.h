#ifndef __TOKEN_H__
#define __TOKEN_H__

typedef int TokenType;

typedef struct _Token {
    TokenType type;
    char* literal;
} Token;

enum {
    TOKEN_ILLEGA = 0,
    TOKEN_EOF,

    TOKEN_IDENT,
    TOKEN_INT,

    TOKEN_ASSIGN,
    TOKEN_PLUS,

    TOKEN_COMMA,
    TOKEN_SEMICOLON,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    TOKEN_FUNCTION,
    TOKEN_LET,
};

#endif
