//
// Created by giley on 12/28/2025.
//

#ifndef HEADERS_H
#define HEADERS_H

#define MAX_NAME 32
#define MAX_SURNAME 32
#define MAX_CITY 32
#define MAX_EMPLOYEE 256
#define MAX_BUFFER 256
#define MAX_ID 16

#define OUT_SIZE 128
typedef struct {
    char id[MAX_ID];
    char firstName[MAX_NAME];
    char lastName[MAX_SURNAME];
    char city[MAX_CITY];
} Employee;

int readPerson(FILE *file, Employee *person);
int printPersonByCity(char *city, Employee *employees, int size);
void mergeSort(Employee *arr, int l, int r);//
// Created by giley on 12/28/2025.
//

#ifndef HEADERS_H
#define HEADERS_H

#define MAX_NAME 32
#define MAX_SURNAME 32
#define MAX_CITY 32
#define MAX_EMPLOYEE 256
#define MAX_BUFFER 256
#define MAX_ID 16

#define OUT_SIZE 128
typedef struct {
    char id[MAX_ID];
    char firstName[MAX_NAME];
    char lastName[MAX_SURNAME];
    char city[MAX_CITY];
} Employee;

int readPerson(FILE *file, Employee *person);
int printPersonByCity(char *city, Employee *employees, int size);
void mergeSort(Employee *arr, int l, int r);
void merge(Employee *arr, int l, int m, int r);
void convertIDToBirth(const char *ID, char *out);
int containsIgnoreCase(const char *text, const char *pattern);
int buildCityList(Employee *employees,
                  int size,
                  char cities[][MAX_CITY]);
#endif //HEADERS_H
void merge(Employee *arr, int l, int m, int r);
void convertIDToBirth(const char *ID, char *out);
int containsIgnoreCase(const char *text, const char *pattern);
int buildCityList(Employee *employees,
                  int size,
                  char cities[][MAX_CITY]);
#endif //HEADERS_H
