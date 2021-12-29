#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "repl.h"
#include "lexer.h"
#include "token.h"
#define INPUT_LEN 1024

void scanInput(char* input, int inputLen) {
    int idx = 0;
    char ch; 
    while((ch = getchar()) != '\n' && idx < inputLen-1) { input[idx] = ch; idx++;
    }
    input[idx] = '\0';
}

void start() {
    InitializeTokenTypes();
    InitializeKeywords();

    const char* prompt = ">> ";
    char input[INPUT_LEN];

    while(1) {
        printf("%s", prompt);
        scanInput(input, INPUT_LEN);

        Lexer* l = newLexer(input);

        if(input[0] == EOF || !strcmp(input, "quit")) {
            return;
        }

        while(1) {
            Token* tok = nextToken(l);

            if(tok->tokenType == TOKEN_EOF) {
                break;
            }

            printf("{ Token : %-8s , Literal : %s } \n", 
                    getTokenTypeString(tok->tokenType), tok->literal);
        }
    }
}
