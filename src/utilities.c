#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

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
    printf("Pressione uma tecla para voltar...");
    getchar();
}

// Show the amount of results displayed
void showCount(int count) {
    if (count != 1) {
        printf("A mostrar %d resultados.\n", count);
    } else {
        printf("A mostrar %d resultado.\n", count);
    }
}