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
    printf("%s;%s\n", head->id, id);
    if (existVertice(head, id)) {
        printf("%d\n", strcmp(head->id, id));
        while (strcmp(head->id, id) != 0) {
            head = head->next;
        }
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
        printf("%s;%s\n", id, name);
        v = createVertice(v, id, name);
    }

    fclose(fp);

    fp = fopen(DATA_DIR"edges.txt", "r");
    if (fp == NULL) return v;

    float distance;
    char origin[SIZE_LOCATION], destination[SIZE_LOCATION];

    while (!feof(fp)) {
        fscanf(fp, "%[^;];%[^;];%f\n", &origin, &destination, &distance);
        printf("%s;%s;%f\n", origin, destination, distance);
        v = createEdge(v, origin, destination, distance);
    }

    fclose(fp);

    return v;
}