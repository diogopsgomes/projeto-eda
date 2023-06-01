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
        Location* headLocations = readLocations();

        clrscr();
        menuHeaderVehicles();

        if ((count = listVehicles(head, headTypes, headLocations, HQ)) == 0) {
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
                count = listVehiclesByRange(head, headTypes, headLocations, HQ);
                puts("");
                showCount(count);
                puts("");
                enterToContinue();

                break;
            case 5:
                menuLine();
                printf("Localizacao: ");
                clrbuffer();
                fgets(location, sizeof(location), stdin);
                location[strcspn(location, "\n")] = 0;

                clrscr();
                menuHeaderVehicles();

                if ((count = listVehiclesInLocation(head, headTypes, headLocations, location)) == 0) {
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
    Vehicle* new = malloc(sizeof(struct vehicle)), * aux = head;

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
    Vehicle* prev = head, * current = head, * aux;

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
int listVehicles(Vehicle* head, Type* headTypes, Location* headLocations, char location[]) {
    int count = 0;
    char available[5];

    while (head != NULL) {
        (head->available == 1) ? (strcpy(available, "Sim")) : (strcpy(available, "Nao"));

        printf("  %06d\t%-25s\t%-5.1f\t\t\t%-7.3f\t\t\t%-5s\t\t%s (%.3f km)\n", head->id, getTypeName(headTypes, head->type), head->battery, head->range, available, head->location, getDistance(headLocations, location, head->location));

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
int listVehiclesByRange(Vehicle* head, Type* headTypes, Location* headLocations, char location[]) {
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

    return listVehicles(head, headTypes, headLocations, location);
}

/**
 * This function sorts a linked list of vehicles by their battery level and then lists them.
 *
 * @param head a pointer to the head of a linked list of Vehicle structs
 * @param headTypes A pointer to the head of a linked list of Type structs, which contain information
 * about the types of vehicles (e.g. electric, hybrid, gas).
 * @param headLocations A pointer to the head of a linked list of Location structs.
 * @param location The parameter "location" is a string that represents the location where the vehicles
 * are located. It is used as a filter to only list the vehicles that are located in that specific
 * location.
 *
 * @return the result of calling the function `listVehicles` with the sorted linked list of vehicles as
 * its first argument, and the other arguments passed to the function as well.
 */
int listVehiclesByBattery(Vehicle* head, Type* headTypes, Location* headLocations, char location[]) {
    int swapped;

    if (head != NULL) {
        do {
            Vehicle* prev = NULL;
            Vehicle* current = head;
            swapped = 0;

            while (current->next != NULL) {
                if (current->battery > current->next->battery) {
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

    return listVehicles(head, headTypes, headLocations, location);
}

/**
 * This function lists vehicles in ascending order of distance from a given location, with ties broken
 * by range.
 *
 * @param head a pointer to the head of a linked list of Vehicle structs
 * @param headTypes a pointer to the head of a linked list of Type structs
 * @param headLocations a pointer to the head of a linked list of Location structs
 * @param location A string representing the location for which the vehicles need to be listed in order
 * of increasing distance.
 *
 * @return an integer value, which is the result of calling the function `listVehicles` with the sorted
 * linked list of vehicles as its first argument, and the other linked lists as the remaining
 * arguments.
 */
int listVehiclesByDistance(Vehicle* head, Type* headTypes, Location* headLocations, char location[]) {
    int swapped;

    if (head != NULL) {
        do {
            Vehicle* prev = NULL;
            Vehicle* current = head;
            swapped = 0;

            while (current->next != NULL) {
                if (getDistance(headLocations, location, current->location) > getDistance(headLocations, location, current->next->location)) {
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
                } else if (getDistance(headLocations, location, current->location) == getDistance(headLocations, location, current->next->location)) {
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
                    }
                } else {
                    prev = current;
                    current = current->next;
                }
            }
        } while (swapped);
    }

    return listVehicles(head, headTypes, headLocations, location);
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
int listVehiclesInLocation(Vehicle* head, Type* headTypes, Location* headLocations, char location[]) {
    Vehicle* filtered = NULL;

    while (head != NULL) {
        if (strcmp(head->location, location) == 0) {
            filtered = insertVehicle(filtered, head->id, head->type, head->battery, head->range, head->available, head->location);
        }

        head = head->next;
    }

    return listVehiclesByRange(filtered, headTypes, headLocations, location);
}

/**
 * The function lists all vehicles within a certain radius of a given location.
 *
 * @param head a pointer to the head of a linked list of Vehicle structs
 * @param headTypes A pointer to the head of a linked list of vehicle types.
 * @param headLocations A linked list of Location structs containing information about the locations of
 * vehicles.
 * @param location The location parameter is a string that represents the starting location from which
 * the distance to each vehicle's location will be calculated.
 * @param radius The radius is a float value that represents the maximum distance from a given location
 * within which vehicles should be listed.
 *
 * @return the result of calling the function `listVehiclesByDistance` with the filtered list of
 * vehicles as the first argument, and the head of the types and locations linked lists, as well as a
 * location string, as the remaining arguments.
 */
int listVehiclesInRadius(Vehicle* head, Type* headTypes, Location* headLocations, char location[], float radius) {
    Vehicle* filtered = NULL;

    while (head != NULL) {
        if (getDistance(headLocations, location, head->location) <= radius) {
            filtered = insertVehicle(filtered, head->id, head->type, head->battery, head->range, head->available, head->location);
        }

        head = head->next;
    }

    return listVehiclesByDistance(filtered, headTypes, headLocations, location);
}

/**
 * This function filters a linked list of vehicles by type and location within a certain radius and
 * then lists them by distance.
 *
 * @param head A pointer to the head of a linked list of Vehicle structs.
 * @param headTypes It is a pointer to the head of a linked list of Type structs.
 * @param headLocations A pointer to the head of a linked list of Location structs, which contains
 * information about the locations of vehicles.
 * @param type an integer representing the type of vehicle to filter by. If set to 0, all types of
 * vehicles will be included in the result.
 * @param location The location parameter is a string that represents the location from which the
 * distance to the vehicles will be calculated.
 * @param radius The radius is a float value that represents the maximum distance from a given location
 * within which vehicles of a certain type should be listed.
 *
 * @return the result of calling the function `listVehiclesByDistance` with the filtered list of
 * vehicles as the first argument, and the head of the types and locations lists and the specified
 * location as the remaining arguments.
 */
int listVehiclesByTypeInRadius(Vehicle* head, Type* headTypes, Location* headLocations, int type, char location[], float radius) {
    Vehicle* filtered = NULL;

    while (head != NULL) {
        if ((getDistance(headLocations, location, head->location) <= radius) && (head->type == type || type == 0)) {
            filtered = insertVehicle(filtered, head->id, head->type, head->battery, head->range, head->available, head->location);
        }

        head = head->next;
    }

    return listVehiclesByDistance(filtered, headTypes, headLocations, location);
}

/**
 * This function filters vehicles with battery levels below 50% and lists them by location.
 *
 * @param head a pointer to the head of a linked list of Vehicle structs
 * @param headTypes A pointer to the head of the linked list of vehicle types.
 * @param headLocations A pointer to the head of a linked list of Location structs, which contains
 * information about the locations of vehicles.
 * @param location The parameter "location" is a string that represents the name of a location. It is
 * used as a filter to list only the vehicles that are located in that specific location.
 *
 * @return the result of calling the function `listVehiclesByBattery()` with the filtered list of
 * vehicles as the first argument, along with the other arguments passed to the function.
 */
int listVehiclesByBatteryHalfCharged(Vehicle* head, Type* headTypes, Location* headLocations, char location[]) {
    Vehicle* filtered = NULL;

    while (head != NULL) {
        if (head->battery < 50) {
            filtered = insertVehicle(filtered, head->id, head->type, head->battery, head->range, head->available, head->location);
        }

        head = head->next;
    }

    return listVehiclesByBattery(filtered, headTypes, headLocations, location);
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
            if (head->available == 1) return 1;
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
            if (head->battery > 0 && head->range > 0) return 1;
        }

        head = head->next;
    }

    return 0;
}

/**
 * The function updates the location of a vehicle with a given ID in a linked list.
 *
 * @param head a pointer to the head of a linked list of Vehicle structs
 * @param id The id parameter is an integer that represents the unique identifier of a vehicle.
 * @param location The parameter "location" is a character array that represents the new location of a
 * vehicle. This function updates the location of a vehicle with the given "id" to the new location
 * provided in the "location" parameter.
 */
void updateVehicleLocation(Vehicle* head, int id, char location[]) {
    while (head != NULL) {
        if (head->id == id) {
            strcpy(head->location, location);

            break;
        }

        head = head->next;
    }
}

/**
 * The function charges all vehicles located in a specific location by setting their battery to 100%
 * and updating their range accordingly.
 *
 * @param head A pointer to the head of a linked list of Vehicle structures.
 * @param location The location where the vehicles are currently parked or located.
 *
 * @return a pointer to the head of the linked list of vehicles.
 */
Vehicle* chargeVehicles(Vehicle* head, char location[]) {
    Vehicle* aux = head;

    while (aux != NULL) {
        if (strcmp(aux->location, location) == 0) {
            aux->battery = 100.0F;
            aux->range = aux->battery * 2.0F;
        }

        aux = aux->next;
    }

    return head;
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

/**
 * The function returns the name of a vehicle type given its ID, by iterating through a linked list of
 * vehicles and using a separate linked list of types.
 *
 * @param head A pointer to the head of a linked list of Vehicle structs.
 * @param headTypes A pointer to the head of a linked list of Type structs.
 * @param id The id parameter is an integer that represents the unique identifier of a vehicle.
 *
 * @return a string that represents the name of the vehicle type associated with the given ID. If the
 * ID is not found in the linked list of vehicles, the function returns a string of asterisks.
 */
char* getVehicleTypeName(Vehicle* head, Type* headTypes, int id) {
    while (head != NULL) {
        if (head->id == id) return getTypeName(headTypes, head->type);

        head = head->next;
    }

    return "*********";
}

/**
 * The function returns the battery level of a vehicle with a given ID from a linked list of vehicles.
 *
 * @param head a pointer to the head of a linked list of Vehicle structs
 * @param id The id parameter is an integer that represents the unique identifier of a vehicle.
 *
 * @return The function `getVehicleBattery` returns a float value representing the battery level of a
 * vehicle with the given `id`. If a vehicle with the given `id` is found in the linked list pointed to
 * by `head`, the function returns the battery level of that vehicle. If no vehicle with the given `id`
 * is found, the function returns -1.
 */
float getVehicleBattery(Vehicle* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->battery;

        head = head->next;
    }

    return -1;
}

/**
 * The function returns the location of a vehicle with a given ID from a linked list of vehicles.
 *
 * @param head a pointer to the head of a linked list of Vehicle structs
 * @param id The id parameter is an integer that represents the unique identifier of a vehicle.
 *
 * @return If a vehicle with the given ID is found in the linked list, the function returns the
 * location of that vehicle as a string. If no vehicle with the given ID is found, the function returns
 * the string "*********".
 */
char* getVehicleLocation(Vehicle* head, int id) {
    while (head != NULL) {
        if (head->id == id) return head->location;

        head = head->next;
    }

    return "*********";
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
    Type* new = malloc(sizeof(struct type)), * aux = head;

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

    fp = fopen(DATA_DIR"types.bin", "wb");

    if (fp != NULL) {
        while (head != NULL) {
            fwrite(head, sizeof(struct type), 1, fp);

            head = head->next;
        }

        fclose(fp);
    }

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