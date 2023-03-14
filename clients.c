#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void clientsMain() {
    int option, id, nif;
    char username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME], address[SIZE_ADDRESS], nifStr[SIZE_NIF];
    float balance;

    do {
        Client* head = readClients();

        clrscr();
        menuHeaderClients();

        if (listClients(head) == 0) puts("\n                                        Nao existem clientes registados!                                         \n");

        menuFooterClients();
        scanf("%d", &option);

        switch (option) {
            case 1:
                clrscr();
                menuTitleInsertClient();

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
                scanf("%d", &nif);

                printf("Morada: ");
                clrbuffer();
                fgets(address, sizeof(address), stdin);
                address[strcspn(address, "\n")] = 0;

                head = insertClient(head, assignClientId(head), username, password, name, nif, address, 0);
                saveClients(head);

                break;
            case 2:
                menuTitleEditClient();
                scanf("%d", &id);

                if (!existClient(head, id)) break;

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

                printf("Morada: ");
                clrbuffer();
                fgets(address, sizeof(address), stdin);
                address[strcspn(address, "\n")] = 0;
                
                editClient(head, id, username, password, name, nif, address);
                saveClients(head);

                break;
            case 3:
                menuTitleRemoveClient();
                scanf("%d", &id);

                head = removeClient(head, id);
                saveClients(head);

                break;
            default:
                break;
        }

    } while (option != 0);
}

// Insert New Client
Client* insertClient(Client* head, int id, char username[], char password[], char name[], int nif, char address[], float balance) {
    Client *new = malloc(sizeof(struct client)), *prev;

    if (new != NULL) {
        new->id = id;
        strcpy(new->username, username);
        strcpy(new->password, password);
        strcpy(new->name, name);
        new->nif = nif;
        strcpy(new->address, address);
        new->balance = balance;
        new->next = NULL;
    }

    if (head == NULL) return new;

    for (prev = head; prev->next != NULL; prev = prev->next);

    prev->next = new;

	return head;
}

// Remove Client by ID
Client* removeClient(Client* head, int id) {
    Client *prev=head, *current=head, *aux;

    if (current == NULL) {
        return NULL;
    } else if (current->id == id) {
        aux = current->next;
        free(current);

        return aux;
    } else {
        while ((current != NULL) && (current->id != id)) {
            prev = current;
            current = current->next;
        }
        if (current == NULL) {
            return head;
        } else {
            prev->next = current->next;
            free(current);

            return head;
        }
    }
}

// Edit Client
void editClient(Client* head, int id, char username[], char password[], char name[], int nif, char address[]) {
    Client* current = head;

    while (current != NULL) {
        if (current->id == id) {
            if (strlen(username) > 0) strcpy(current->username, username);
            if (strlen(password) > 0) strcpy(current->password, password);
            if (strlen(name) > 0) strcpy(current->name, name);
            if (nif >= 0) current->nif = nif;
            if (strlen(address) > 0) strcpy(current->address, address);

            break;
        }

        current = current->next;
    }
}

// List Clients in Console
int listClients(Client* head) {
    if (head != NULL) {
        while (head != NULL) {
            printf("  %06d\t%-25s\t%-25s\t%-10.2f\t%-15d\t%-25s\t\n", head->id, head->name, head->username, head->balance, head->nif, head->address);

            head = head->next;
        }

        return 1;
    }

    return 0;
}

// Get Client Name from Client ID
char* getClientName(Client* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->name;

        head = head->next;
    }

    return "*********";
}

// Check if Client ID exists
int existClient(Client* head, int id) {
    while (head != NULL) {
        if (head->id == id) return 1;

        head = head->next;
    }

    return 0;
}

// Assign an ID to a Client based on the last Client in the list (+1)
int assignClientId(Client* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }
}

// Update Client Balance
void updateBalance(Client* head, int id, float balance) {
    while (head != NULL) {
        if (head->id == id) head->balance = balance;
        
        head = head->next;
    }
}

// Save Clients in File
int saveClients(Client* head) {
    FILE* fp;
    fp = fopen("clients.txt", "w");

    if (fp != NULL) {
        Client* aux = head;

        while (aux != NULL) {
            fprintf(fp, "%d;%s;%s;%s;%d;%s;%f\n", aux->id, aux->username, aux->password, aux->name, aux->nif, aux->address, aux->balance);
            aux = aux->next;
        }

        fclose(fp);

        return 1;
    }
    
    return 0;
}

// Read Clients from File
Client* readClients() {
    FILE* fp;
    fp = fopen("clients.txt", "r");
    Client* aux = NULL;

    if (fp != NULL) {
        int id, nif;
        char username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME], address[SIZE_ADDRESS];
        float balance;

        while (!feof(fp)) { 
            fscanf(fp, "%d;%[^;];%[^;];%[^;];%d;%[^;];%f\n", &id, &username, &password, &name, &nif, &address, &balance);
            aux = insertClient(aux, id, username, password, name, nif, address, balance);
        }

        fclose(fp);
    }

    return aux;
}