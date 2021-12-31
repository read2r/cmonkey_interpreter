#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "merr.h"
#include "token.h"

#define ERROR_SIZE 100
#define ERRORS_SIZE 100

#define ANSI_CORLOR_RED     "\x1b[31m"
#define ANSI_CORLOR_RESET   "\x1b[0m"

Error newError(const char* errfmt, ...) {
    char buf[512] = {0,};
    va_list ap;

    va_start(ap, errfmt);
    vsprintf(buf, errfmt, ap);
    va_end(ap);

    Error error = (char*)malloc(sizeof(char) * (strlen(buf) + 1));
    strcpy(error, buf);

    return error;
}

Errors* newErrors() {
    Errors* errors = (Errors*)malloc(sizeof(Errors));
    errors->len = 0;
    errors->arr = (Error*)malloc(sizeof(Error) * ERRORS_SIZE);
    return errors;
}

void freeError(Error error) {
    if(error != NULL) {
        free(error);
        error = NULL;
    }
}

void freeErrors(Errors* errors) {
    if(errors != NULL) {
        for(int i = 0; i < errors->len; i++) {
            freeError(errors->arr[i]);
        }
        errors = NULL;
    }
}

Error getErrorAt(Errors *errors, unsigned int idx) {
    return errors->arr[idx];
}

void appendError(Errors* errors, Error error) {
    errors->arr[errors->len++] = error; 
}

void peekError(Errors* errors, Token* peekToken, TokenType tokenType) {
    Error error = newError(NULL);
    int errlen = sprintf(error, "expected next token to be '%s', got '%s' instead",
            getTokenTypeString(tokenType), getTokenTypeString(peekToken->tokenType));
    error[errlen] = '\0';
    appendError(errors, error);
}

void printfError(const char* errfmt, ...) {
    char buf[512] = {0,};
    va_list ap;

    strcpy(buf, ANSI_CORLOR_RED "[ERROR] " ANSI_CORLOR_RESET);
    va_start(ap, errfmt);
    vsprintf(buf + strlen(buf), errfmt, ap);
    va_end(ap);

    puts(buf);
}
