#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void menuMain() {
    puts("---------------------------------------------------------------------------------------------");
    puts("|                                       MENU PRINCIPAL                                      |");
    puts("---------------------------------------------------------------------------------------------");
    puts("| 1 - Viagens                                                                               |");
    puts("| 2 - Veiculos                                                                              |");
    puts("| 3 - Clientes                                                                              |");
    puts("| 4 - Gestores                                                                              |");
    puts("| 0 - Sair                                                                                  |");
    puts("---------------------------------------------------------------------------------------------");
    printf("Escolha uma opcao: ");
}

void menuAuth() {
    puts("--------------------------------------------------");
    puts("|                  AUTENTICACAO                  |");
    puts("--------------------------------------------------");
    puts("| 1 - Sou Cliente                                |");
    puts("| 2 - Sou Gestor                                 |");
    puts("| 0 - Sair                                       |");
    puts("--------------------------------------------------");
    printf("Escolha uma opcao: ");
}

void menuAuthClients() {
    puts("--------------------------------------------------");
    puts("|              AUTENTICACAO CLIENTE              |");
    puts("--------------------------------------------------");
    puts("| 1 - Iniciar Sessao                             |");
    puts("| 2 - Criar Conta                                |");
    puts("| 0 - Sair                                       |");
    puts("--------------------------------------------------");
    printf("Escolha uma opcao: ");
}

void menuAuthManagers() {
    puts("--------------------------------------------------");
    puts("|               AUTENTICACAO GESTOR              |");
    puts("--------------------------------------------------");
    puts("| 1 - Iniciar Sessao                             |");
    puts("| 0 - Sair                                       |");
    puts("--------------------------------------------------");
    printf("Escolha uma opcao: ");
}

void menuHeaderVehicles() {
    puts("-----------------------------------------------------------------------------------------------------------------");
    puts("|                                                   Veiculos                                                    |");
    puts("-----------------------------------------------------------------------------------------------------------------");
    puts("| Codigo\tTipo\t\t\tBateria\t\t\tAutonomia\t\tLocalizacao\t\t|");
    puts("-----------------------------------------------------------------------------------------------------------------");
}

void menuFooterRides()
{
    puts("----------------------------------------------------------------------------");
    printf("| 1 - Inserir | 2 - Editar | 3 - Remover | 0 - Voltar | Escolha uma opcao: ");
}

void menuFooterVehicles()
{
    puts("-----------------------------------------------------------------------------------------------------------------");
    printf("| 1 - Inserir | 2 - Editar | 3 - Remover | 4 - Atualizar | 0 - Voltar | Escolha uma opcao: ");
}

void menuFooterClients()
{
    puts("----------------------------------------------------------------------------");
    printf("| 1 - Inserir | 2 - Editar | 3 - Remover | 0 - Voltar | Escolha uma opcao: ");
}

void menuFooterManagers() {
    puts("----------------------------------------------------------------------------");
    printf("| 1 - Inserir | 2 - Editar | 3 - Remover | 0 - Voltar | Escolha uma opcao: ");
}

void menuTitleInsertVehicle() {
    puts("----------------------------------------");
    puts("|         Inserir novo veiculo         |");
    puts("----------------------------------------");
}

void menuTitleRemoveVehicle() {
    puts("-----------------------------------------------------------------------------------------------------------------");
    printf("Codigo do veiculo a remover: ");
}

void menuTitleEditVehicle() {
    puts("-----------------------------------------------------------------------------------------------------------------");
    printf("Codigo do veiculo a editar: ");
}