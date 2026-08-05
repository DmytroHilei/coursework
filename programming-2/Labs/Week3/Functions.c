#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "Headers.h"

int readPerson(FILE *file, Employee *person) {
    return fscanf(file, "%15s %31s %31s %31s",
        person->id,
        person->firstName,
        person->lastName,
        person->city) == 4;
}
int printPersonByCity(char *city, Employee *employees, int size) {
    int counter = 0;
    for (int i = 0; i < size; i++) {
        if (containsIgnoreCase(employees[i].city, city) == 1) {
            printf("%s %s\n", employees[i].lastName, employees[i].firstName);
            printf("City: %s\n", employees[i].city);

            char birth[MAX_ID];
            convertIDToBirth(employees[i].id, birth);
            printf("Birth: %s\n", birth);

            counter++;
        }
    }
    if (counter == 0) {
        printf("No city found\n");
    }
    return counter;
}

void merge(Employee *arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    Employee L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (strcmp(L[i].lastName, R[j].lastName) <= 0)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(Employee *arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}


void convertIDToBirth(const char *ID, char *out) {
    static const char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    int day   = (ID[5] - '0') * 10 + (ID[6] - '0');
    int month = (ID[3] - '0') * 10 + (ID[4] - '0');
    int year  = (ID[1] - '0') * 10 + (ID[2] - '0');

    if (ID[0] == '1' || ID[0] == '2') year += 1800;
    else if (ID[0] == '3' || ID[0] == '4') year += 1900;
    else if (ID[0] == '5' || ID[0] == '6') year += 2000;

    snprintf(out, OUT_SIZE, "%d. %s %d",
             day,
             months[month - 1],
             year);

}
int containsIgnoreCase(const char *text, const char *pattern) {
    char a[MAX_CITY];
    char b[MAX_BUFFER];

    strncpy(a, text, sizeof(a));
    a[sizeof(a) - 1] = '\0';

    strncpy(b, pattern, sizeof(b));
    b[sizeof(b) - 1] = '\0';

    for (int i = 0; a[i]; i++) {
        a[i] = (char)tolower(a[i]);
    }
    for (int i = 0; b[i]; i++) {
        b[i] = (char)tolower(b[i]);
    }

    return strstr(a, b) != NULL;
}
int buildCityList(Employee *employees,
                  int size,
                  char cities[][MAX_CITY])
{
    if (size == 0)
        return 0;

    int count = 1;

    strcpy(cities[0], employees[0].city);

    for (int i = 1; i < size; i++) {
        if (strcmp(employees[i].city, cities[count - 1]) != 0) {
            strcpy(cities[count], employees[i].city);
            count++;
        }
    }

    return count;
}





