#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

void menuApp() {
    printf(YELLOW);
    puts(" __   _____  _  _____ ");
    puts(" \\ \\ / / _ \\| ||_   _|");
    puts("  \\ V / (_) | |__| |  ");
    puts("   \\_/ \\___/|____|_|      by Diogo Gomes");
    puts("                      ");
    printf(RESET);
}

void menuMain() {
    puts("----------------------------------------");
    puts("|            MENU PRINCIPAL            |");
    puts("----------------------------------------");
    puts("| 1 - Viagens                          |");
    puts("| 2 - Veiculos                         |");
    puts("| 3 - Clientes                         |");
    puts("| 4 - Gestores                         |");
    puts("| 0 - Sair                             |");
    puts("----------------------------------------");
    printf("Escolha uma opcao: ");
}

void menuMainClients(int available) {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    if (available == 1) {
        printf("| 1 - Iniciar Viagem | 2 - Historico de Viagens | 3 - Procurar Veiculo | 4 - Adicionar Saldo | 5 - Editar Perfil | 6 - Atualizar | 0 - Voltar | Escolha uma opcao: ");
    } else {
        printf("| 1 - Terminar Viagem | 2 - Historico de Viagens | 3 - Procurar Veiculo | 4 - Adicionar Saldo | 5 - Editar Perfil | 6 - Atualizar | 0 - Voltar | Escolha uma opcao: ");
    }
}

void menuMainClientsLine() {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
}

void menuAuth() {
    puts("----------------------------------------");
    puts("|             AUTENTICACAO             |");
    puts("----------------------------------------");
    puts("| 1 - Sou Cliente                      |");
    puts("| 2 - Sou Gestor                       |");
    puts("| 0 - Sair                             |");
    puts("----------------------------------------");
    printf("Escolha uma opcao: ");
}

void menuAuthClients() {
    puts("----------------------------------------");
    puts("|         AUTENTICACAO CLIENTE         |");
    puts("----------------------------------------");
    puts("| 1 - Iniciar Sessao                   |");
    puts("| 2 - Criar Conta                      |");
    puts("| 0 - Sair                             |");
    puts("----------------------------------------");
    printf("Escolha uma opcao: ");
}

void menuAuthManagers() {
    puts("----------------------------------------");
    puts("|          AUTENTICACAO GESTOR         |");
    puts("----------------------------------------");
    puts("| 1 - Iniciar Sessao                   |");
    puts("| 0 - Sair                             |");
    puts("----------------------------------------");
    printf("Escolha uma opcao: ");
}

void menuHeaderVehicles() {
    printf(CYAN);
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    puts("|                                                                   Veiculos                                                                    |");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    puts("| Codigo\tTipo\t\t\t\tBateria\t\t\tAutonomia\t\tLocalizacao\t\t\t\t\t|");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf(RESET);
}

void menuHeaderClients() {
    printf(CYAN);
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    puts("|                                                                   Clientes                                                                    |");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    puts("| Codigo\tNome\t\t\t\tUtilizador\t\t\tSaldo\t\tNIF\t\tMorada\t\t\t\t|");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf(RESET);
}

void menuHeaderClient() {
    printf(CYAN);
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    puts("|                                                                    Perfil                                                                     |");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    puts("| Codigo\tNome\t\t\t\tUtilizador\t\t\tSaldo\t\tNIF\t\tMorada\t\t\t\t|");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf(RESET);
}

void menuHeaderManagers() {
    printf(CYAN);
    puts("-------------------------------------------------------------------------------------------------");
    puts("|                                           Gestores                                            |");
    puts("-------------------------------------------------------------------------------------------------");
    puts("| Codigo\tNome\t\t\t\tUtilizador\t\t\t\t\t|");
    puts("-------------------------------------------------------------------------------------------------");
    printf(RESET);
}

void menuFooterRides()
{
    puts("-----------------------------------------------------------------------------------------------------------------");
    printf("| 1 - Inserir | 2 - Editar | 3 - Remover | 4 - Atualizar | 0 - Voltar | Escolha uma opcao: ");
}

void menuFooterVehicles()
{
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("| 1 - Inserir | 2 - Editar | 3 - Remover | 4 - Listar por Autonomia | 5 - Listar por Localizacao | 6 - Atualizar | 0 - Voltar | Escolha uma opcao: ");
}

void menuFooterClients()
{
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("| 1 - Inserir | 2 - Editar | 3 - Remover | 4 - Adicionar Saldo | 5 - Remover Saldo | 6 - Atualizar | 0 - Voltar | Escolha uma opcao: ");
}

void menuFooterManagers() {
    puts("-------------------------------------------------------------------------------------------------");
    printf("| 1 - Inserir | 2 - Editar | 3 - Remover | 4 - Atualizar | 0 - Voltar | Escolha uma opcao: ");
}

void menuTitleInsertVehicle() {
    puts("----------------------------------------");
    puts("|         Inserir novo veiculo         |");
    puts("----------------------------------------");
}

void menuTitleRemoveVehicle() {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("Codigo do veiculo a remover: ");
}

void menuTitleEditVehicle() {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("Codigo do veiculo a editar: ");
}

void menuTitleListVehiclesByLocation() {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("Localizacao: ");
}

void menuTitleInsertClient() {
    puts("----------------------------------------");
    puts("|         Inserir novo cliente         |");
    puts("----------------------------------------");
}

void menuTitleRemoveClient() {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("Codigo do cliente a remover: ");
}

void menuTitleEditClient() {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("Codigo do cliente a editar: ");
}

void menuTitleAddBalance() {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("Codigo do cliente a adicionar saldo: ");
}

void menuTitleRemoveBalance() {
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("Codigo do cliente a remover saldo: ");
}

void menuTitleInsertManager() {
    puts("----------------------------------------");
    puts("|         Inserir novo gestor          |");
    puts("----------------------------------------");
}

void menuTitleRemoveManager() {
    puts("-------------------------------------------------------------------------------------------------");
    printf("Codigo do gestor a remover: ");
}

void menuTitleEditManager() {
    puts("-------------------------------------------------------------------------------------------------");
    printf("Codigo do gestor a editar: ");
}