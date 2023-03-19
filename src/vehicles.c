#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

void vehiclesMain() {
    int option, id, type, count;
    float battery, range;
    char location[SIZE_LOCATION], typeStr[SIZE_TYPE], batteryStr[SIZE_BATTERY], rangeStr[SIZE_RANGE];

    do {
        Vehicle* head = readVehicles();
        Type* headTypes = readTypes();

        clrscr();
        menuHeaderVehicles();

        if ((count = listVehicles(head, headTypes)) == 0) {
            puts("\n                                                    Nao existem veiculos registados!                                                     \n");
        } else {
            puts("");
            showCount(count);
        }

        menuFooterVehicles();
        scanf("%d", &option);

        switch (option) {
            case 1:
                clrscr();
                menuTitleInsertVehicle();

                printf("Tipo: ");
                scanf("%d", &type);

                printf("Bateria: ");
                scanf("%f", &battery);

                printf("Autonomia: ");
                scanf("%f", &range);
                
                printf("Localizacao: ");
                clrbuffer();
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = 0;

                head = insertVehicle(head, assignVehicleId(head), type, battery, range, 1, location);
                saveVehicles(head);

                break;
            case 2:
                menuTitleEditVehicle();
                scanf("%d", &id);

                if (!existVehicle(head, id)) break;

                type = battery = range = -1;

                printf("Tipo: ");
                clrbuffer();
                fgets(typeStr, sizeof(typeStr), stdin);
                typeStr[strcspn(typeStr, "\n")] = 0;
                if (strlen(typeStr) > 0) type = atoi(typeStr);

                printf("Bateria: ");
                clrbuffer();
                fgets(batteryStr, sizeof(batteryStr), stdin);
                batteryStr[strcspn(batteryStr, "\n")] = 0;
                if (strlen(batteryStr) > 0) battery = atof(batteryStr);

                printf("Autonomia: ");
                clrbuffer();
                fgets(rangeStr, sizeof(rangeStr), stdin);
                rangeStr[strcspn(rangeStr, "\n")] = 0;
                if (strlen(rangeStr) > 0) range = atof(rangeStr);

                printf("Localizacao: ");
                clrbuffer();
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = 0;
                
                editVehicle(head, headTypes, id, type, battery, range, location);
                saveVehicles(head);

                break;
            case 3:
                menuTitleRemoveVehicle();
                scanf("%d", &id);

                head = removeVehicle(head, id);
                saveVehicles(head);

                break;
            case 4:
                clrscr();
                menuHeaderVehicles();
                count = listVehiclesByRange(head, headTypes);
                puts("");
                showCount(count);
                puts("");
                enterToContinue();

                break;
            case 5:
                menuTitleListVehiclesByLocation();
                clrbuffer();
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = 0;

                clrscr();
                menuHeaderVehicles();

                if ((count = listVehiclesByLocation(head, headTypes, location)) == 0) {
                    puts("\n                                                 Nao existem veiculos nessa localizacao!                                                 \n");
                } else {
                    puts("");
                    showCount(count);
                }

                puts("");
                enterToContinue();

                break;
            default:
                break;
        }

    } while (option != 0);
}

// Insert New Vehicle
Vehicle* insertVehicle(Vehicle* head, int id, int type, float battery, float range, int available, char location[]) {
    Vehicle *new = malloc(sizeof(struct vehicle)), *aux = head;

    if (new != NULL) {
        new->id = id;
        new->type = type;
        new->battery = battery;
        new->range = range;
        new->available = available;
        strcpy(new->location, location);
        new->next = NULL;
    }

    if (head == NULL) return new;

    while (aux->next != NULL) aux = aux->next;

    aux->next = new;

	return head;
}

// Remove Vehicle by ID
Vehicle* removeVehicle(Vehicle* head, int id) {
    Vehicle *prev=head, *current=head, *aux;

    if (current == NULL) {
        return NULL;
    } else if (current->id == id) {
        aux = current->next;
        free(current);

        return aux;
    } else {
        while ((current != NULL) && (current->id != id)) {
            prev = current;
            current = current->next;
        }
        if (current == NULL) {
            return head;
        } else {
            prev->next = current->next;
            free(current);

            return head;
        }
    }
}

// Edit Vehicle
void editVehicle(Vehicle* head, Type* headTypes, int id, int type, float battery, float range, char location[]) {
    while (head != NULL) {
        if (head->id == id) {
            if (type >= 0 && existType(headTypes, type)) head->type = type;
            if (battery >= 0) head->battery = battery;
            if (range >= 0) head->range = range;
            if (strlen(location) > 0) strcpy(head->location, location);

            break;
        }

        head = head->next;
    }
}

// List Vehicles in Console
int listVehicles(Vehicle* head, Type* headTypes) {
    int count = 0;
    char available[5];

    while (head != NULL) {
        (head->available == 1) ? (strcpy(available, "Sim")) : (strcpy(available, "Nao"));

        printf("  %06d\t%-25s\t%-5.1f\t\t\t%-7.3f\t\t\t%-5s\t\t%s\n", head->id, getTypeName(headTypes, head->type), head->battery, head->range, available, head->location);

        count++;

        head = head->next;
    }

    return count;
}

