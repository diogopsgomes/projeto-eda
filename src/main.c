#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/header.h"

int main() {
    Ride* headRides;
    Vehicle* headVehicles, * headVehiclesSorted;
    Type* headTypes;
    Client* headClients;
    Manager* headManagers;
    Vertice* headVertices;
    int optionA, optionB, optionC, count, valid, nif, available, charged, ride, vehicle, user;
    float balance;
    char location[SIZE_LOCATION], username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME], address[SIZE_ADDRESS], nifStr[SIZE_NIF];

    do {
        clrscr();
        menuApp();
        menuAuth();
        scanf("%d", &optionA);

        switch (optionA) {
            case 1:
                do {
                    headClients = readClients();

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

                            if ((user = authClient(headClients, username, password)) < 0) {
                                puts(RED"\nO nome de utilizador ou palavra-passe estao incorretos!\n"RESET);
                                enterToContinue();
                                break;
                            }

                            do {
                                headRides = readRides();
                                headVehicles = readVehicles();
                                headTypes = readTypes();
                                headClients = readClients();

                                available = isClientAvailable(headClients, user);

                                clrscr();
                                menuApp();
                                printf("Ola, %s!\n\n", getClientName(headClients, user));

                                menuHeaderClient();
                                if (listClient(headClients, user) == 0) {
                                    puts("\n                                                   Nao foi possivel carregar dados do perfil!                                                    \n");
                                } else {
                                    puts("");
                                }

                                menuHeaderVehicles();
                                headVehiclesSorted = copyLinkedList(headVehicles);
                                if ((count = listVehiclesByRange(headVehiclesSorted, headTypes)) == 0) {
                                    puts("\n                                                        Nao existem veiculos registados!                                                         \n");
                                } else {
                                    puts("");
                                    showCount(count);
                                }

                                menuMainClients(available);
                                scanf("%d", &optionC);

                                switch (optionC) {
                                    case 1:
                                        menuMainClientsLine();

                                        if (available == 1) {
                                            if (hasBalance(headClients, user) == 0) {
                                                puts(RED"\nSaldo insuficiente!\n"RESET);
                                                enterToContinue();
                                                break;
                                            }

                                            do {
                                                printf("Veiculo: ");
                                                scanf("%d", &vehicle);
                                                if ((available = isVehicleAvailable(headVehicles, vehicle)) == 0) {
                                                    puts(RED"\nVeiculo em uso!\n"RESET);
                                                }
                                                if ((charged = isVehicleCharged(headVehicles, vehicle)) == 0) {
                                                    puts(RED"\nVeiculo sem bateria!\n"RESET);
                                                }
                                            } while (available == 0 || charged == 0);

                                            headRides = startRide(headRides, headVehicles, headTypes, headClients, assignRideId(headRides), vehicle, user);

                                            puts(GREEN"\nBoa Viagem!\n"RESET);
                                            enterToContinue();
                                        } else {
                                            printf("Localizacao: ");
                                            clrbuffer();
                                            fgets(location, sizeof(location), stdin);
                                            location[strcspn(location, "\n")] = 0;

                                            ride = currentRide(headRides, user);

                                            endRide(headRides, headVehicles, headTypes, headClients, ride, location);

                                            puts(GREEN"\nViagem Terminada!\n"RESET);
                                            showRide(headRides, ride);
                                            puts("");
                                            enterToContinue();
                                        }

                                        saveRides(headRides);
                                        saveVehicles(headVehicles);
                                        saveClients(headClients);

                                        break;
                                    case 2:
                                        clrscr();
                                        menuApp();
                                        menuHeaderRidesClient();

                                        if ((count = listRidesClient(headRides, headClients, user)) == 0) {
                                            puts("\n                                                             Nao existem viagens registadas!                                                             \n");
                                        } else {
                                            puts("");
                                            showCount(count);
                                        }

                                        puts("");
                                        enterToContinue();

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

                                        addBalance(headClients, user, balance);
                                        saveClients(headClients);

                                        break;
                                    case 5:
                                        menuMainClientsLine();

                                        nif = -1;

                                        printf("Nome: ");
                                        clrbuffer();
                                        fgets(name, sizeof(name), stdin);
                                        name[strcspn(name, "\n")] = 0;

                                        do {
                                            printf("Nome de Utilizador: ");
                                            clrbuffer();
                                            fgets(username, sizeof(username), stdin);
                                            username[strcspn(username, "\n")] = 0;
                                            if (strlen(username) == 0) break;
                                            if ((available = (existClientUsername(headClients, username) == 1) ? (0) : (1)) == 0) {
                                                puts(RED"\nNome de Utilizador indisponivel!\n"RESET);
                                            }
                                        } while (available == 0);

                                        do {
                                            printf("Palavra-passe: ");
                                            clrbuffer();
                                            fgets(password, sizeof(password), stdin);
                                            password[strcspn(password, "\n")] = 0;
                                            if (strlen(password) == 0) break;
                                            if ((valid = (strlen(password) < 6) ? (0) : (1)) == 0) {
                                                puts(RED"\nPalavra-passe invalida!\n"RESET);
                                            }
                                        } while (valid == 0);
                                        encrypt(password);

                                        do {
                                            printf("NIF: ");
                                            clrbuffer();
                                            fgets(nifStr, sizeof(nifStr), stdin);
                                            nifStr[strcspn(nifStr, "\n")] = 0;
                                            if (strlen(nifStr) == 0) break;
                                            if ((valid = (strlen(nifStr) != 9) ? (0) : (1)) == 0) {
                                                puts(RED"\nNIF invalido!\n"RESET);
                                            }
                                        } while (valid == 0);
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
                        case 2:
                            clrscr();
                            menuApp();

                            do {
                                printf("Nome: ");
                                clrbuffer();
                                fgets(name, sizeof(name), stdin);
                                name[strcspn(name, "\n")] = 0;
                            } while (strlen(name) == 0);

                            do {
                                printf("Nome de Utilizador: ");
                                clrbuffer();
                                fgets(username, sizeof(username), stdin);
                                username[strcspn(username, "\n")] = 0;
                                if ((available = (existClientUsername(headClients, username) == 1) ? (0) : (1)) == 0) {
                                    puts(RED"\nNome de Utilizador indisponivel!\n"RESET);
                                }
                            } while (available == 0 || strlen(username) == 0);

                            do {
                                printf("Palavra-passe: ");
                                clrbuffer();
                                fgets(password, sizeof(password), stdin);
                                password[strcspn(password, "\n")] = 0;
                                if ((valid = (strlen(password) < 6) ? (0) : (1)) == 0) {
                                    puts(RED"\nPalavra-passe invalida!\n"RESET);
                                }
                            } while (valid == 0);
                            encrypt(password);

                            do {
                                printf("NIF: ");
                                scanf("%d", &nif);
                                if ((valid = (nif < 100000000 || nif > 999999999) ? (0) : (1)) == 0) {
                                    puts(RED"\nNIF invalido!\n"RESET);
                                }
                            } while (valid == 0);

                            do {
                                printf("Morada: ");
                                clrbuffer();
                                fgets(address, sizeof(address), stdin);
                                address[strcspn(address, "\n")] = 0;
                            } while (strlen(address) == 0);

                            headClients = insertClient(headClients, assignClientId(headClients), username, password, name, nif, address, 0, 1);
                            saveClients(headClients);

                            puts("\nConta criada com sucesso!\n");
                            enterToContinue();

                            break;
                        default:
                            break;
                    }

                } while (optionB != 0);

                break;
            case 2:
                do {
                    headManagers = readManagers();

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

                            if ((user = authManager(headManagers, username, password)) < 0) {
                                puts(RED"\nO nome de utilizador ou palavra-passe estao incorretos.\n"RESET);
                                enterToContinue();
                                break;
                            }

                            do {
                                clrscr();
                                menuApp();
                                printf("Ola, %s!\n\n", getManagerName(headManagers, user));
                                menuMain();
                                scanf("%d", &optionC);

                                switch (optionC) {
                                    case 1:
                                        ridesMain();
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
            case 3:
                headVertices = createGraph();

                puts("----------------------------------------");
                puts("Localizacoes adjacentes a tatica.ideia.mornos:\n");
                listAdjacents(headVertices, "tatica.ideia.morno");
                puts("----------------------------------------");

                enterToContinue();
            default:
                break;
        }

    } while (optionA != 0);

    clrscr();

    return 0;
}