#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void managersMain() {
    int option, id;
    char username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME];

    do {
        Manager* head = readManagers();

        clrscr();
        menuHeaderManagers();

        if (listManagers(head) == 0) puts("\n                                        Nao existem gestores registados!                                         \n");

        menuFooterManagers();
        scanf("%d", &option);

        switch (option) {
            case 1:
                clrscr();
                menuTitleInsertManager();

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

                printf("Nome de Utilizador: ");
                clrbuffer();
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;

                printf("Palavra-passe: ");
                clrbuffer();
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;
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
Manager* insertManager(Manager* head, int id, char username[], char password[], char name[]) {
    Manager *new = malloc(sizeof(struct manager)), *prev;

    if (new != NULL) {
        new->id = id;
        strcpy(new->username, username);
        strcpy(new->password, password);
        strcpy(new->name, name);
        new->next = NULL;
    }

    if (head == NULL) return new;

    for (prev = head; prev->next != NULL; prev = prev->next);

    prev->next = new;

	return head;
}

// Remove Manager by ID
Manager* removeManager(Manager* head, int id) {
    Manager *prev=head, *current=head, *aux;

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
void editManager(Manager* head, int id, char username[], char password[], char name[]) {
    Manager* current = head;

    while (current != NULL) {
        if (current->id == id) {
            if (strlen(username) > 0) strcpy(current->username, username);
            if (strlen(password) > 0) strcpy(current->password, password);
            if (strlen(name) > 0) strcpy(current->name, name);

            break;
        }

        current = current->next;
    }
}

// List Managers in Console
int listManagers(Manager* head) {
    if (head != NULL) {
        while (head != NULL) {
            printf("  %06d\t%s\t\t%s\t\n", head->id, head->name, head->username);

            head = head->next;
        }

        return 1;
    }

    return 0;
}

// Get Manager Name from Manager ID
char* getManagerName(Manager* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->name;

        head = head->next;
    }

    return "*********";
}

// Check if Manager ID exists
int existManager(Manager* head, int id) {
    while (head != NULL) {
        if (head->id == id) return 1;

        head = head->next;
    }

    return 0;
}

// Assign an ID to a Manager based on the last Manager in the list (+1)
int assignManagerId(Manager* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }
}

// Save Managers in File
int saveManagers(Manager* head) {
    FILE* fp;
    fp = fopen("managers.txt", "w");

    if (fp != NULL) {
        Manager* aux = head;

        while (aux != NULL) {
            fprintf(fp, "%d;%s;%s;%s\n", aux->id, aux->username, aux->password, aux->name);
            aux = aux->next;
        }

        fclose(fp);

        return 1;
    }
    
    return 0;
}

// Read Managers from File
Manager* readManagers() {
    FILE* fp;
    fp = fopen("managers.txt", "r");
    Manager* aux = NULL;

    if (fp != NULL) {
        int id;
        char username[SIZE_USERNAME], password[SIZE_PASSWORD], name[SIZE_NAME];

        while (!feof(fp)) { 
            fscanf(fp, "%d;%[^;];%[^;];%[^\n]\n", &id, &username, &password, &name);
            aux = insertManager(aux, id, username, password, name);
        }

        fclose(fp);
    }

    return aux;
}