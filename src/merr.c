#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "merr.h"
#include "token.h"

#define ERROR_SIZE 100
#define ERRORS_SIZE 100

#define ANSI_CORLOR_RED     "\x1b[31m"
#define ANSI_CORLOR_RESET   "\x1b[0m"

Error newError(const char* msg) {
    Error error = NULL;
    int errlen = ERROR_SIZE;

    if(msg != NULL)  {
        errlen = strlen(msg) + 1;
        error = (char*)malloc(sizeof(char) * errlen);
        strcpy(error, msg);
    } else {
        error = (char*)malloc(sizeof(char) * errlen);
        memset(error, 0, sizeof(char) * errlen);
    }

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
    int errlen = sprintf(error, "expected next token to be '%s', got '%s' instead", tokenType, peekToken->type);
    error[errlen] = '\0';
    appendError(errors, error);
}

void printError(Error error) {
    printf(ANSI_CORLOR_RED "[ERROR] " ANSI_CORLOR_RESET "%s\n", error);
}
