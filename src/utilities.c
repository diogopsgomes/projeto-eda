#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

// Clear console
/**
 * It clears the screen
 */
void clrscr() {
    system("@cls||clear");
}

// Clear standard input (keyboard) buffer
/**
 * It clears the input buffer
 */
void clrbuffer() {
    fflush(stdin);
}

// Enter to continue
/**
 * It clears the buffer and prints a message to the user, then waits for the user to press a key
 */
void enterToContinue() {
    clrbuffer();
    printf(GREEN"Pressione uma tecla para voltar..."RESET);
    getchar();
}

// Show the amount of results displayed
/**
 * It prints a message to the user, telling them how many results were found
 *
 * @param count The number of results to be shown.
 */
void showCount(int count) {
    if (count != 1) {
        printf(GREEN"A mostrar %d resultados.\n"RESET, count);
    } else {
        printf(GREEN"A mostrar %d resultado.\n"RESET, count);
    }
}