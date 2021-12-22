#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "repl.h"

int main() {
    char* username = getlogin();

    if(username == NULL) {
        exit(1);
    }

    printf("Hello %s! This is the CMonkey programming language!\n", username);
    printf("Feel free to type in commands\n");

    start();

    return 0;
}
