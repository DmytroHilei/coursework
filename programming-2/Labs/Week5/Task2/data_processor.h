/**
 * File:        data_processor.h
 * Author:      Risto Heinsar
 * Created:     27.02.2026
 * Modified:    27.02.2026
 *
 * Description: Handles readign the data files and preprocessing of the data
 */
 
#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

struct BussStopData{
    char line[16];
    char destination[64];
    int expectedTime;
    int scheduledTime;
};

struct Time{
    int hour;
    int min;
    int sec;
};

#define ICT_LAT 59.3947
#define ICT_LON 24.6719


struct Time SplitTimeFromSecToStruct(int time);
void PrintTimetable(struct BussStopData *data, int n, int currentTime);
int ReadDataFromFile(char *filename, struct BussStopData *data, int * currentTime);

#endif
