#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 101
#define MAX_STR 100

// ----------- HASH TABLE for USERS ------------

typedef struct User {
char username[50];
char password[50];
struct User* next; // for chaining
} User;

User* userHashTable[MAX_USERS];

unsigned int hashFunc(char* key) {
unsigned int hash = 0;
for (int i = 0; key[i]; i++) {
hash = (hash * 31 + key[i]) % MAX_USERS;
}
return hash;
}

User* findUser(char* username) {
unsigned int idx = hashFunc(username);
User* cur = userHashTable[idx];
while (cur) {
if (strcmp(cur->username, username) == 0)
return cur;
cur = cur->next;
}
return NULL;
}

void registerUser(char* username, char* password) {
if (findUser(username)) {
printf("[!] Username already exists!\n");
return;
}
User* newUser = malloc(sizeof(User));
strcpy(newUser->username, username);
strcpy(newUser->password, password);
unsigned int idx = hashFunc(username);
newUser->next = userHashTable[idx];
userHashTable[idx] = newUser;
printf("[+] User registered successfully!\n");
}

int login(char* username, char* password) {
User* u = findUser(username);
if (u && strcmp(u->password, password) == 0) return 1;
return 0;
}

// ----------- BST for ACTIVITIES ------------

typedef struct Activity {
char city[50];
char date[20]; // "DD/MM/YYYY"
char time[10]; // "HH:MM"
char desc[100];
struct Activity *left, *right;
} Activity;

int compareDateTime(char* date1, char* time1, char* date2, char* time2) {
int d1, m1, y1, d2, m2, y2;
sscanf(date1, "%d/%d/%d", &d1, &m1, &y1);
sscanf(date2, "%d/%d/%d", &d2, &m2, &y2);
if (y1 != y2) return (y1 < y2) ? -1 : 1;
if (m1 != m2) return (m1 < m2) ? -1 : 1;
if (d1 != d2) return (d1 < d2) ? -1 : 1;

int hh1, mm1, hh2, mm2;
sscanf(time1, "%d:%d", &hh1, &mm1);
sscanf(time2, "%d:%d", &hh2, &mm2);
if (hh1 != hh2) return (hh1 < hh2) ? -1 : 1;
if (mm1 != mm2) return (mm1 < mm2) ? -1 : 1;

return 0;
}

Activity* insertActivity(Activity* root, char* city, char* date, char* time, char* desc) {
if (!root) {
Activity* node = malloc(sizeof(Activity));
strcpy(node->city, city);
strcpy(node->date, date);
strcpy(node->time, time);
strcpy(node->desc, desc);
node->left = node->right = NULL;
return node;
}
int cmp = compareDateTime(date, time, root->date, root->time);
if (cmp < 0)
root->left = insertActivity(root->left, city, date, time, desc);
else if (cmp > 0)
root->right = insertActivity(root->right, city, date, time, desc);
else
printf("[!] Activity already exists at this date and time!\n");
return root;
}

void inorderActivities(Activity* root, char* city, char* date) {
if (!root) return;
inorderActivities(root->left, city, date);
if (strcmp(root->city, city) == 0 && strcmp(root->date, date) == 0) {
printf("Time: %s | %s\n", root->time, root->desc);
}
inorderActivities(root->right, city, date);
}

// ----------- QUEUE for BOOKINGS ------------

typedef struct Booking {
char city[50];
char hotel[100];
struct Booking* next;
} Booking;

typedef struct {
Booking* front;
Booking* rear;
} BookingQueue;

void initQueue(BookingQueue* q) {
q->front = q->rear = NULL;
}

int isEmpty(BookingQueue* q) {
return q->front == NULL;
}

void enqueue(BookingQueue* q, char* city, char* hotel) {
Booking* b = malloc(sizeof(Booking));
strcpy(b->city, city);
strcpy(b->hotel, hotel);
b->next = NULL;
if (q->rear) q->rear->next = b;
q->rear = b;
if (!q->front) q->front = b;
printf("[+] Booking added to queue!\n");
}

void viewBookings(BookingQueue* q) {
if (isEmpty(q)) {
printf("No bookings in queue.\n");
return;
}
Booking* cur = q->front;
printf("Your Bookings:\n");
while (cur) {
printf("- City: %s | Hotel: %s\n", cur->city, cur->hotel);
cur = cur->next;
}
}

// ----------- URL REDIRECTION (Fixed) ------------

void openURL(char* url) {
char cmd[300];
#ifdef _WIN32
sprintf(cmd, "start %s", url); // Windows
#elif _APPLE_
sprintf(cmd, "open '%s'", url); // macOS
#else
sprintf(cmd, "xdg-open '%s'", url); // Linux
#endif
system(cmd);
}

void redirectBookingCom(char* city) {
char url[300];
snprintf(url, sizeof(url), "https://www.makemytrip.com/hotels/%s", city);
openURL(url);
}

void redirectGoogleMapsPlaces(char* city) {
char url[300];
snprintf(url, sizeof(url), "https://www.google.com/search?q=tourist+places+to+visit+in+%s", city);
openURL(url);
}

