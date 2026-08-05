//
// Created by giley on 2/10/2026.
//

#ifndef HEADERS_FUNCTIONS_H
#define HEADERS_FUNCTIONS_H
#include "HeadersEnums.h"
#include <stdio.h>
typedef struct {
    char id[MAX_ID];
    char firstName[MAX_NAME];
    char lastName[MAX_SURNAME];
} resident;

typedef struct {
    int day;
    int month;
    int year;
} Date;

int readPerson(FILE *file, resident *residents);
int convertIDToBirth (const char* ID, Date* dates);
int isLeapYear(int year);
int dayOfYear(const Date *d);
void printGuysWithSoonBirthday(FILE *file, Date* dates,
                               resident* residents,
                               int numOfResidents,
                               const Date* currentDate);
int isValidDay(Date currentDate);

#endif //HEADERS_FUNCTIONS_H
