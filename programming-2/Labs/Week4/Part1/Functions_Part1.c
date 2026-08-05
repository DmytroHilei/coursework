#include <stdio.h>
#include <string.h>

#include "Header_Part1.h"

int reading_People(Person *peoples, int length, const char* file_name) {
    int counter = 0;
    FILE *pl = fopen(file_name, "r");
    if (pl == NULL) {
        printf("Error opening file\n");
        return -1;
    }

    while (counter < length && fscanf(pl, "%" SCNu64 " %49s %49s %lf",
        &peoples->id
        , peoples->first_name,
        peoples->last_name
        , &peoples->income) == 4) {

        counter++;
        peoples++;
    }
    fclose(pl);
    return counter;
}

int reading_Vehicle(Car *vehicles, int length, const char* file_name) {
    int counter = 0;
    FILE *cr = fopen(file_name, "r");
    if (cr == NULL) {
        printf("Error opening file\n");
        return -1;
    }

    while (counter < length && fscanf(cr, "%" SCNu64 " %49s" " %lf",
        &vehicles->id
        , vehicles->registration_number,
        &vehicles->anual_tax) == 3) {
        counter++;
        vehicles++;
    }
    fclose(cr);
    return counter;
}

void merge(Person *peoples, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    Person L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = *(peoples + l + i);
    for (int j = 0; j < n2; j++) R[j] = *(peoples + m + 1 + j);

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (strcmp(L[i].first_name, R[j].first_name) <= 0)
            *(peoples + k++) = L[i++];
        else
            *(peoples + k++) = R[j++];
    }

    while (i < n1) *(peoples + k++) = L[i++];
    while (j < n2) *(peoples + k++) = R[j++];
}

void mergeSort(Person *peoples, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(peoples, l, m);
        mergeSort(peoples, m + 1, r);
        merge(peoples, l, m, r);
    }
}

void display_data(Person *peoples, int peoples_number,
                  Car *vehicles, int vehicles_number)
{
    for (int i = 0; i < peoples_number; i++) {

        Person *p = peoples + i;

        printf("%" PRIu64 " %s %s (income: %.2f)\n",
               p->id,
               p->first_name,
               p->last_name,
               p->income);


        int car_count = 0;

        for (int j = 0; j < vehicles_number; j++) {
            Car *c = vehicles + j;

            if (c->id == p->id) {
                car_count++;
                printf("        car %d: %7s (tax: %.2f)\n",
                       car_count,
                       c->registration_number,
                       c->anual_tax);

            }
        }

        if (car_count == 0) {
            printf("        *** No registered cars ***\n");
        }

        printf("\n");
    }
}


