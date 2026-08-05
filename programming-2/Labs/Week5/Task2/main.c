/**
 * File:         main.c
 * Author:       Risto Heinsar
 * Created:      27.02.2026
 * Modified:     27.02.2026
 *
 * Description:  Starter code for a lab task that tracks Tallinn public
 *               transport departure times from a specified bus stop.
 * 
 *               This file contains the main logic for the program and printing
 *               the timetables.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "file_helper.h"
#include "data_processor.h"
#include "gps_processing.h"
#include "main.h"

int main(void)
{
    struct BussStopData data[100];
    struct VehicleGpsData gpsData[1000];

    int currentTime;
    int n;
    int gpsCount = 0;
    int tick = 0;

    DownloadFileToDisk(
        TALLINN_STOP_URL,
        "timetable.txt",
        CSV_TO_SPACE_DELIMITED,
        true
    );

    n = ReadDataFromFile("timetable.txt", data, &currentTime);

    DownloadFileToDisk(
        TALLINN_GPS_URL,
        "gps_data.txt",
        CSV_TO_SPACE_DELIMITED,
        true
    );

    gpsCount = ReadGpsDataFromFile("gps_data.txt", gpsData);

    if (gpsCount > 0)
    {
        ComputeDistances(gpsData, gpsCount);

        qsort(
            gpsData,
            gpsCount,
            sizeof(struct VehicleGpsData),
            CompareVehicleDistance
        );
    }

    while (1)
    {
        printf("\033[2J\033[1;1H");

        struct Time t = SplitTimeFromSecToStruct(currentTime);

        printf(
            "Current time: %d:%02d:%02d\n\n",
            t.hour,
            t.min,
            t.sec
        );

        PrintTimetable(data, n, currentTime);

        printf("\n");

        if (tick % 5 == 0)
        {
            DownloadFileToDisk(
                TALLINN_GPS_URL,
                "gps_data.txt",
                CSV_TO_SPACE_DELIMITED,
                true
            );

            gpsCount = ReadGpsDataFromFile("gps_data.txt", gpsData);

            if (gpsCount > 0)
            {
                ComputeDistances(gpsData, gpsCount);

                qsort(
                    gpsData,
                    gpsCount,
                    sizeof(struct VehicleGpsData),
                    CompareVehicleDistance
                );
            }
        }

        PrintClosestVehicles(gpsData, gpsCount);

        tick++;
        currentTime++;

        sleep(1);
    }

    return 0;
}
