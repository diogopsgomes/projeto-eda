#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/header.h"

void ridesMain() {
    Ride* head = readRides();
    Client* headClients = readClients();
    int count;

    clrscr();
    menuHeaderRides();
    count = listRides(head, headClients);
    puts("");
    showCount(count);
    puts("");
    enterToContinue();
}

// Insert Ride
/**
 * It inserts a new ride into the linked list of rides
 *
 * @param head The head of the linked list
 * @param id The id of the ride
 * @param vehicle The id of the vehicle
 * @param client The id of the client
 * @param startTime The start time of the ride
 * @param endTime The end time of the ride
 * @param startLocation The start location of the ride
 * @param endLocation The end location of the ride
 * @param cost The cost of the ride
 * @param distance The distance of the ride
 *
 * @return The head of the list.
 */
Ride* insertRide(Ride* head, int id, int vehicle, int client, int startTime, int endTime, char startLocation[], char endLocation[], float cost, float distance) {
    Ride* new = malloc(sizeof(struct ride)), * aux = head;

    if (new == NULL) return head;

    new->id = id;
    new->vehicle = vehicle;
    new->client = client;
    new->startTime = (startTime < 0) ? (time(NULL)) : (startTime);
    new->endTime = (endTime < 0) ? (-1) : (endTime);
    strcpy(new->startLocation, startLocation);
    strcpy(new->endLocation, endLocation);
    new->cost = (cost < 0) ? (-1) : (cost);
    new->distance = (distance < 0) ? (-1) : (distance);
    new->next = NULL;

    if (aux == NULL) return new;

    while (aux->next != NULL) aux = aux->next;

    aux->next = new;

    return head;
}

// Start New Ride
/**
 * It takes a ride, a vehicle, a type, a client, and an id, and returns a ride
 *
 * @param head The head of the linked list
 * @param headVehicles Pointer to the first vehicle in the linked list
 * @param headTypes Pointer to the first type of vehicle in the linked list
 * @param headClients Pointer to the first client in the linked list
 * @param id The id of the ride
 * @param vehicle The id of the vehicle
 * @param client The id of the client
 *
 * @return The head of the list.
 */
Ride* startRide(Ride* head, Vehicle* headVehicles, Type* headTypes, Client* headClients, int id, int vehicle, int client) {
    if (headVehicles == NULL || headTypes == NULL || headClients == NULL) return head;

    char startLocation[SIZE_LOCATION];

    while (headVehicles != NULL) {
        if (headVehicles->id == vehicle) {
            strcpy(startLocation, headVehicles->location);
            headVehicles->available = 0;
            break;
        }

        headVehicles = headVehicles->next;
    }

    while (headClients != NULL) {
        if (headClients->id == client) {
            headClients->available = 0;
            break;
        }

        headClients = headClients->next;
    }

    head = insertRide(head, assignRideId(head), vehicle, client, -1, -1, startLocation, "NULL", -1, -1);

    return head;
}

// End Ride
/**
 * It takes a ride, a vehicle, a type, a client, an id, and an end location, and then it sets the end
 * time, end location, cost, distance, and range of the ride
 *
 * @param head The head of the linked list
 * @param headVehicles Pointer to the first vehicle in the linked list
 * @param headTypes Pointer to the first type of vehicle in the linked list
 * @param headClients Pointer to the first client in the linked list
 * @param id The id of the ride
 * @param endLocation The end location of the ride
 */
void endRide(Ride* head, Vehicle* headVehicles, Type* headTypes, Client* headClients, Location* headLocations, int id, char endLocation[]) {
    if (headVehicles == NULL || headTypes == NULL || headClients == NULL) return;

    while (head != NULL) {
        if (head->id == id) {
            head->endTime = time(NULL);
            if (strlen(endLocation) > 0) strcpy(head->endLocation, endLocation);

            double timeElapsed = difftime(head->endTime, head->startTime);
            int minutesElapsed = (int)(timeElapsed / 60.0);
            float cost = getVehicleCost(headVehicles, headTypes, head->vehicle) * minutesElapsed;
            head->cost = cost;

            /* float distance = minutesElapsed * 0.4;
            head->distance = distance; */

            float distance = getDistance(headLocations, head->startLocation, endLocation);
            head->distance = distance;

            while (headVehicles != NULL) {
                if (headVehicles->id == head->vehicle) {
                    if (strlen(endLocation) > 0) strcpy(headVehicles->location, endLocation);
                    headVehicles->range = (headVehicles->range > distance) ? (headVehicles->range - distance) : (0.0F);
                    headVehicles->battery = headVehicles->range / 2.0F;
                    headVehicles->available = 1;
                    break;
                }

                headVehicles = headVehicles->next;
            }

            while (headClients != NULL) {
                if (headClients->id == head->client) {
                    headClients->balance -= cost;
                    headClients->available = 1;
                    break;
                }

                headClients = headClients->next;
            }

            break;
        }

        head = head->next;
    }
}

// List Rides in Console
/**
 * It prints the list of rides
 *
 * @param head The head of the linked list
 * @param headClients Pointer to the first client in the linked list
 *
 * @return The number of rides in the list.
 */
