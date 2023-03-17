#ifndef HEADER_H_
#define HEADER_H_

#define DATA_DIR "data/"

#define SIZE_USERNAME 40
#define SIZE_PASSWORD 40
#define SIZE_NAME 60
#define SIZE_ADDRESS 150
#define SIZE_LOCATION 60

#define SIZE_TYPE 5
#define SIZE_BATTERY 15
#define SIZE_RANGE 15

#define SIZE_NIF 15

#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"

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
void editVehicle(Vehicle* head, Type* headTypes, int id, int type, float battery, float range, char location[]);
int listVehicles(Vehicle* head, Type* headTypes);
int listVehiclesByRange(Vehicle* head, Type* headTypes);
int listVehiclesByLocation(Vehicle* head, Type* headTypes, char location[]);
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

/*Clients*/
void clientsMain();
Client* insertClient(Client* head, int id, char username[], char password[], char name[], int nif, char address[], float balance);
Client* removeClient(Client* head, int id);
void editClient(Client* head, int id, char username[], char password[], char name[], int nif, char address[]);
int listClients(Client* head);
int listClient(Client* head, int id);
char* getClientName(Client* head, int id);
int existClient(Client* head, int id);
int assignClientId(Client* head);
void addBalance(Client* head, int id, float balance);
void removeBalance(Client* head, int id, float balance);
void editBalance(Client* head, int id, float balance);
int saveClients(Client* head);
Client* readClients();

/*Managers*/
void managersMain();
Manager* insertManager(Manager* head, int id, char username[], char password[], char name[]);
Manager* removeManager(Manager* head, int id);
void editManager(Manager* head, int id, char username[], char password[], char name[]);
int listManagers(Manager* head);
char* getManagerName(Manager* head, int id);
int existManager(Manager* head, int id);
int assignManagerId(Manager* head);
int saveManagers(Manager* head);
Manager* readManagers();

/*Auth*/
void encrypt(char password[]);
void decrypt(char password[]);
int authClient(Client* head, char username[], char password[]);
int authManager(Manager* head, char username[], char password[]);

/*Menus*/
void menuApp();
void menuMain();
void menuMainClients();
void menuMainClientsLine();
void menuAuth();
void menuAuthClients();
void menuAuthManagers();
void menuHeaderVehicles();
void menuHeaderClients();
void menuHeaderClient();
void menuHeaderManagers();
void menuFooterRides();
void menuFooterVehicles();
void menuFooterClients();
void menuFooterManagers();
void menuTitleInsertVehicle();
void menuTitleRemoveVehicle();
void menuTitleEditVehicle();
void menuTitleListVehiclesByLocation();
void menuTitleInsertClient();
void menuTitleRemoveClient();
void menuTitleEditClient();
void menuTitleAddBalance();
void menuTitleRemoveBalance();
void menuTitleInsertManager();
void menuTitleRemoveManager();
void menuTitleEditManager();

/*Utilities*/
void clrscr();
void clrbuffer();
void enterToContinue();
void showCount(int count);

#endif