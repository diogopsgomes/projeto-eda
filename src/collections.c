#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/header.h"

void collectionsMain(int manager) {
    int option, count;
    char location[SIZE_LOCATION];
    Collection* head = NULL;

    do {
        //Collection* head = loadCollections();
        Vehicle* headVehicles = readVehicles();
        Type* headTypes = readTypes();
        Location* headLocations = readLocations();

        clrscr();
        menuHeaderVehicles();

        if ((count = listVehiclesByBatteryHalfCharged(headVehicles, headTypes, headLocations, HQ)) == 0) {
            puts("\n                                                   Nao existem veiculos para recolha!                                                    \n");
        } else {
            puts("");
            showCount(count);
        }

        menuFooterCollections();
        scanf("%d", &option);

        switch (option) {
            case 1:
                menuLine();
                printf("Partida (Em branco - Sede): ");
                clrbuffer();
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = 0;
                if (strlen(location) <= 0) strcpy(location, HQ);

                head = collect(head, headVehicles, headLocations, location, manager);

                clrscr();

                listLatestCollection(head, headVehicles, headTypes, headLocations);

                saveCollections(head);

                enterToContinue();

                break;
            case 2:
                menuLine();
                printf("Localizacao (Em branco - Sede): ");
                clrbuffer();
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = 0;
                if (strlen(location) <= 0) strcpy(location, HQ);

                headVehicles = chargeVehicles(headVehicles, HQ);
                saveVehicles(headVehicles);
            default:
                break;
        }

    } while (option != 0);
}

/**
 * The function collects data from a given starting location by visiting adjacent locations using
 * vehicles and saves the collected data.
 *
 * @param head A pointer to the head of a linked list of Collection structs.
 * @param headVehicles A pointer to the head of a linked list of Vehicle structs.
 * @param headLocations A pointer to the head of a linked list of Location structs, representing all
 * the locations in the system.
 * @param startLocation The starting location for the collection route.
 * @param manager An integer representing the ID of the manager responsible for the collection.
 *
 * @return a pointer to a Collection, which is the updated head of the linked list of collections.
 */
Collection* collect(Collection* head, Vehicle* headVehicles, Location* headLocations, char startLocation[], int manager) {
    Vehicle* v;
    Adjacent* a;
    int visit = 0;
    float lowest;

    Location* l = headLocations;
    while (l != NULL) {
        visit++;

        l = l->next;
    }
    if (visit <= 1) return head;

    int id = assignCollectionId(head);
    head = insertCollection(head, id, startLocation, time(NULL), manager);

    Visited* visited = NULL;
    visited = insertVisited(visited, startLocation);

    char location[SIZE_LOCATION], nearest[SIZE_LOCATION];
    strcpy(location, startLocation);
    strcpy(nearest, location);

    while (visit != 0) {
        v = headVehicles;
        l = headLocations;

        while (strcmp(l->id, location) != 0) l = l->next;

        a = l->adjacents;
        while (a != NULL) {
            if (isVisited(visited, a->id) == 0) {
                strcpy(nearest, a->id);
                lowest = a->distance;
            }

            a = a->next;
        }

        if (visit == 1) {
            strcpy(nearest, startLocation);
            break;
        }

        a = l->adjacents;
        while (a->next != NULL) {
            if ((a->next->distance < lowest) && (isVisited(visited, a->next->id) == 0)) {
                strcpy(nearest, a->next->id);
                lowest = a->next->distance;
            }

            a = a->next;
        }

        strcpy(location, nearest);

        visited = insertVisited(visited, location);
        visit--;

        head = insertPoint(head, id, location);

        while (v != NULL) {
            if ((strcmp(v->location, location) == 0) && (v->battery < 50)) {
                head = insertCollected(head, id, location, v->id);
                strcpy(v->location, startLocation);
            }

            v = v->next;
        }
    }

    saveVehicles(headVehicles);
    return head;
}

/**
 * The function inserts a new collection into a linked list of collections.
 *
 * @param head A pointer to the head of a linked list of Collection structs.
 * @param id an integer representing the unique identifier of the collection
 * @param startLocation A character array that represents the starting location of the collection.
 * @param datetime The datetime parameter is a variable of type time_t, which represents the date and
 * time of the collection. It is likely stored as a Unix timestamp, which is the number of seconds that
 * have elapsed since January 1, 1970, 00:00:00 UTC.
 * @param manager The parameter "manager" is an integer that represents the ID of the user who is
 * managing the collection.
 *
 * @return a pointer to the head of the linked list of collections.
 */
Collection* insertCollection(Collection* head, int id, char startLocation[], time_t datetime, int manager) {
    Collection* new = malloc(sizeof(struct collection));

    if (new == NULL) return head;

    new->id = id;
    strcpy(new->startLocation, startLocation);
    new->datetime = datetime;
    new->manager = manager;
    new->points = NULL;
    new->next = NULL;

    if (head == NULL) return new;

    Collection* aux = head;

    while (aux->next != NULL) aux = aux->next;
    aux->next = new;

    return head;
}

