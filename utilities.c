#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void clrscr() // Clear console
{
    system("@cls||clear");
}

void clrbuffer() // Clear standard input (keyboard) buffer
{
    fflush(stdin);
}

void enterToContinue() // Enter to continue
{
    clrbuffer();
    printf("Pressione a tecla enter para voltar...");
    getchar();
}

void readStr(char* str) {
    clrbuffer();
    scanf("%s", str);
    //scanf("%[^\n]", str);
    /* fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0; */
}