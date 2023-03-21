#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

/**
 * It takes a string, and adds a key to each character in the string. 
 * 
 * The key is 18445, but it's multiplied by 4 if the character is in an even position, and multiplied
 * by 2 if the character is in an odd position. 
 * 
 * The key is then added to the character. 
 * 
 * The result is stored in the same position in the string. 
 * 
 * The function returns nothing.
 * 
 * @param password The password to be encrypted.
 */
void encrypt(char password[]) {
    for (int i = 0; i < strlen(password); i++) {
        int key = 18445;

        (i % 2 == 0) ? (key *= 4) : (key *= 2);
        
        password[i] = (password[i] + key);
    }
}

/**
 * It takes a pointer to the head of a linked list of clients, a username and a password, encrypts the
 * password, and returns the id of the client if the username and password match, or 0 if they don't
 * 
 * @param head The head of the linked list
 * @param username "test"
 * @param password the password to be encrypted
 * 
 * @return The ID of the client.
 */
int authClient(Client* head, char username[], char password[]) {
    encrypt(password);

    while (head != NULL) {
        if (strcmp(head->username, username) == 0 && strcmp(head->password, password) == 0) return head->id;

        head = head->next;
    }

    return 0;
}

/**
 * It takes a pointer to a linked list of managers, a username and a password, encrypts the password,
 * and then compares the username and password to the username and password of each manager in the
 * linked list. If it finds a match, it returns the manager's ID. If it doesn't find a match, it
 * returns 0
 * 
 * @param head pointer to the first node of the linked list
 * @param username the username of the manager
 * @param password the password to be encrypted
 * 
 * @return The ID of the manager.
 */
int authManager(Manager* head, char username[], char password[]) {
    encrypt(password);

    while (head != NULL) {
        if (strcmp(head->username, username) == 0 && strcmp(head->password, password) == 0) return head->id;

        head = head->next;
    }

    return 0;
}