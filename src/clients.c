#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

void clientsMain() {
    int option, id, nif, count;
    char username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME], address[SIZE_ADDRESS], nifStr[SIZE_NIF];
    float balance;

    do {
        Client* head = readClients();

        clrscr();
        menuHeaderClients();

        if ((count = listClients(head)) == 0) {
            puts("\n                                                        Nao existem clientes registados!                                                         \n");
        } else {
            puts("");
            showCount(count);
        }

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

                head = insertClient(head, assignClientId(head), username, password, name, nif, address, 0, 1);
                saveClients(head);

                break;
            case 2:
                menuTitleEditClient();
                scanf("%d", &id);

                if (!existClient(head, id)) break;

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
                
                editClient(head, id, username, password, name, nif, address);
                saveClients(head);

                break;
            case 3:
                menuTitleRemoveClient();
                scanf("%d", &id);

                head = removeClient(head, id);
                saveClients(head);

                break;
            case 4:
                menuTitleAddBalance();
                scanf("%d", &id);

                if (!existClient(head, id)) break;

                printf("Montante: ");
                scanf("%f", &balance);

                addBalance(head, id, balance);
                saveClients(head);
                
                break;
            case 5:
                menuTitleRemoveBalance();
                scanf("%d", &id);

                if (!existClient(head, id)) break;

                printf("Montante: ");
                scanf("%f", &balance);

                removeBalance(head, id, balance);
                saveClients(head);

                break;
            default:
                break;
        }

    } while (option != 0);
}

// Insert New Client
/**
 * It inserts a new client at the end of the list
 * 
 * @param head The head of the linked list
 * @param id The id of the client
 * @param username The username of the client
 * @param password The password of the client
 * @param name The name of the client
 * @param nif The tax identification number of the client
 * @param address The address of the client
 * @param balance The balance of the client
 * @param available 0 = not available, 1 = available
 * 
 * @return The head of the list.
 */
Client* insertClient(Client* head, int id, char username[], char password[], char name[], int nif, char address[], float balance, int available) {
    Client *new = malloc(sizeof(struct client)), *aux = head;

    if (new != NULL) {
        new->id = id;
        strcpy(new->username, username);
        strcpy(new->password, password);
        strcpy(new->name, name);
        new->nif = nif;
        strcpy(new->address, address);
        new->balance = balance;
        new->available = available;
        new->next = NULL;
    }

    if (head == NULL) return new;

    while (aux->next != NULL) aux = aux->next;

    aux->next = new;

	return head;
}

// Remove Client by ID
/**
 * If the list is empty, return NULL. If the first element is the one to be removed, free it and return
 * the second element. Otherwise, find the element to be removed and free it
 * 
 * @param head The head of the linked list
 * @param id The id of the client to be removed
 * 
 * @return The head of the list.
 */
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
/**
 * It edits a client's information
 * 
 * @param head The head of the linked list
 * @param id The id of the client to edit
 * @param username The username of the client
 * @param password The password of the client
 * @param name The name of the client
 * @param nif The tax identification number of the client
 * @param address The address of the client
 */
void editClient(Client* head, int id, char username[], char password[], char name[], int nif, char address[]) {
    while (head != NULL) {
        if (head->id == id) {
            if (strlen(username) > 0) strcpy(head->username, username);
            if (strlen(password) > 0) strcpy(head->password, password);
            if (strlen(name) > 0) strcpy(head->name, name);
            if (nif >= 0) head->nif = nif;
            if (strlen(address) > 0) strcpy(head->address, address);

            break;
        }

        head = head->next;
    }
}

// List Clients in Console
/**
 * It prints the contents of a linked list of clients
 * 
 * @param head The head of the linked list
 * 
 * @return The number of clients in the list.
 */
int listClients(Client* head) {
    int count = 0;

    while (head != NULL) {
        printf("  %06d\t%-25s\t%-25s\t%-10.2f\t%-15d\t%-25s\t\n", head->id, head->name, head->username, head->balance, head->nif, head->address);

        count++;

        head = head->next;
    }

    return count;
}

// List Client in Console
/**
 * It prints the client's information if the client's id matches the id passed as an argument
 * 
 * @param head The head of the linked list
 * @param id The id of the client
 * 
 * @return The number of clients with the same id.
 */
int listClient(Client* head, int id) {
    int count = 0;

    while (head != NULL) {
        if (head->id == id) {
            printf("  %06d\t%-25s\t%-25s\t%-10.2f\t%-15d\t%-25s\t\n", head->id, head->name, head->username, head->balance, head->nif, head->address);

            count++;

            return count;
        }

        head = head->next;
    }

    return count;
}