int listRides(Ride* head, Client* headClients) {
    int count = 0;
    char startTimeStr[SIZE_DATETIME], endTimeStr[SIZE_DATETIME];

    while (head != NULL) {
        strftime(startTimeStr, sizeof(startTimeStr), "%d/%m/%Y %H:%M", localtime(&head->startTime));
        strftime(endTimeStr, sizeof(endTimeStr), "%d/%m/%Y %H:%M", localtime(&head->endTime));

        printf("  %06d\t%06d\t\t%-23s\t%-16s\t%-16s\t%-23s\t%-23s\t%-7.3f\t\t%-7.2f\n", head->id, head->vehicle, getClientUsername(headClients, head->client), startTimeStr, endTimeStr, head->startLocation, head->endLocation, head->distance, head->cost);

        count++;

        head = head->next;
    }

    return count;
}

// List Rides of a Client in Console
/**
 * It prints out the rides of a client
 *
 * @param head The head of the linked list
 * @param headClients Pointer to the first node of the clients linked list
 * @param id The id of the client
 *
 * @return The number of rides that the client has.
 */
int listRidesClient(Ride* head, Client* headClients, int id) {
    int count = 0;
    char startTimeStr[SIZE_DATETIME], endTimeStr[SIZE_DATETIME];

    while (head != NULL) {
        if (head->client == id) {
            strftime(startTimeStr, sizeof(startTimeStr), "%d/%m/%Y %H:%M", localtime(&head->startTime));
            strftime(endTimeStr, sizeof(endTimeStr), "%d/%m/%Y %H:%M", localtime(&head->endTime));

            printf("  %06d\t%06d\t\t%-16s\t%-16s\t%-23s\t%-23s\t%-7.3f\t\t%-7.2f\n", head->id, head->vehicle, startTimeStr, endTimeStr, head->startLocation, head->endLocation, head->distance, head->cost);

            count++;
        }

        head = head->next;
    }

    return count;
}

// Assign an ID to a Ride based on the last Ride in the list (+1)
/**
 * It returns the next available ride id
 *
 * @param head The head of the linked list
 *
 * @return The next available ride id.
 */
int assignRideId(Ride* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }

    return 1;
}

// Returns the ID of the current Ride of a Client
/**
 * It returns the id of the ride that the client is currently on, or -1 if the client is not on a ride
 *
 * @param head The head of the linked list
 * @param id The id of the client
 *
 * @return The id of the ride that the client is currently on.
 */
int currentRide(Ride* head, int id) {
    while (head != NULL) {
        if (head->client == id && head->endTime == -1) return head->id;

        head = head->next;
    }

    return -1;
}

/**
 * It prints the information of a ride given its id
 *
 * @param head The head of the linked list
 * @param id The id of the ride
 */
void showRide(Ride* head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            char startTimeStr[SIZE_DATETIME], endTimeStr[SIZE_DATETIME];

            strftime(startTimeStr, sizeof(startTimeStr), "%d/%m/%Y %H:%M", localtime(&head->startTime));
            strftime(endTimeStr, sizeof(endTimeStr), "%d/%m/%Y %H:%M", localtime(&head->endTime));

            printf("Inicio: %s\n", startTimeStr);
            printf("Fim: %s\n", endTimeStr);
            printf("Total: %.2f\n", head->cost);
            printf("Partida: %s\n", head->startLocation);
            printf("Destino: %s\n", head->endLocation);
            printf("Distancia: %.3f\n", head->distance);

            break;
        }

        head = head->next;
    }
}

// Save Rides in File
/**
 * It saves the linked list of rides to a file
 *
 * @param head The head of the linked list
 *
 * @return 1 if the file was saved successfully, and 0 if it wasn't.
 */
int saveRides(Ride* head) {
    FILE* fp;
    fp = fopen(DATA_DIR"rides.txt", "w");

    if (fp == NULL) return 0;

    while (head != NULL) {
        fprintf(fp, "%d;%d;%d;%d;%d;%s;%s;%f;%f\n", head->id, head->vehicle, head->client, head->startTime, head->endTime, head->startLocation, head->endLocation, head->cost, head->distance);

        head = head->next;
    }

    fclose(fp);

    fp = fopen(DATA_DIR"rides.bin", "wb");

    if (fp != NULL) {
        while (head != NULL) {
            fwrite(head, sizeof(struct ride), 1, fp);

            head = head->next;
        }

        fclose(fp);
    }

    return 1;
}

// Read Rides from File
/**
 * It reads a file and inserts the data into a linked list
 *
 * @return A pointer to a Ride struct.
 */
Ride* readRides() {
    FILE* fp;
    fp = fopen(DATA_DIR"rides.txt", "r");
    Ride* aux = NULL;

    if (fp == NULL) return aux;

    int c = fgetc(fp);
    if (c == EOF) {
        fclose(fp);
        return aux;
    }
    ungetc(c, fp);

    int id, vehicle, client;
    int startTime, endTime;
    float cost, distance;
    char startLocation[SIZE_LOCATION], endLocation[SIZE_LOCATION];

    while (!feof(fp)) {
        fscanf(fp, "%d;%d;%d;%d;%d;%[^;];%[^;];%f;%f\n", &id, &vehicle, &client, &startTime, &endTime, &startLocation, &endLocation, &cost, &distance);
        aux = insertRide(aux, id, vehicle, client, startTime, endTime, startLocation, endLocation, cost, distance);
    }

    fclose(fp);

    return aux;
}