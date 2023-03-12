#ifndef HEADER_H_
#define HEADER_H_

#define SIZE_USERNAME 40
#define SIZE_PASSWORD 40
#define SIZE_NAME 60
#define SIZE_ADDRESS 150
#define SIZE_LOCATION 60

#define SIZE_TYPE 5
#define SIZE_BATTERY 15
#define SIZE_RANGE 15

typedef struct datetime {
    int day;
    int month;
    int year;
    int hour;
    int minute;

} DateTime;

typedef struct type {
    int id; // 1 - Trotinete; 2 - Bicicleta
    char name[SIZE_NAME];
    float cost;
    struct type* next;

} Type;

typedef struct vehicle {
    int id;
    int type;
    float battery;
    float range;
    char location[SIZE_LOCATION];
    struct vehicle* next;

} Vehicle;

typedef struct client {
    int id;
    char username[SIZE_USERNAME];
    char password[SIZE_PASSWORD];
    char name[SIZE_NAME];
    int nif;
    char address[SIZE_ADDRESS];
    float balance;
    struct client* next;

} Client;

typedef struct manager {
    int id;
    char username[SIZE_USERNAME];
    char password[SIZE_PASSWORD];
    char name[SIZE_NAME];
    struct manager* next;

} Manager;

typedef struct ride {
    int id;
    int client;
    int vehicle;
    DateTime startDate;
    DateTime endDate;
    char startLocation[SIZE_LOCATION];
    char endLocation[SIZE_LOCATION];
    float cost; //?
    int duration; //?
    float distance;
    struct ride* next;

} Ride;

/*Vehicles*/
void vehiclesMain();
Vehicle* insertVehicle(Vehicle* head, int id, int type, float battery, float range, char location[]);
Vehicle* removeVehicle(Vehicle* head, int id);
void editVehicle(Vehicle* head, Type* typesHead, int id, int type, float battery, float range, char location[]);
int listVehicles(Vehicle* head, Type* typesHead);
int existVehicle(Vehicle* head, int id);
int assignVehicleId(Vehicle* head);
int saveVehicles(Vehicle* head);
Vehicle* readVehicles();
char* getTypeName(Type* head, int id);
Type* insertType(Type* head, int id, char name[], float cost);
int listTypes(Type* head);
int existType(Type* head, int id);
int saveTypes(Type* head);
Type* readTypes();

/*Managers*/
char* getManagerName(Manager* head, int id);
Manager* insertManager(Manager* head, int id, char username[], char password[], char name[]);
int authManager(Manager* head, char username[], char password[]);
Manager* readManagers();

/*Menus*/
void menuMain();
void menuAuth();
void menuAuthClients();
void menuAuthManagers();
void menuHeaderVehicles();
void menuFooterRides();
void menuFooterVehicles();
void menuFooterClients();
void menuFooterManagers();
void menuTitleInsertVehicle();
void menuTitleRemoveVehicle();
void menuTitleEditVehicle();

/*Utilities*/
void clrscr();
void clrbuffer();
void enterToContinue();
void readStr(char* str);

#endif