// Get Client Name from Client ID
/**
 * It returns the name of the client with the given id, or "*********" if the client doesn't exist
 * 
 * @param head The head of the linked list
 * @param id The id of the client you want to get the name of
 * 
 * @return The name of the client with the given id.
 */
char* getClientName(Client* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->name;

        head = head->next;
    }

    return "*********";
}

// Get Client Username from Client ID
/**
 * Get the username of the client with the given id.
 * 
 * @param head The head of the linked list
 * @param id The id of the client you want to get the username of
 * 
 * @return The username of the client with the given id.
 */
char* getClientUsername(Client* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->username;

        head = head->next;
    }

    return "*********";
}

// Check if Username is already in use
/**
 * It returns 1 if the username exists in the linked list, otherwise it returns 0
 * 
 * @param head The head of the linked list
 * @param username The username
 * 
 * @return 1 if the username exists in the list, otherwise it returns 0.
 */
int existClientUsername(Client* head, char username[]) {
    while (head != NULL) {
        if (strcmp(head->username, username) == 0) return 1;

        head = head->next;
    }

    return 0;
}

// Check if Client ID exists
/**
 * It checks if a client with the given id exists in the list
 * 
 * @param head The head of the linked list
 * @param id The id of the client
 * 
 * @return 1 if the client exists in the list, otherwise it returns 0.
 */
int existClient(Client* head, int id) {
    while (head != NULL) {
        if (head->id == id) return 1;

        head = head->next;
    }

    return 0;
}

// Assign an ID to a Client based on the last Client in the list (+1)
/**
 * It returns the next available client id
 * 
 * @param head The head of the linked list
 * 
 * @return The next available client ID.
 */
int assignClientId(Client* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }

    return 1;
}

// Check if a Client in not in a Ride
/**
 * It checks if a client is available
 * 
 * @param head The head of the linked list
 * @param id The id of the client
 * 
 * @return The value of the head->available variable.
 */
int isClientAvailable(Client* head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            if (head->available == 1) {
                return 1;
            }
        }

        head = head->next;
    }

    return 0;
}

// Add a given amount to Client Balance
/**
 * It adds the balance to the client with the given id
 * 
 * @param head The head of the linked list
 * @param id The id of the client
 * @param balance The amount of money to add to the client's balance
 */
void addBalance(Client* head, int id, float balance) {
    while (head != NULL) {
        if (head->id == id) head->balance += balance;
        
        head = head->next;
    }
}

// Remove a given amount from Client Balance
/**
 * It removes the balance from the client with the given id
 * 
 * @param head The head of the linked list
 * @param id The id of the client
 * @param balance The amount of money to be removed from the client's balance
 */
void removeBalance(Client* head, int id, float balance) {
    while (head != NULL) {
        if (head->id == id) {
            if (head->balance > balance) {
                head->balance -= balance;
            } else {
                head->balance = 0;
            }
        }
        
        head = head->next;
    }
}

// Update Client Balance
/**
 * It loops through the linked list until it finds the client with the matching id, then it sets the
 * balance to the new balance
 * 
 * @param head The head of the linked list
 * @param id The id of the client to edit
 * @param balance The new balance
 */
void editBalance(Client* head, int id, float balance) {
    while (head != NULL) {
        if (head->id == id) head->balance = balance;
        
        head = head->next;
    }
}

// Save Clients in File
/**
 * It saves the clients to a file
 * 
 * @param head The head of the linked list
 * 
 * @return 1 if the file was saved successfully, or 0 if it wasn't.
 */
int saveClients(Client* head) {
    FILE* fp;
    fp = fopen(DATA_DIR"clients.txt", "w");

    if (fp == NULL) return 0;

    while (head != NULL) {
        fprintf(fp, "%d;%s;%s;%s;%d;%s;%f;%d\n", head->id, head->username, head->password, head->name, head->nif, head->address, head->balance, head->available);
        head = head->next;
    }

    fclose(fp);

    return 1;
}

// Read Clients from File
/**
 * It reads a file and inserts the data into a linked list
 * 
 * @return A pointer to a Client struct.
 */
Client* readClients() {
    FILE* fp;
    fp = fopen(DATA_DIR"clients.txt", "r");
    Client* aux = NULL;

    if (fp == NULL) return aux;
    
    int c = fgetc(fp);
    if (c == EOF) {
        fclose(fp);
        return aux;
    }
    ungetc(c, fp);

    int id, nif, available;
    char username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME], address[SIZE_ADDRESS];
    float balance;

    while (!feof(fp)) {
        fscanf(fp, "%d;%[^;];%[^;];%[^;];%d;%[^;];%f;%d\n", &id, &username, &password, &name, &nif, &address, &balance, &available);
        aux = insertClient(aux, id, username, password, name, nif, address, balance, available);
    }

    fclose(fp);

    return aux;
}