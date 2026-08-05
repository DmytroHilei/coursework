#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gps_processing.h"
#include "main.h"
#include "data_processor.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#define EARTH_RADIUS 6371000.0

double DegToRad(double deg)
{
    return deg * M_PI / 180.0;
}

double Haversine(double lat1, double lon1, double lat2, double lon2)
{
    double dLat = DegToRad(lat2 - lat1);
    double dLon = DegToRad(lon2 - lon1);

    lat1 = DegToRad(lat1);
    lat2 = DegToRad(lat2);

    double a = sin(dLat/2)*sin(dLat/2) +
               sin(dLon/2)*sin(dLon/2)*cos(lat1)*cos(lat2);

    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return EARTH_RADIUS * c;
}

int ReadGpsDataFromFile(char *filename, struct VehicleGpsData *data)
{
    FILE *fp = fopen(filename, "r");

    if (!fp)
        return 0;

    int n = 0;
    long lon_raw;
    long lat_raw;

    while (fscanf(fp,
                  "%*d %*d %ld %ld %*d %*d %*s %15s",
                  &lon_raw,
                  &lat_raw,
                  data[n].line) == 3)
    {
        data[n].longitude = lon_raw / 1000000.0;
        data[n].latitude  = lat_raw / 1000000.0;

        fscanf(fp,"%*[^\n]\n");

        n++;
    }

    fclose(fp);
    return n;
}

void ComputeDistances(struct VehicleGpsData *data, int n)
{
    for (int i = 0; i < n; i++)
    {
        data[i].distance = Haversine(
            ICT_LAT,
            ICT_LON,
            data[i].latitude,
            data[i].longitude
        );
        data[i].distance = data[i].distance/1000;
    }
}

void PrintClosestVehicles(struct VehicleGpsData *data, int n)
{
    int limit = n < 15 ? n : 15;

    printf("\nClosest vehicles to ICT:\n\n");

    for (int i = 0; i < limit; i++)
    {
        printf("bus %-4s (lon %.6f lat %.6f) is %.3f km from ICT\n",
               data[i].line,
               data[i].longitude,
               data[i].latitude,
               data[i].distance);
    }
}

int CompareVehicleDistance(const void *a, const void *b)
{
    const struct VehicleGpsData *v1 = (const struct VehicleGpsData *)a;
    const struct VehicleGpsData *v2 = (const struct VehicleGpsData *)b;

    if (v1->distance < v2->distance) return -1;
    if (v1->distance > v2->distance) return 1;
    return 0;
}

