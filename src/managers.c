#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

void managersMain() {
    int option, id, count, available, valid;
    char username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME];

    do {
        Manager* head = readManagers();

        clrscr();
        menuHeaderManagers();

        if ((count = listManagers(head)) == 0) {
            puts("\n                                Nao existem gestores registados!                                 \n");
        } else {
            puts("");
            showCount(count);
        }

        menuFooterManagers();
        scanf("%d", &option);

        switch (option) {
            case 1:
                clrscr();
                menuTitleInsertManager();

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
                    if ((available = (existManagerUsername(head, username) == 1) ? (0) : (1)) == 0) {
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

                head = insertManager(head, assignManagerId(head), username, password, name);
                saveManagers(head);

                break;
            case 2:
                menuTitleEditManager();
                scanf("%d", &id);

                if (!existManager(head, id)) break;

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
                    if ((available = (existManagerUsername(head, username) == 1) ? (0) : (1)) == 0) {
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

                editManager(head, id, username, password, name);
                saveManagers(head);

                break;
            case 3:
                menuTitleRemoveManager();
                scanf("%d", &id);

                head = removeManager(head, id);
                saveManagers(head);

                break;
            default:
                break;
        }

    } while (option != 0);
}

// Insert New Manager
/**
 * It inserts a new manager at the end of the list
 *
 * @param head The head of the linked list
 * @param id The id
 * @param username The username
 * @param password The password
 * @param name The name
 *
 * @return The head of the list.
 */
Manager* insertManager(Manager* head, int id, char username[], char password[], char name[]) {
    Manager* new = malloc(sizeof(struct manager)), * aux = head;

    if (new != NULL) {
        new->id = id;
        strcpy(new->username, username);
        strcpy(new->password, password);
        strcpy(new->name, name);
        new->next = NULL;
    }

    if (head == NULL) return new;

    while (aux->next != NULL) aux = aux->next;

    aux->next = new;

    return head;
}

// Remove Manager by ID
/**
 * If the list is empty, return NULL. If the first element is the one to be removed, remove it and
 * return the new head. Otherwise, find the element to be removed and remove it
 *
 * @param head The head of the linked list
 * @param id The id of the manager to be removed
 *
 * @return The head of the list.
 */
Manager* removeManager(Manager* head, int id) {
    Manager* prev = head, * current = head, * aux;

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

// Edit Manager
/**
 * It's a function that edits a manager's information
 *
 * @param head The head of the linked list
 * @param id The id of the manager to edit
 * @param username The username
 * @param password The password
 * @param name The name
 */
void editManager(Manager* head, int id, char username[], char password[], char name[]) {
    while (head != NULL) {
        if (head->id == id) {
            if (strlen(username) > 0) strcpy(head->username, username);
            if (strlen(password) > 0) strcpy(head->password, password);
            if (strlen(name) > 0) strcpy(head->name, name);

            break;
        }

        head = head->next;
    }
}

// List Managers in Console
/**
 * It prints the id, name, and username of each manager in the list
 *
 * @param head The head of the linked list
 *
 * @return The number of managers in the list.
 */
int listManagers(Manager* head) {
    int count = 0;

    while (head != NULL) {
        printf("  %06d\t%-25s\t%-25s\t\n", head->id, head->name, head->username);

        count++;

        head = head->next;
    }

    return count;
}

// Get Manager Name from Manager ID
/**
 * It returns the name of the manager with the given id, or "*********" if no manager with that id
 * exists
 *
 * @param head The head of the linked list
 * @param id The id of the manager you want to get the name of
 *
 * @return The name of the manager with the given id.
 */
char* getManagerName(Manager* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->name;

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
int existManagerUsername(Manager* head, char username[]) {
    while (head != NULL) {
        if (strcmp(head->username, username) == 0) return 1;

        head = head->next;
    }

    return 0;
}

// Check if Manager ID exists
/**
 * It checks if a manager with the given id exists in the list
 *
 * @param head The head of the linked list
 * @param id The id of the manager
 *
 * @return 1 if the manager exists in the list, otherwise it returns 0.
 */
int existManager(Manager* head, int id) {
    while (head != NULL) {
        if (head->id == id) return 1;

        head = head->next;
    }

    return 0;
}

// Assign an ID to a Manager based on the last Manager in the list (+1)
/**
 * It returns the next available manager id.
 *
 * @param head The head of the linked list
 *
 * @return The id of the last manager in the list.
 */
int assignManagerId(Manager* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }

    return 1;
}

// Save Managers in File
/**
 * It saves the managers to a file
 *
 * @param head The head of the linked list
 *
 * @return 1 if the file was saved successfully, and 0 if it wasn't.
 */
int saveManagers(Manager* head) {
    FILE* fp;
    fp = fopen(DATA_DIR"managers.txt", "w");

    if (fp == NULL) return 0;

    while (head != NULL) {
        fprintf(fp, "%d;%s;%s;%s\n", head->id, head->username, head->password, head->name);

        head = head->next;
    }

    fclose(fp);

    fp = fopen(DATA_DIR"managers.bin", "wb");

    if (fp != NULL) {
        while (head != NULL) {
            fwrite(head, sizeof(struct manager), 1, fp);

            head = head->next;
        }

        fclose(fp);
    }

    return 1;
}

// Read Managers from File
/**
 * It reads a file and creates a linked list of managers
 *
 * @return A pointer to a Manager struct.
 */
Manager* readManagers() {
    FILE* fp;
    fp = fopen(DATA_DIR"managers.txt", "r");
    Manager* aux = NULL;

    if (fp == NULL) return aux;

    int c = fgetc(fp);
    if (c == EOF) {
        fclose(fp);
        return aux;
    }
    ungetc(c, fp);

    int id;
    char username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME];

    while (!feof(fp)) {
        fscanf(fp, "%d;%[^;];%[^;];%[^\n]\n", &id, &username, &password, &name);
        aux = insertManager(aux, id, username, password, name);
    }

    fclose(fp);

    return aux;
}