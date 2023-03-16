#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main() {
    int optionA, optionB, optionC, user;
    char username[SIZE_USERNAME], password[SIZE_PASSWORD];

    do {
        clrscr();
        menuAuth();
        scanf("%d", &optionA);

        switch (optionA) {
            case 1:
                clrscr();
                menuAuthClients();
                scanf("%d", &optionB);

                break;
            case 2:
                do {
                    Manager* head = readManagers();

                    clrscr();
                    menuAuthManagers();
                    scanf("%d", &optionB);

                    switch (optionB) {
                        case 1:
                            clrscr();

                            // printf("Nome de Utilizador: ");
                            // clrbuffer();
                            // fgets(username, sizeof(username), stdin);
                            // username[strcspn(username, "\n")] = 0;

                            // printf("Palavra-passe: ");
                            // clrbuffer();
                            // fgets(password, sizeof(password), stdin);
                            // password[strcspn(password, "\n")] = 0;

                            // if ((user = authManager(head, username, password)) <= 0) {
                            //     puts("\nO nome de utilizador ou palavra-passe estao incorretos.\n");
                            //     enterToContinue();
                            //     break;
                            // }

                            do {
                                clrscr();
                                printf("Ola, %s!\n\n", getManagerName(head, user));
                                menuMain();
                                scanf("%d", &optionC);

                                switch (optionC) {
                                    case 1:
                                        
                                        break;
                                    case 2:
                                        vehiclesMain();
                                        break;
                                    case 3:
                                        clientsMain();
                                        break;
                                    case 4:
                                        managersMain();
                                        break;
                                    default:
                                        break;
                                }

                            } while (optionC != 0);

                            break;
                        default:
                            break;
                    }

                } while (optionB != 0);

                break;
            default:
                break;
        }

    } while (optionA != 0);

    clrscr();

    return 0;
}