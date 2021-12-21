#ifndef __TOKEN_H__
#define __TOKEN_H__

#define STR_SIZE 128 
#define TOKENTYPES_LEN 100
#define KEYWORDS_LEN 100

typedef char* TokenType;
typedef int TokenTypeCode;

typedef struct _Token {
    TokenType type;
    char* literal;
} Token;

typedef struct _Keyword {
    TokenTypeCode typeCode;
    char* literal;
} Keyword;

typedef struct _Keywords {
    int len;
    Keyword* arr[KEYWORDS_LEN];
} Keywords;

enum {
    CODE_ILLEGAL = 0,
    CODE_EOF,

    CODE_IDENT,
    CODE_INT,

    CODE_ASSIGN,
    CODE_PLUS,
    CODE_MINUS,
    CODE_BANG,
    CODE_ASTERISK,
    CODE_SLASH,

    CODE_LT,
    CODE_GT,

    CODE_EQ,
    CODE_NOT_EQ,

    CODE_COMMA,
    CODE_SEMICOLON,

    CODE_LPAREN,
    CODE_RPAREN,
    CODE_LBRACE,
    CODE_RBRACE,

    CODE_FUNCTION,
    CODE_LET,
    CODE_TRUE,
    CODE_FALSE,
    CODE_IF,
    CODE_ELSE,
    CODE_RETURN,
};

TokenType TOKENTYPES[TOKENTYPES_LEN];
Keywords* KEYWORDS;

void InitializeTokenTypes();
void InitializeKeywords();
TokenType LookupIdent(char* ident);

#endif
