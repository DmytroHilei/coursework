#pragma once
#include <inttypes.h>

#ifndef HEADER_PART1_H
#define HEADER_PART1_H

#define MAX_PEOPLE 100
#define MAX_VEHICLE 100

typedef struct {
    uint64_t id;
    char first_name[50];
    char last_name[50];
    double income;

    //Part2
    double total_tax;
    double tax_percentage;
} Person;

typedef struct {
    uint64_t id;
    char registration_number[50];
    double anual_tax;
} Car;


int reading_People(Person* peoples, int length, const char* file_name);
int reading_Vehicle(Car *vehicles, int length, const char* file_name);
void merge(Person *peoples, int l, int m, int r);
void mergeSort(Person *peoples, int l, int r);
void display_data(Person *peoples, int peoples_number,
                  Car *vehicles, int vehicles_number);
void print_tax(Person *p);
void calculate_taxes(Person *peoples, int peoples_number,
                     Car *vehicles, int vehicles_number);
void display_taxes(Person *peoples, int peoples_number);
#endif //HEADER_PART1_H
