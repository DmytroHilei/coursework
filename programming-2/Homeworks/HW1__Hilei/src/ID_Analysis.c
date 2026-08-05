
#include <ctype.h>
#include <string.h>

#include "Headers_Functions.h"
#include "HeadersEnums.h"


/*
 * Function:    convertIDToBirth
 * Description: Converts an Estonian ID code to a Date struct
 *              containing the person's birth day, month and year.
 * Input:       ID    - pointer to a string containing
 *                      the 11-digit Estonian ID code
 *              dates - pointer to a Date struct to store the result
 * Output:      Returns SUCCESS_CONVERTING_BIRTH_DAY on success,
 *              or an error code (WRONG_ID_LENGTH, WRONG_DAY, etc.)
 *              on failure. The dates struct is populated on success.
 */


int convertIDToBirth (const char* ID, Date* dates)
{
    if (strlen(ID) != ID_LENGTH)
    {
        return WRONG_ID_LENGTH;
    }
    for (int i = 0; i < ID_LENGTH; i++) {
        if (!isdigit(ID[i])) return WRONG_DATA_FORMAT;
    }

    int day = (ID[FIRST_DAY_INDEX] - '0') * 10 + (ID[SECOND_DAY_INDEX] - '0');
    if (day < MIN_DAY || day > MAX_DAY)
    {
        return WRONG_DAY;
    }

    int month = (ID[FIRST_MONTH_INDEX] - '0') * 10 + (ID[SECOND_MONTH_INDEX] - '0');
    if (month < MIN_MONTH || month > MAX_MONTH)
    {
        return WRONG_MONTH;
    }

    int year = (ID[FIRST_YEAR_INDEX] - '0') * 10 + (ID[SECOND_YEAR_INDEX] - '0');

    if (ID[CENTURY_INDEX] == '1' || ID[CENTURY_INDEX] == '2') year += 1800;
    else if (ID[CENTURY_INDEX] == '3' || ID[CENTURY_INDEX] == '4') year += 1900;
    else if (ID[CENTURY_INDEX] == '5' || ID[CENTURY_INDEX] == '6') year += 2000;

    if (year < MIN_YEAR || year > MAX_YEAR)
    {
        return WRONG_YEAR;
    }

    dates -> year = year;
    dates -> month = month;
    dates -> day = day;

    return SUCCESS_CONVERTING_BIRTH_DAY;
}

/*
 * Function:    isValidDay
 * Description: Validates the day value of a given Date struct,
 *              accounting for the correct number of days in each
 *              month and leap year correction for February.
 *
 * Input:       currentDate - a Date struct containing day, month and year
 *
 * Output:      Returns GOOD_DAY if the day is valid,
 *              WRONG_MONTH if the month is out of range (1-12),
 *              WRONG_DAY if the day is out of range for the given month.
 */


int isValidDay(Date currentDate)
{
    static const int daysInMonth[] =
    {
        31, // Jan
        28, // Feb
        31, // Mar
        30, // Apr
        31, // May
        30, // Jun
        31, // Jul
        31, // Aug
        30, // Sep
        31, // Oct
        30, // Nov
        31  // Dec
    };

    if (currentDate.month < MIN_MONTH ||
        currentDate.month > MAX_MONTH)
    {
        return WRONG_MONTH;
    }

    int maxDay = daysInMonth[currentDate.month - 1];

    /* leap year correction */
    if (currentDate.month == 2 &&
        isLeapYear(currentDate.year))
    {
        maxDay = 29;
    }

    if (currentDate.day < MIN_DAY ||
        currentDate.day > maxDay)
    {
        return WRONG_DAY;
    }

    return GOOD_DAY;
}


