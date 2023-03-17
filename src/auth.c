#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

void encrypt(char password[]) {
    for (int i = 0; i < strlen(password); i++) {
        int key = 18445;

        (i % 2 == 0) ? (key *= 4) : (key *= 2);
        
        password[i] = (password[i] + key);
    }
}

int authClient(Client* head, char username[], char password[]) {
    encrypt(password);

    while (head != NULL) {
        if (strcmp(head->username, username) == 0 && strcmp(head->password, password) == 0) return head->id;

        head = head->next;
    }

    return 0;
}

int authManager(Manager* head, char username[], char password[]) {
    encrypt(password);

    while (head != NULL) {
        if (strcmp(head->username, username) == 0 && strcmp(head->password, password) == 0) return head->id;

        head = head->next;
    }

    return 0;
}