#ifndef __TOKEN_H__
#define __TOKEN_H__

#define STR_SIZE 512
#define KEYWORDS_LEN 100

typedef enum _TokenType{
    TOKEN_ILLEGAL = 0,
    TOKEN_EOF,

    TOKEN_IDENT,
    TOKEN_INT,

    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_BANG,
    TOKEN_ASTERISK,
    TOKEN_SLASH,

    TOKEN_LT,
    TOKEN_GT,

    TOKEN_EQ,
    TOKEN_NOT_EQ,

    TOKEN_COMMA,
    TOKEN_SEMICOLON,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    TOKEN_FUNCTION,
    TOKEN_LET,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_RETURN,
} TokenType;

typedef struct _Token {
    TokenType tokenType;
    char *literal;
} Token;

typedef struct _Keyword {
    TokenType tokenType;
    char *literal;
} Keyword;

typedef struct _Keywords {
    int len;
    Keyword *arr[KEYWORDS_LEN];
} Keywords;

Keywords *KEYWORDS;
char *TokenTypeStringList[100];

char *getTokenTypeString();
void InitToken();
void InitializeTokenTypes();
void InitializeKeywords();
TokenType LookupIdent(char *ident);

#endif