void redirectSwiggy(char* city) {
char url[300];
snprintf(url, sizeof(url), "https://www.zomato.com/%s", city);
openURL(url);
}

void redirectGoogleMapsRoute(char* source, char* dest) {
char url[300];
snprintf(url, sizeof(url), "https://www.google.com/maps/dir/%s/%s", source, dest);
openURL(url);
}

// ----------- MAIN ------------

int main() {
char username[50], password[50];
User* loggedIn = NULL;
Activity* activityRoot = NULL;
BookingQueue bookingQueue;
initQueue(&bookingQueue);
char city[MAX_STR] = {0};

while (1) {
if (!loggedIn) {
int ch;
printf("\n--- Welcome to MakeMyTrip ---\n");
printf("1. Register\n2. Login\n0. Exit\nChoice: ");
scanf("%d", &ch);
 getchar();
if (ch == 1) {
printf("Enter username: ");
fgets(username, sizeof(username), stdin);
username[strcspn(username, "\n")] = 0;
printf("Enter password: ");
fgets(password, sizeof(password), stdin);
password[strcspn(password, "\n")] = 0;
registerUser(username, password);
} else if (ch == 2) {
printf("Enter username: ");
fgets(username, sizeof(username), stdin);
username[strcspn(username, "\n")] = 0;
printf("Enter password: ");
fgets(password, sizeof(password), stdin);
password[strcspn(password, "\n")] = 0;
if (login(username, password)) {
printf("[+] Login successful! Welcome %s.\n", username);
loggedIn = findUser(username);
} 
else {
printf("[!] Login failed.\n");
}
} 
else if(ch==0) {
printf("Goodbye!\n");
break;
}
else{
printf("[!] Invalid choice. Try again.\n");
}
} 
else {
if (strlen(city) == 0) {
printf("\nEnter city you want to go: ");
fgets(city, sizeof(city), stdin);
city[strcspn(city, "\n")] = 0;
if (strlen(city) == 0) {
printf("[!] City cannot be empty.\n");
continue;
}
}
printf("%s",city);
printf("\n--- Menu for %s ---\n", city);
printf("1. Book hotels near me (Makemytrip.com)\n");
printf("2. Suggest tourism places (Google )\n");
printf("3. Order food (Zomato)\n");
printf("4. Show route to another city\n");
printf("5. Add activity to day plan\n");
printf("6. View activities for a date\n");
printf("7. View bookings\n");
printf("8. Logout\n");
printf("9. Change city\n");
printf("0. Exit\n");
printf("Choice: ");
int choice;
scanf("%d", &choice);
getchar();

if (choice == 1) {
redirectBookingCom(city);
printf("Makemytrip.com page opened in your browser.\n");
char hotelName[100];
printf("Enter hotel name to add to booking queue (or blank to skip): ");
fgets(hotelName, sizeof(hotelName), stdin);
hotelName[strcspn(hotelName, "\n")] = 0;
if (strlen(hotelName) > 0) {
enqueue(&bookingQueue, city, hotelName);
}
} else if (choice == 2) {
redirectGoogleMapsPlaces(city);
printf("Google suggested tourist places opened.\n");
} else if (choice == 3) {
redirectSwiggy(city);
printf("Swiggy page opened for your city.\n");
} else if (choice == 4) {
char destCity[MAX_STR];
printf("Enter destination city for route: ");
fgets(destCity, sizeof(destCity), stdin);
destCity[strcspn(destCity, "\n")] = 0;
if (strlen(destCity) == 0) {
printf("[!] Destination city cannot be empty.\n");
} else {
redirectGoogleMapsRoute(city, destCity);
printf("Route shown on Google Maps.\n");
}
} else if (choice == 5) {
char date[20], time[10], desc[100];
printf("Enter date (DD/MM/YYYY): ");
fgets(date, sizeof(date), stdin);
date[strcspn(date, "\n")] = 0;
printf("Enter time (HH:MM): ");
fgets(time, sizeof(time), stdin);
time[strcspn(time, "\n")] = 0;
printf("Enter activity description: ");
fgets(desc, sizeof(desc), stdin);
desc[strcspn(desc, "\n")] = 0;
activityRoot = insertActivity(activityRoot, city, date, time, desc);
} else if (choice == 6) {
char date[20];
printf("Enter date to view activities (DD/MM/YYYY): ");
fgets(date, sizeof(date), stdin);
date[strcspn(date, "\n")] = 0;
printf("Activities on %s in %s:\n", date, city);
inorderActivities(activityRoot, city, date);
} else if (choice == 7) {
viewBookings(&bookingQueue);
} else if (choice == 8) {
loggedIn = NULL;
strcpy(city, "");
printf("[+] Logged out successfully.\n");
} else if (choice == 9) {
strcpy(city, "");
printf("[+] City reset. Please enter new city.\n");
} else if (choice == 0) {
printf("Goodbye!\n");
break;
} else {
printf("[!] Invalid choice. Try again.\n");
}
}
}
return 0;
}
