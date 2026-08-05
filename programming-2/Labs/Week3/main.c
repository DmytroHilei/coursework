#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Headers.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr,"Please, enter the name of the file! \n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File could not be opened! \n");
        exit(1);
    }

    Employee employees[MAX_EMPLOYEE];
    int i = 0;
    while (i < MAX_EMPLOYEE && readPerson(file, &employees[i])) {
        i++;
    }
    mergeSort(employees, 0, i - 1);

    char cities[MAX_EMPLOYEE][MAX_CITY];
    int numOfCities = buildCityList(employees, i, cities);

    char buffer[MAX_BUFFER];
    while (1) {
        printf("Enter a city name to search, ':exit' to quit, ':all' to print all guys or ':list' to print all cities\n");
        fgets(buffer, MAX_BUFFER, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // to remove \n at the end
        if (strcasecmp(buffer, ":exit") == 0) {
            printf("Exiting...\n");
            return 0;
        }
        if (strcasecmp(buffer, ":all") == 0) {
            printf("All employees\n");
            for (int j = 0; j < i; j++) {
                printf("%s %s\n", employees[j].lastName, employees[j].firstName);
                printf("City: %s\n", employees[j].city);

                char birth[MAX_ID];
                convertIDToBirth(employees[j].id, birth);
                printf("Birth: %s\n", birth);
            }
            continue;
        }
        if (strcasecmp(buffer, ":list") == 0) {
            printf("List of available cities\n");
            for (int j = 0; j < numOfCities; j++) {
                printf("%s\n", cities[j]);
            }

            continue;
        }
        int counter = printPersonByCity(buffer, employees, i);
        printf("[%d/%d]\n", counter, i);

    }
    return 0;
}
