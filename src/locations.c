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

/**
 * The function creates a new location and adds it to the linked list of locations if it does not
 * already exist.
 *
 * @param head A pointer to the head of a linked list of Location structs.
 * @param id A character array representing the unique identifier of the location being created.
 * @param name The name of the location that we want to create.
 *
 * @return a pointer to the head of the linked list of locations.
 */
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

/**
 * The function creates an edge between two locations in a graph data structure.
 *
 * @param head A pointer to the head of the linked list of locations.
 * @param origin A string representing the ID of the origin location for the edge being created.
 * @param destination The name or identifier of the location that is being connected to the origin
 * location by the new edge.
 * @param distance distance is a float variable that represents the distance between two locations in a
 * graph. It is used in the function createEdge() to create a new adjacent node between two existing
 * locations.
 *
 * @return a pointer to the head of the Location linked list.
 */
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

/**
 * The function checks if a given location ID exists in a linked list of locations.
 *
 * @param head a pointer to the head of a linked list of Location structs
 * @param id The parameter "id" is a character array that represents the ID of a location. It is used
 * to search for a location in a linked list of locations.
 *
 * @return The function `existLocation` returns an integer value of 1 if a location with the given `id`
 * exists in the linked list starting from the `head` node, and 0 otherwise.
 */
int existLocation(Location* head, char id[]) {
    while (head != NULL) {
        if (strcmp(head->id, id) == 0) return 1;

        head = head->next;
    }

    return 0;
}

/**
 * The function returns the name of a location given its ID, or "*********" if the ID is not found.
 *
 * @param head a pointer to the head of a linked list of Location structs
 * @param id The id parameter is a character array that represents the unique identifier of a location.
 *
 * @return If a location with the given id is found in the linked list, its name is returned as a
 * character pointer. If no location with the given id is found, the string "*********" is returned.
 */
char* getLocationName(Location* head, char id[]) {
    while (head != NULL) {
        if (strcmp(head->id, id) == 0) return head->name;

        head = head->next;
    }

    return "*********";
}

/**
 * The function calculates the distance between two locations in a graph.
 *
 * @param head a pointer to the head of a linked list of Location structs
 * @param origin A string representing the ID of the starting location.
 * @param destination The destination parameter is a character array that represents the ID of the
 * location to which the distance is being calculated.
 *
 * @return a float value which represents the distance between two locations. If either the origin or
 * destination location does not exist in the linked list of locations, the function returns -1. If the
 * origin and destination are the same location, the function returns 0.
 */
float getDistance(Location* head, char origin[], char destination[]) {
    if (!existLocation(head, origin) || !existLocation(head, destination)) return -1;
    if (strcmp(origin, destination) == 0) return 0;

    Location* l = head;
    while (strcmp(l->id, origin) != 0) l = l->next;

    Adjacent* a = l->adjacents;
    while (strcmp(a->id, destination) != 0) a = a->next;

    return a->distance;
}

/**
 * The function lists the adjacent locations and their distances from a given location.
 *
 * @param head a pointer to the head of a linked list of Location structs
 * @param id The id parameter is a string that represents the id of a location for which we want to
 * list its adjacent locations.
 */
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

/**
 * The function prints out a list of locations and their adjacent locations with their respective
 * distances.
 *
 * @param head The head pointer of a linked list of Location structs.
 */
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

/**
 * The function reads location and edge data from text files and creates a linked list of locations
 * with edges between them.
 *
 * @return a pointer to a Location struct.
 */
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