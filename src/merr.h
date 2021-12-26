#ifndef __MERR_H__
#define __MERR_H__

#include "token.h"

#define ERROR_NUM 100;

typedef char* Error;

typedef struct _Errors {
    int len;
    Error* arr;
} Errors;

Error newError(const char* msg);
Errors* newErrors();
void freeError(Error error);
void freeErrors(Errors* errors);
Error getErrorAt(Errors* errors, unsigned int idx);
void appendError(Errors* errors, Error error);
void peekError(Errors* errors, Token* peekToken, TokenType tokenType);
void printfError(const char* errkfmt, ...);

#endif
