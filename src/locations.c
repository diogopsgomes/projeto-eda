#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

Vertice* createVertice(Vertice* head, char id[], char name[]) {
    if (existVertice(head, id)) return head;

    Vertice* new = malloc(sizeof(struct vertice));

    if (new == NULL) return 0;

    strcpy(new->id, id);
    strcpy(new->name, name);
    new->adjacents = NULL;
    new->next = NULL;

    if (head == NULL) return new;

    Vertice* aux = head;

    while (aux->next != NULL) aux = aux->next;
    aux->next = new;

    return head;
}

int existVertice(Vertice* head, char id[]) {
    while (head != NULL) {
        if (strcmp(head->id, id) == 0) return 1;

        head = head->next;
    }

    return 0;
}

char* getVerticeName(Vertice* head, char id[]) {
    while (head != NULL) {
        if (strcmp(head->id, id) == 0) return head->name;

        head = head->next;
    }

    return "*********";
}

float getDistance(Vertice* head, char origin[], char destination[]) {
    if (!existVertice(head, origin) || !existVertice(head, destination)) return -1;
    if (strcmp(origin, destination) == 0) return 0;

    Vertice* v = head;
    while (strcmp(v->id, origin) != 0) v = v->next;

    Adjacent* a = v->adjacents;
    while (strcmp(a->id, destination) != 0) a = a->next;

    return a->distance;
}

Vertice* createEdge(Vertice* head, char origin[], char destination[], float distance) {
    if (!existVertice(head, origin) || !existVertice(head, destination)) return head;

    Vertice* aux = head;

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

void listAdjacents(Vertice* head, char id[]) {
    if (existVertice(head, id)) {
        while (strcmp(head->id, id) != 0) head = head->next;

        Adjacent* aux = head->adjacents;

        while (aux != NULL) {
            printf("Localizacao: %s | Distancia: %.3f km\n", aux->id, aux->distance);
            aux = aux->next;
        }
    }
}

Vertice* createGraph() {
    Vertice* v = NULL;
    FILE* fp;

    fp = fopen(DATA_DIR"vertices.txt", "r");
    if (fp == NULL) return v;

    char id[SIZE_LOCATION], name[SIZE_LOCATION];

    while (!feof(fp)) {
        fscanf(fp, "%[^;];%[^\n]\n", &id, &name);
        v = createVertice(v, id, name);
    }

    fclose(fp);

    fp = fopen(DATA_DIR"edges.txt", "r");
    if (fp == NULL) return v;

    float distance;
    char origin[SIZE_LOCATION], destination[SIZE_LOCATION];

    while (!feof(fp)) {
        fscanf(fp, "%[^;];%[^;];%f\n", &origin, &destination, &distance);
        v = createEdge(v, origin, destination, distance);
    }

    fclose(fp);

    return v;
}

void listGraph(Vertice* head) {
    Vertice* v = head;

    while (v != NULL) {
        printf("%s (%s)\n", v->name, v->id);

        Adjacent* a = v->adjacents;

        while (a != NULL) {
            char location[SIZE_LOCATION];
            strcpy(location, a->id);
            printf("|--> %-50s (%-30s -->    %.3f km\n", getVerticeName(head, a->id), strcat(location, ")"), a->distance);
            a = a->next;
        }

        printf("\n");
        v = v->next;
    }
}