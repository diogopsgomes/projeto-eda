#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

void locationsMain() {
    Location* headLocations = readLocations();

    clrscr();

    listGraph(headLocations);

    enterToContinue();
}

Location* createLocation(Location* head, char id[], char name[]) {
    if (existLocation(head, id)) return head;

    Location* new = malloc(sizeof(struct location));

    if (new == NULL) return 0;

    strcpy(new->id, id);
    strcpy(new->name, name);
    new->adjacents = NULL;
    new->next = NULL;

    if (head == NULL) return new;

    Location* aux = head;

    while (aux->next != NULL) aux = aux->next;
    aux->next = new;

    return head;
}

Location* createEdge(Location* head, char origin[], char destination[], float distance) {
    if (!existLocation(head, origin) || !existLocation(head, destination)) return head;

    Location* aux = head;

    while (strcmp(head->id, origin) != 0) head = head->next;

    Adjacent* new = malloc(sizeof(struct adjacent));

    if (new == NULL) return aux;

    strcpy(new->id, destination);
    new->distance = distance;
    new->next = NULL;

    if (head->adjacents == NULL) {
        head->adjacents = new;
        return aux;
    }

    Adjacent* aux2 = head->adjacents;

    while (aux2->next != NULL) aux2 = aux2->next;
    aux2->next = new;

    return aux;
}

int existLocation(Location* head, char id[]) {
    while (head != NULL) {
        if (strcmp(head->id, id) == 0) return 1;

        head = head->next;
    }

    return 0;
}

char* getLocationName(Location* head, char id[]) {
    while (head != NULL) {
        if (strcmp(head->id, id) == 0) return head->name;

        head = head->next;
    }

    return "*********";
}

float getDistance(Location* head, char origin[], char destination[]) {
    if (!existLocation(head, origin) || !existLocation(head, destination)) return -1;
    if (strcmp(origin, destination) == 0) return 0;

    Location* l = head;
    while (strcmp(l->id, origin) != 0) l = l->next;

    Adjacent* a = l->adjacents;
    while (strcmp(a->id, destination) != 0) a = a->next;

    return a->distance;
}

void listAdjacents(Location* head, char id[]) {
    if (existLocation(head, id)) {
        while (strcmp(head->id, id) != 0) head = head->next;

        Adjacent* aux = head->adjacents;

        while (aux != NULL) {
            printf("Localizacao: %s | Distancia: %.3f km\n", aux->id, aux->distance);
            aux = aux->next;
        }
    }
}

void listGraph(Location* head) {
    Location* l = head;

    while (l != NULL) {
        printf("%s (%s)\n", l->name, l->id);

        Adjacent* a = l->adjacents;

        while (a != NULL) {
            char location[SIZE_LOCATION];
            strcpy(location, a->id);
            printf("|--> %-50s (%-30s -->    %.3f km\n", getLocationName(head, a->id), strcat(location, ")"), a->distance);
            a = a->next;
        }

        printf("\n");
        l = l->next;
    }
}

Location* readLocations() {
    Location* l = NULL;
    FILE* fp;

    fp = fopen(DATA_DIR"locations.txt", "r");
    if (fp == NULL) return l;

    char id[SIZE_LOCATION], name[SIZE_LOCATION];

    while (!feof(fp)) {
        fscanf(fp, "%[^;];%[^\n]\n", &id, &name);
        l = createLocation(l, id, name);
    }

    fclose(fp);

    fp = fopen(DATA_DIR"edges.txt", "r");
    if (fp == NULL) return l;

    float distance;
    char origin[SIZE_LOCATION], destination[SIZE_LOCATION];

    while (!feof(fp)) {
        fscanf(fp, "%[^;];%[^;];%f\n", &origin, &destination, &distance);
        l = createEdge(l, origin, destination, distance);
    }

    fclose(fp);

    return l;
}