/**
 * The function inserts a new point into a collection with a given ID and location.
 *
 * @param head A pointer to the head of a linked list of Collection structs.
 * @param id The ID of the collection where the new point will be inserted.
 * @param location The location parameter is a string that represents the ID or name of a point that is
 * being inserted into a collection.
 *
 * @return a pointer to a Collection, which is the head of the linked list.
 */
Collection* insertPoint(Collection* head, int id, char location[]) {
    Collection* aux = head;

    while (aux != NULL && aux->id != id) aux = aux->next;

    if (aux == NULL) return head;

    Point* new = malloc(sizeof(struct point));

    if (new == NULL) return head;

    strcpy(new->id, location);
    new->collected = NULL;
    new->next = NULL;

    if (aux->points == NULL) {
        aux->points = new;
        return head;
    }

    Point* aux2 = aux->points;

    while (aux2->next != NULL) aux2 = aux2->next;
    aux2->next = new;

    return head;
}

/**
 * The function inserts a new vehicle ID into a specific location's collection within a given
 * collection list.
 *
 * @param head A pointer to the head of a linked list of Collection structs.
 * @param id The ID of the collection to which the new collected item will be added.
 * @param location A string representing the location where an item was collected.
 * @param vehicle The parameter "vehicle" is an integer representing the ID of the vehicle that
 * collected the item at the specified location.
 *
 * @return a pointer to the head of the Collection linked list.
 */
Collection* insertCollected(Collection* head, int id, char location[], int vehicle) {
    Collection* aux = head;

    while (aux != NULL && aux->id != id) aux = aux->next;

    if (aux == NULL) return head;

    Point* aux2 = aux->points;

    while (aux2 != NULL && strcmp(aux2->id, location) != 0) aux2 = aux2->next;

    if (aux2 == NULL) return head;

    Integer* new2 = malloc(sizeof(struct integer));

    if (new2 == NULL) return head;

    new2->id = vehicle;
    new2->next = NULL;

    if (aux2->collected == NULL) {
        aux2->collected = new2;
        return head;
    }

    Integer* aux3 = aux2->collected;

    while (aux3->next != NULL) aux3 = aux3->next;
    aux3->next = new2;

    return head;
}

/**
 * The function inserts a new visited location into a linked list.
 *
 * @param head a pointer to the head of a linked list of Visited nodes.
 * @param location a string representing the ID of a location that the user has visited.
 *
 * @return a pointer to the head of the linked list of visited locations.
 */
Visited* insertVisited(Visited* head, char location[]) {
    Visited* new = malloc(sizeof(struct visited));

    if (new == NULL) return head;

    strcpy(new->id, location);
    new->next = NULL;

    if (head == NULL) return new;

    Visited* aux = head;

    while (aux->next != NULL) aux = aux->next;
    aux->next = new;

    return head;
}

/**
 * The function lists the collections, their start location, date and time, and the collected vehicles
 * at each collection point.
 *
 * @param head A pointer to the head of a linked list of Collection structs.
 * @param headVehicles A pointer to the head of a linked list of Vehicle structs.
 * @param headTypes A pointer to the head of a linked list of Type structs.
 *
 * @return The function does not return anything, it only prints information about the collections and
 * their points of collection.
 */
void listCollections(Collection* head, Vehicle* headVehicles, Type* headTypes) {
    if (head == NULL) {
        printf("Nao foi possivel obter os dados das recolhas.\n");
        return;
    }

    Collection* currentCollection = head;

    while (currentCollection != NULL) {
        menuLine();
        printf("Local de Partida: %s\n", currentCollection->startLocation);
        printf("Data e Hora: %s", ctime(&currentCollection->datetime));

        Point* currentPoint = currentCollection->points;

        printf("Pontos de Recolha:\n");

        while (currentPoint != NULL) {
            printf("|--> %s\n", currentPoint->id);

            Integer* currentCollected = currentPoint->collected;

            while (currentCollected != NULL) {
                printf("     |--> %d | %s | %.1f%%\n", currentCollected->id, getVehicleTypeName(headVehicles, headTypes, currentCollected->id), getVehicleBattery(headVehicles, currentCollected->id));

                currentCollected = currentCollected->next;
            }

            currentPoint = currentPoint->next;
        }

        currentCollection = currentCollection->next;
    }
}

/**
 * The function lists the details of the latest collection, including the start location, date and
 * time, and the collected vehicles at each point of collection.
 *
 * @param head a pointer to the head of a linked list of Collection structs
 * @param headVehicles A pointer to the head of a linked list of Vehicle structs.
 * @param headTypes A pointer to the head of a linked list of Type structs, which contain information
 * about the types of vehicles available in the system.
 *
 * @return The function does not return anything, it only prints information about the latest
 * collection.
 */
