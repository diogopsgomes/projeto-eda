#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

int main() {
    int optionA, optionB, optionC, count, user;
    char username[SIZE_USERNAME], password[SIZE_PASSWORD];

    do {
        clrscr();
        menuApp();
        menuAuth();
        scanf("%d", &optionA);

        switch (optionA) {
            case 1:
                do {
                    Client* head = readClients();

                    clrscr();
                    menuApp();
                    menuAuthClients();
                    scanf("%d", &optionB);

                    switch (optionB) {
                        case 1:
                            clrscr();
                            menuApp();

                            printf("Nome de Utilizador: ");
                            clrbuffer();
                            fgets(username, sizeof(username), stdin);
                            username[strcspn(username, "\n")] = 0;

                            printf("Palavra-passe: ");
                            clrbuffer();
                            fgets(password, sizeof(password), stdin);
                            password[strcspn(password, "\n")] = 0;

                            if ((user = authClient(head, username, password)) <= 0) {
                                puts("\nO nome de utilizador ou palavra-passe estao incorretos.\n");
                                enterToContinue();
                                break;
                            }

                            do {
                                Vehicle* headVehicles = readVehicles();
                                Type* headTypes = readTypes();
                                Client* headClients = readClients();
                                int nif;
                                float balance;
                                char location[SIZE_LOCATION], username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME], address[SIZE_ADDRESS], nifStr[SIZE_NIF];

                                clrscr();
                                menuApp();
                                printf("Ola, %s!\n\n", getClientName(head, user));

                                menuHeaderClient();
                                if (listClient(headClients, user) == 0) {
                                    puts("\n                                                   Nao foi possivel carregar dados do perfil!                                                    \n");
                                } else {
                                    puts("");
                                }

                                menuHeaderVehicles();
                                if ((count = listVehiclesByRange(headVehicles, headTypes)) == 0) {
                                    puts("\n                                                        Nao existem veiculos registados!                                                         \n");
                                } else {
                                    puts("");
                                    showCount(count);
                                }

                                menuMainClients();
                                scanf("%d", &optionC);

                                switch (optionC) {
                                    case 1:
                                        
                                        break;
                                    case 2:
                                        
                                        break;
                                    case 3:
                                        menuTitleListVehiclesByLocation();
                                        clrbuffer();
                                        fgets(location, sizeof(location), stdin);
                                        location[strcspn(location, "\n")] = 0;

                                        clrscr();
                                        menuApp();
                                        menuHeaderVehicles();

                                        if ((count = listVehiclesByLocation(headVehicles, headTypes, location)) == 0) {
                                            puts("\n                                                     Nao existem veiculos nessa localizacao!                                                     \n");
                                        } else {
                                            puts("");
                                            showCount(count);
                                        }

                                        puts("");
                                        enterToContinue();

                                        break;
                                    case 4:
                                        menuMainClientsLine();
                                        printf("Montante: ");
                                        scanf("%f", &balance);

                                        addBalance(head, user, balance);
                                        saveClients(head);

                                        break;
                                    case 5:
                                        menuMainClientsLine();

                                        nif = -1;

                                        printf("Nome: ");
                                        clrbuffer();
                                        fgets(name, sizeof(name), stdin);
                                        name[strcspn(name, "\n")] = 0;

                                        printf("Nome de Utilizador: ");
                                        clrbuffer();
                                        fgets(username, sizeof(username), stdin);
                                        username[strcspn(username, "\n")] = 0;

                                        printf("Palavra-passe: ");
                                        clrbuffer();
                                        fgets(password, sizeof(password), stdin);
                                        password[strcspn(password, "\n")] = 0;
                                        encrypt(password);

                                        printf("NIF: ");
                                        clrbuffer();
                                        fgets(nifStr, sizeof(nifStr), stdin);
                                        nifStr[strcspn(nifStr, "\n")] = 0;
                                        if (strlen(nifStr) > 0) nif = atoi(nifStr);

                                        printf("Morada: ");
                                        clrbuffer();
                                        fgets(address, sizeof(address), stdin);
                                        address[strcspn(address, "\n")] = 0;
                                        
                                        editClient(headClients, user, username, password, name, nif, address);
                                        saveClients(headClients);

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
            case 2:
                do {
                    Manager* head = readManagers();

                    clrscr();
                    menuApp();
                    menuAuthManagers();
                    scanf("%d", &optionB);

                    switch (optionB) {
                        case 1:
                            clrscr();
                            menuApp();

                            printf("Nome de Utilizador: ");
                            clrbuffer();
                            fgets(username, sizeof(username), stdin);
                            username[strcspn(username, "\n")] = 0;

                            printf("Palavra-passe: ");
                            clrbuffer();
                            fgets(password, sizeof(password), stdin);
                            password[strcspn(password, "\n")] = 0;

                            if ((user = authManager(head, username, password)) <= 0) {
                                puts("\nO nome de utilizador ou palavra-passe estao incorretos.\n");
                                enterToContinue();
                                break;
                            }

                            do {
                                clrscr();
                                menuApp();
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