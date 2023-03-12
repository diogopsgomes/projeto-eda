#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Get Manager Name from Manager ID
char* getManagerName(Manager* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->name;

        head = head->next;
    }

    return "*********";
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

int authManager(Manager* head, char username[], char password[]) {
    while (head != NULL) {
        if (strcmp(head->username, username) == 0 && strcmp(head->password, password) == 0) return head->id;

        head = head->next;
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