void listLatestCollection(Collection* head, Vehicle* headVehicles, Type* headTypes, Location* headLocations) {
    if (head == NULL) {
        printf("Nao foi possivel obter os dados da ultima recolha.\n");
        return;
    }

    float count = 0, distance;

    Collection* latest = head;

    while (latest->next != NULL) latest = latest->next;

    char lastLocation[SIZE_LOCATION];
    strcpy(lastLocation, latest->startLocation);

    puts("Ultima Recolha:\n");
    printf("Local de Partida: %s\n", latest->startLocation);
    printf("Data e Hora: %s", ctime(&latest->datetime));

    Point* currentPoint = latest->points;

    printf("Pontos de Recolha:\n");

    while (currentPoint != NULL) {
        printf("|--> %s (%.3f km)\n", currentPoint->id, distance = getDistance(headLocations, lastLocation, currentPoint->id));

        count += distance;

        Integer* currentCollected = currentPoint->collected;

        while (currentCollected != NULL) {
            printf("     |--> %d | %s | %.1f%%\n", currentCollected->id, getVehicleTypeName(headVehicles, headTypes, currentCollected->id), getVehicleBattery(headVehicles, currentCollected->id));

            currentCollected = currentCollected->next;
        }

        strcpy(lastLocation, currentPoint->id);

        currentPoint = currentPoint->next;
    }

    printf("Distancia Percorrida: %.3f km\n\n", count);
}

/**
 * This function assigns a unique ID to a new element in a linked list.
 *
 * @param head A pointer to the head of a linked list of Collection structs.
 *
 * @return an integer value, which is the next available ID for a new collection.
 */
int assignCollectionId(Collection* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }

    return 1;
}

/**
 * The function checks if a given location has been visited before by searching through a linked list
 * of visited locations.
 *
 * @param head a pointer to the head of a linked list of Visited nodes
 * @param location A character array representing the ID of a location that we want to check if it has
 * been visited before.
 *
 * @return an integer value of either 1 or 0. The value 1 indicates that the location passed as an
 * argument has been visited before and is present in the linked list pointed to by the head parameter.
 * The value 0 indicates that the location has not been visited before and is not present in the linked
 * list.
 */
int isVisited(Visited* head, char location[]) {
    while (head != NULL) {
        if (strcmp(head->id, location) == 0) return 1;

        head = head->next;
    }

    return 0;
}

/**
 * The function saves a linked list of collections, along with their points and collected integers, to
 * a binary file.
 *
 * @param head A pointer to the head of a linked list of Collection structs.
 *
 * @return an integer value. If the file "collections.bin" is successfully opened for writing, the
 * function returns 1. If the file cannot be opened, the function returns 0.
 */
int saveCollections(Collection* head) {
    FILE* fp = fopen(DATA_DIR"collections.bin", "wb");
    if (fp == NULL) return 0;

    Collection* currentCollection = head;

    while (currentCollection != NULL) {
        fwrite(currentCollection, sizeof(struct collection), 1, fp);

        Point* currentPoint = currentCollection->points;
        while (currentPoint != NULL) {
            fwrite(currentPoint, sizeof(struct point), 1, fp);

            Integer* currentCollected = currentPoint->collected;
            while (currentCollected != NULL) {
                fwrite(currentCollected, sizeof(struct integer), 1, fp);
                currentCollected = currentCollected->next;
            }

            currentPoint = currentPoint->next;
        }

        currentCollection = currentCollection->next;
    }

    fclose(fp);

    return 1;
}

/**
 * This function loads collections from a binary file and returns a pointer to the head of the linked
 * list.
 *
 * @return a pointer to the head of a linked list of Collection structs.
 */
Collection* loadCollections() {
    FILE* fp = fopen(DATA_DIR"collections.bin", "rb");
    Collection* head = NULL;

    if (fp == NULL) return head;

    while (!feof(fp)) {
        Collection* newCollection = malloc(sizeof(struct collection));
        if (newCollection == NULL) return head;
        fread(newCollection, sizeof(struct collection), 1, fp);

        Point* previousPoint = NULL;
        while (!feof(fp)) {
            Point* newPoint = malloc(sizeof(struct point));
            if (newPoint == NULL) return head;
            fread(newPoint, sizeof(struct point), 1, fp);

            Integer* previousCollected = NULL;
            while (!feof(fp)) {
                Integer* newCollected = malloc(sizeof(struct integer));
                if (newCollected == NULL) return head;
                fread(newCollected, sizeof(struct integer), 1, fp);

                if (previousCollected == NULL) {
                    newPoint->collected = newCollected;
                } else {
                    previousCollected->next = newCollected;
                }
                previousCollected = newCollected;
            }

            if (previousPoint == NULL) {
                newCollection->points = newPoint;
            } else {
                previousPoint->next = newPoint;
            }
            previousPoint = newPoint;
        }

        if (head == NULL) head = newCollection;
        else {
            Collection* aux = head;
            while (aux->next != NULL) aux = aux->next;
            aux->next = newCollection;
        }
    }

    fclose(fp);

    return head;
}