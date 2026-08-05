/**
 * File:        data_processor.c
 * Author:      Risto Heinsar
 * Created:     27.02.2026
 * Modified:    27.02.2026
 *
 * Description: Handles readign the data files and preprocessing of the data
 */
 
#include "data_processor.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>

int ReadDataFromFile(char *filename, struct BussStopData *data, int * currentTime) {
    printf("DEBUG currentTime = %d\n", *currentTime);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Cannot open the file\n");
        exit(1);
    }

    int ret = fscanf(fp, "%*s %*s %*s %*s %d %*[^\n]\n", currentTime);

    printf("ret = %d\n", ret);

    fscanf(fp,"%*[^\n]\n");

    int n = 0;

    while (fscanf(fp,"%*s %[^ ] %d %d %[^ ]",
       data[n].line,
       &data[n].expectedTime,
       &data[n].scheduledTime,
       data[n].destination) == 4)
    {
        fscanf(fp,"%*[^\n]\n");
        n++;
    }

    fclose(fp);
    return n;
}
struct Time SplitTimeFromSecToStruct(int time) {
    struct Time t;
    t.hour = time /3600;
    t.min = (time % 3600) /60;
    t.sec = (time - t.hour*3600 - t.min*60);

    return t;
}

void PrintTimetable(struct BussStopData *data, int n, int currentTime)
{
    for (int i = 0; i < n; i++)
    {
        int timeLeft = data[i].expectedTime - currentTime;

        if (timeLeft < 0)
            continue;

        struct Time dep = SplitTimeFromSecToStruct(data[i].expectedTime);
        struct Time left = SplitTimeFromSecToStruct(timeLeft);

        printf("Bus nr %-4s to %-20s %02d:%02d:%02d (time left: %02d:%02d)",
               data[i].line,
               data[i].destination,
               dep.hour, dep.min, dep.sec,
               left.min, left.sec);

        int delay = data[i].expectedTime - data[i].scheduledTime;

        if (delay > 60)
        {
            struct Time del = SplitTimeFromSecToStruct(delay);
            printf("   DELAYED %d min %d sec", del.min, del.sec);
        }

        printf("\n");
    }
}