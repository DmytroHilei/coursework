#ifndef GPS_PROCESSING_H
#define GPS_PROCESSING_H

#include "main.h"

struct VehicleGpsData
{
    char line[16];
    double latitude;
    double longitude;
    double distance;
};


/* Read GPS data from file */
int ReadGpsDataFromFile(char *filename, struct VehicleGpsData *data);

/* Compute distance of vehicles from ICT building */
void ComputeDistances(struct VehicleGpsData *data, int n);

/* Comparator for qsort (distance ascending) */
int CompareVehicleDistance(const void *a, const void *b);

/* Print closest vehicles */
void PrintClosestVehicles(struct VehicleGpsData *data, int n);

/* Haversine distance calculation */
double Haversine(double lat1, double lon1, double lat2, double lon2);

int CompareVehicleDistance(const void *a, const void *b);
#endif
