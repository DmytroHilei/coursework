#include <stdio.h>

#include "Headers_Functions.h"
#include "HeadersEnums.h"
#include "Settings.h"

/*
 * Function:    isLeapYear
 * Description: Determines whether a given year is a leap year.
 *              A year is a leap year if it is divisible by 4,
 *              except for century years, which must be divisible
 *              by 400.
 *
 * Input:       year - the year to check (e.g. 2024)
 *
 * Output:      Returns 1 if the year is a leap year, 0 otherwise.
 */

int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) ||
           (year % 400 == 0);
}

/*
 * Function:    dayOfYear
 * Description: Calculates the day number within the year (1-366)
 *              for a given date. Uses a precomputed lookup table
 *              of cumulative days before each month. Adds one extra
 *              day for dates after February in a leap year.
 *
 * Input:       d - pointer to a Date struct containing day, month and year
 *
 * Output:      Returns the day of the year as an integer (1-365, or
 *              1-366 for a leap year).
 */


int dayOfYear(const Date *d)
{
    static const int daysBeforeMonth[] =
    {
        0,   31,  59,  90, 120, 151,
        181, 212, 243, 273, 304, 334
    };

    int day = daysBeforeMonth[d->month - 1] + d->day;

    if (d->month > 2 && isLeapYear(d->year))
        day++;

    return day;
}


/*
 * Function:    printGuysWithSoonBirthday
 * Description: Finds and prints all residents whose birthday falls
 *              within the next MAX_DISTANCE_TO_BIRTHDAY days from
 *              the given current date. Handles year wrap-around for
 *              birthdays in the next year. Marks birthdays occurring
 *              today with "<-- TODAY!" and milestone ages (multiples
 *              of 10) with "[MILESTONE]". Output is directed to
 *              terminal and/or file based on global settings.
 *              If no upcoming birthdays are found, a message is
 *              printed instead.
 *
 * Input:       file           - pointer to an opened output FILE,
 *                               or NULL if file output is disabled
 *              dates          - array of Date structs with birth dates
 *              residents      - array of resident structs with names
 *              numOfResidents - number of entries in both arrays
 *              currentDate    - pointer to a Date struct representing
 *                               today's date
 * Output:      None
 */



void printGuysWithSoonBirthday(FILE* file, Date* dates,
                               resident* residents,
                               int numOfResidents,
                               const Date* currentDate)
{

    int today = dayOfYear(currentDate);
    int daysInYear = isLeapYear(currentDate->year) ? LEN_OF_LEAP_YEAR : LEN_OF_YEAR;
    int validBirthdays = 0;

    for (int i = 0; i < numOfResidents; i++) {
        int birthday = dayOfYear(dates + i);

        int diff = birthday - today;

        int isNextYearBirthday = 0;
        if (diff < 0) {
            diff += daysInYear;
            isNextYearBirthday = 1;
        }

        if (diff >= 0 && diff <= MAX_DISTANCE_TO_BIRTHDAY)
        {
            validBirthdays++;
            int age;
            if (isNextYearBirthday) {
                age = currentDate->year - dates[i].year + 1;
            }
            else {
                age = currentDate->year - dates[i].year;
            }


            if (settings.printToTerminal) {
                printf("%s %s - %02d.%02d.%04d (turns %d)",
                       residents[i].firstName,
                       residents[i].lastName,
                       dates[i].day,
                       dates[i].month,
                       dates[i].year,
                       age);
            }

            if (settings.writeToFile && file != NULL) {
                fprintf(file, "%s %s - %02d.%02d.%04d (turns %d)",
                       residents[i].firstName,
                       residents[i].lastName,
                       dates[i].day,
                       dates[i].month,
                       dates[i].year,
                       age);
            }

            if (diff == 0) {
                if (settings.printToTerminal) {
                    printf("  <-- TODAY!");
                }
                if (settings.writeToFile && file != NULL) {
                    fprintf(file, "  <-- TODAY!");
                }

            }


            if (age % 10 == 0) {
                if (settings.printToTerminal) {
                    printf("  [MILESTONE]");
                }
                if (settings.writeToFile && file != NULL) {
                    fprintf(file, "  [MILESTONE]");
                }

            }

            if (settings.printToTerminal) {
                printf("\n");
            }
            if (settings.writeToFile && file != NULL) {
                fprintf(file, "\n");
            }

        }
    }
    if (validBirthdays == 0)
    {
        if (settings.printToTerminal) {
            printf("No upcoming birthdays in the next 14 days.\n");
        }
        if (settings.writeToFile && file != NULL) {
            fprintf(file, "No upcoming birthdays in the next 14 days.\n");
        }

    }
}
