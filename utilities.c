#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Clear console
void clrscr() {
    system("@cls||clear");
}

// Clear standard input (keyboard) buffer
void clrbuffer() {
    fflush(stdin);
}

// Enter to continue
void enterToContinue() {
    clrbuffer();
    printf("Pressione a tecla enter para voltar...");
    getchar();
}