// List Vehicles in Console Ordered by Range (Descending)
int listVehiclesByRange(Vehicle* head, Type* headTypes) {
    int swapped;

    if (head != NULL) {
        do {
            Vehicle* prev = NULL;
            Vehicle* current = head;
            swapped = 0;

            while (current->next != NULL) {
                if (current->range < current->next->range) {
                    Vehicle* next = current->next;
                    current->next = next->next;
                    next->next = current;

                    if (prev != NULL) {
                        prev->next = next;
                    } else {
                        head = next;
                    }

                    prev = next;
                    swapped = 1;
                } else {
                    prev = current;
                    current = current->next;
                }
            }
        } while (swapped);
    }

    return listVehicles(head, headTypes);
}

int listVehiclesByLocation(Vehicle* head, Type* headTypes, char location[]) {
    Vehicle* filtered = NULL;

    while (head != NULL) {
        if (strcmp(head->location, location) == 0) {
            filtered = insertVehicle(filtered, head->id, head->type, head->battery, head->range, head->available, head->location);
        }

        head = head->next;
    }
    
    return listVehiclesByRange(filtered, headTypes);
}

// Check if Vehicle ID exists
int existVehicle(Vehicle* head, int id) {
    while (head != NULL) {
        if (head->id == id) return 1;

        head = head->next;
    }

    return 0;
}

// Assign an ID to a Vehicle based on the last Vehicle in the list (+1)
int assignVehicleId(Vehicle* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }

    return 1;
}

// Check if a Vehicle is not in a Ride
int isVehicleAvailable(Vehicle* head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            if (head->available == 1) {
                return 1;
            }
        }

        head = head->next;
    }

    return 0;
}

// Check if a Vehicle is not out of Battery
int isVehicleCharged(Vehicle* head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            if (head->battery > 0 && head->range > 0) {
                return 1;
            }
        }

        head = head->next;
    }

    return 0;
}

// Copy linked list
Vehicle* copyLinkedList(Vehicle* head) {
    Vehicle* copy = NULL;
    while (head != NULL) {
        copy = insertVehicle(copy, head->id, head->type, head->battery, head->range, head->available, head->location);
        head = head->next;
    }
    return copy;
}

// Save Vehicles in File
int saveVehicles(Vehicle* head) {
    FILE* fp;
    fp = fopen(DATA_DIR"vehicles.txt", "w");

    if (fp == NULL) return 0;

    while (head != NULL) {
        fprintf(fp, "%d;%d;%f;%f;%d;%s\n", head->id, head->type, head->battery, head->range, head->available, head->location);
        head = head->next;
    }

    fclose(fp);

    return 1;
}

// Read Vehicles from File
Vehicle* readVehicles() {
    FILE* fp;
    fp = fopen(DATA_DIR"vehicles.txt", "r");
    Vehicle* aux = NULL;

    if (fp == NULL) return aux;
    
    int c = fgetc(fp);
    if (c == EOF) {
        fclose(fp);
        return aux;
    }
    ungetc(c, fp);

    int id, type, available;
    float battery, range;
    char location[SIZE_LOCATION];

    while (!feof(fp)) {
        fscanf(fp, "%d;%d;%f;%f;%d;%s\n", &id, &type, &battery, &range, &available, &location);
        aux = insertVehicle(aux, id, type, battery, range, available, location);
    }

    fclose(fp);

    return aux;
}

// Get Type Cost from Type ID
float getVehicleCost(Vehicle* head, Type* headTypes, int id) {
    while (head != NULL) {
        if (head->id == id) return getTypeCost(headTypes, head->type);

        head = head->next;
    }

    return -1;
}

// Get Type Cost from Type ID
float getTypeCost(Type* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->cost;

        head = head->next;
    }

    return -1;
}

// Get Type Name from Type ID
char* getTypeName(Type* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->name;

        head = head->next;
    }

    return "*********";
}

// Insert Type of Vehicle
Type* insertType(Type* head, int id, char name[], float cost) {
    Type *new = malloc(sizeof(struct type)), *prev;

    if (new != NULL) {
        new->id = id;
        strcpy(new->name, name);
        new->cost = cost;
        new->next = head;
    }

    if (head == NULL) return new;

    for (prev = head; prev->next != NULL; prev = prev->next);

    prev->next = new;

	return head;
}

// List Type of Vehicles in Console
int listTypes(Type* head) {
    if (head != NULL) {
        while (head != NULL) {
            printf("  %06d\t%s\t\t\t%.2f\t\n", head->id, head->name, head->cost);
            head = head->next;
        }

        return 1;
    }

    return 0;
}

// Check if Type ID exists
int existType(Type* head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            return 1;
        }

        head = head->next;
    }

    return 0;
}

// Save Types in File
int saveTypes(Type* head) {
    FILE* fp;
    fp = fopen(DATA_DIR"types.txt", "w");

    if (fp == NULL) return 0;

    while (head != NULL) {
        fprintf(fp, "%d;%s;%f\n", head->id, head->name, head->cost);
        head = head->next;
    }

    fclose(fp);

    return 1;
}

// Read Types from File
Type* readTypes() {
    FILE* fp;
    fp = fopen(DATA_DIR"types.txt", "r");
    Type* aux = NULL;

    if (fp == NULL) return aux;
    
    int c = fgetc(fp);
    if (c == EOF) {
        fclose(fp);
        return aux;
    }
    ungetc(c, fp);

    int id;
    float cost;
    char name[SIZE_NAME];

    while (!feof(fp)) { 
        fscanf(fp, "%d;%[^;];%f\n", &id, &name, &cost);
        aux = insertType(aux, id, name, cost);
    }

    fclose(fp);

    return aux;
}