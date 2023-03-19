#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../inc/header.h"

void ridesMain() {
    
}

// Insert New Ride
Ride* insertRide(Ride* head, Vehicle* headVehicles, Type* headTypes, int id, int vehicle, int client, int startTime, int endTime, char startLocation[], char endLocation[], float cost, float distance) {
    Ride *new = malloc(sizeof(struct ride)), *aux = head;

    if (new == NULL) return head;

    if (cost == -1 && startTime >= 0 && endTime >= 0 && headVehicles != NULL && headTypes != NULL) {
        double timeElapsed = difftime(endTime, startTime);
        int minutesElapsed = (int) (timeElapsed / 60.0);
        cost = getVehicleCost(headVehicles, headTypes, vehicle) * minutesElapsed;
    }

    new->id = id;
    new->vehicle = vehicle;
    new->client = client;
    new->startTime = (startTime < 0) ? (time(NULL)) : (startTime);
    new->endTime = (endTime < 0) ? (-1) : (endTime);
    strcpy(new->startLocation, startLocation);
    strcpy(new->endLocation, endLocation);
    new->cost = cost;
    new->distance = (distance < 0) ? (-1) : (distance);
    new->next = NULL;

    if (aux == NULL) return new;

    while (aux->next != NULL) aux = aux->next;

    aux->next = new;

    return head;
}

// Start New Ride
Ride* startRide(Ride* head, Vehicle* headVehicles, Type* headTypes, Client* headClients, int id, int vehicle, int client) {
    if (headVehicles == NULL || headTypes == NULL || headClients == NULL) return head;
    
    char startLocation[SIZE_LOCATION];

    while (headVehicles != NULL) {
        if (headVehicles->id == vehicle) {
            strcpy(startLocation, headVehicles->location);
            break;
        }

        headVehicles = headVehicles->next;
    }

    head = insertRide(head, headVehicles, headTypes, assignRideId(head), vehicle, client, -1, -1, startLocation, "NULL", -1, -1);

    while (headVehicles != NULL) {
        if (headVehicles->id == vehicle) {
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

    return head;
}

// End Ride
void endRide(Ride* head, Vehicle* headVehicles, Type* headTypes, Client* headClients, int id, char endLocation[]) {
    if (headVehicles == NULL || headTypes == NULL || headClients == NULL) return;

    while (head != NULL) {
        if (head->id == id) {
            head->endTime = time(NULL);
            if (strlen(endLocation) > 0) strcpy(head->endLocation, endLocation);

            double timeElapsed = difftime(head->endTime, head->startTime);
            int minutesElapsed = (int) (timeElapsed / 60.0);
            float cost = getVehicleCost(headVehicles, headTypes, head->vehicle) * minutesElapsed;
            head->cost = cost;

            float distance = minutesElapsed * 0.4;
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

// Assign an ID to a Ride based on the last Ride in the list (+1)
int assignRideId(Ride* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }

    return 1;
}

// Returns the ID of the current Ride of a Client
int currentRide(Ride* head, int id) {
    while (head != NULL) {
        if (head->client == id && head->endTime == -1) return head->id;

        head = head->next;
    }

    return -1;
}

void showRide(Ride* head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            printf("Inicio: %d\n", head->startTime);
            printf("Fim: %d\n", head->endTime);
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
int saveRides(Ride* head) {
    FILE* fp;
    fp = fopen(DATA_DIR"rides.txt", "w");

    if (fp == NULL) return 0;

    while (head != NULL) {
        fprintf(fp, "%d;%d;%d;%d;%d;%s;%s;%f;%f\n", head->id, head->vehicle, head->client, head->startTime, head->endTime, head->startLocation, head->endLocation, head->cost, head->distance);
        head = head->next;
    }

    fclose(fp);

    return 1;
}

// Read Rides from File
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
        aux = insertRide(aux, NULL, NULL, id, vehicle, client, startTime, endTime, startLocation, endLocation, cost, distance);
    }

    fclose(fp);

    return aux;
}