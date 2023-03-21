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
/**
 * It inserts a new vehicle at the end of the list
 * 
 * @param head The head of the linked list
 * @param id The id of the vehicle
 * @param type The type of the vehicle
 * @param battery The battery of the vehicle
 * @param range The range of the vehicle
 * @param available 0 = not available, 1 = available
 * @param location The location of the vehicle
 * 
 * @return The head of the list.
 */
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
/**
 * If the list is empty, return NULL. If the first element is the one to be removed, free it and return
 * the second element. Otherwise, find the element to be removed and free it
 * 
 * @param head The head of the linked list
 * @param id The id of the vehicle to be removed
 * 
 * @return The head of the list.
 */
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
/**
 * It edits a vehicle's information
 * 
 * @param head The head of the linked list
 * @param headTypes Pointer to the first type of vehicle in the linked list
 * @param id The id of the vehicle to edit
 * @param type The type of vehicle
 * @param battery The battery of the vehicle
 * @param range The range of the vehicle
 * @param location The location of the vehicle
 */
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
/**
 * It prints a list of vehicles
 * 
 * @param head The head of the linked list
 * @param headTypes Pointer to the first type of vehicle in the linked list
 * 
 * @return The number of vehicles in the list.
 */
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
/**
 * It sorts the linked list by range, then lists the vehicles
 * 
 * @param head The head of the linked list
 * @param headTypes Pointer to the first type of vehicle in the linked list
 * 
 * @return The return value is the result of the function listVehicles.
 */
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

/**
 * It filters the linked list by location, then lists the vehicles sorted by range
 * 
 * @param head pointer to the first element of the linked list
 * @param headTypes a linked list of types
 * @param location The location of the vehicle
 * 
 * @return The return value is the number of vehicles that were listed.
 */
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
/**
 * It returns 1 if the vehicle with the given id exists in the list, otherwise it returns 0
 * 
 * @param head The head of the linked list
 * @param id The id of the vehicle to be added
 * 
 * @return 1 if the vehicle exists in the list, otherwise it returns 0.
 */
int existVehicle(Vehicle* head, int id) {
    while (head != NULL) {
        if (head->id == id) return 1;

        head = head->next;
    }

    return 0;
}

// Assign an ID to a Vehicle based on the last Vehicle in the list (+1)
/**
 * It returns the next available vehicle id
 * 
 * @param head The head of the linked list
 * 
 * @return The next available ID number.
 */
int assignVehicleId(Vehicle* head) {
    while (head != NULL) {
        if (head->next == NULL) return head->id + 1;

        head = head->next;
    }

    return 1;
}

// Check if a Vehicle is not in a Ride
/**
 * It checks if a vehicle is available
 * 
 * @param head The head of the linked list
 * @param id The id of the vehicle to check
 * 
 * @return 1 if the vehicle is available, otherwise it returns 0.
 */
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
/**
 * It checks if the vehicle is charged and has a range greater than 0
 * 
 * @param head The head of the linked list
 * @param id The id of the vehicle to check
 * 
 * @return 1 if the vehicle has any battery, otherwise it returns 0.
 */
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
/**
 * It creates a new linked list, and copies the contents of the original linked list into the new
 * linked list
 * 
 * @param head The head of the linked list
 * 
 * @return The head of the copied linked list.
 */
Vehicle* copyLinkedList(Vehicle* head) {
    Vehicle* copy = NULL;
    while (head != NULL) {
        copy = insertVehicle(copy, head->id, head->type, head->battery, head->range, head->available, head->location);
        head = head->next;
    }
    return copy;
}

// Save Vehicles in File
/**
 * It saves the vehicles to a file
 * 
 * @param head The head of the linked list
 * 
 * @return 1 if the file was successfully saved, and 0 if it was not.
 */
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
/**
 * It reads a file and inserts the data into a linked list
 * 
 * @return A pointer to a Vehicle struct.
 */
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
/**
 * It loops through the linked list of vehicles, and if the vehicle's id matches the id passed in, it
 * returns the cost of the vehicle's type
 * 
 * @param head The head of the linked list of vehicles
 * @param headTypes The head of the linked list of types
 * @param id The id of the vehicle you want to get the cost of.
 * 
 * @return The cost of the vehicle.
 */
float getVehicleCost(Vehicle* head, Type* headTypes, int id) {
    while (head != NULL) {
        if (head->id == id) return getTypeCost(headTypes, head->type);

        head = head->next;
    }

    return -1;
}

// Get Type Cost from Type ID
/**
 * It returns the cost of a type with a given id
 * 
 * @param head The head of the linked list
 * @param id The id of the type you want to get the cost of.
 * 
 * @return The cost of the type with the given id.
 */
float getTypeCost(Type* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->cost;

        head = head->next;
    }

    return -1;
}

// Get Type Name from Type ID
/**
 * It returns the name of the type with the given id, or "*********" if the type doesn't exist
 * 
 * @param head The head of the linked list
 * @param id The id of the type you want to get the name of.
 * 
 * @return The name of the type with the given id.
 */
char* getTypeName(Type* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->name;

        head = head->next;
    }

    return "*********";
}

// Insert Type of Vehicle
/**
 * It inserts a new client at the end of the list
 * 
 * @param head The head of the linked list
 * @param id The id of the type of vehicle
 * @param name The name of the type of vehicle
 * @param cost The cost of the type of vehicle
 * 
 * @return The head of the list.
 */
Type* insertType(Type* head, int id, char name[], float cost) {
    Type *new = malloc(sizeof(struct type)), *aux = head;

    if (new != NULL) {
        new->id = id;
        strcpy(new->name, name);
        new->cost = cost;
        new->next = head;
    }

    if (head == NULL) return new;

    while (aux->next != NULL) aux = aux->next;

    aux->next = new;

	return head;
}

// List Type of Vehicles in Console
/**
 * It prints the contents of a linked list of types
 * 
 * @param head The head of the linked list
 * 
 * @return The number of items in the list.
 */
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
/**
 * It checks if a type with the given id exists in the list
 * 
 * @param head The head of the linked list
 * @param id The id of the type
 * 
 * @return 1 if the type exists in the list, otherwise it returns 0.
 */
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
/**
 * It saves the types to a file
 * 
 * @param head The head of the linked list
 * 
 * @return 1 if the file was saved successfully, or 0 if it wasn't.
 */
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
/**
 * It reads a file and inserts the data into a linked list
 * 
 * @return A pointer to a Type struct.
 */
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