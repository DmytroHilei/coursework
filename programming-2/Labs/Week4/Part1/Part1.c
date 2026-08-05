#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "Header_Part1.h"

int main(void) {
    Person peoples[MAX_PEOPLE];
    Car vehicles[MAX_VEHICLE];

    int number_of_people = reading_People(peoples, MAX_PEOPLE, "../person.txt" );
    int number_of_vehicles = reading_Vehicle(vehicles, MAX_VEHICLE, "../vehicles.txt");
    mergeSort(peoples, 0, number_of_people - 1);
    display_data(peoples, number_of_people, vehicles, number_of_vehicles);

    if (number_of_people < 0 || number_of_vehicles < 0) {
        printf("No data found\n");
        exit(0);
    }

    return 0;
}



