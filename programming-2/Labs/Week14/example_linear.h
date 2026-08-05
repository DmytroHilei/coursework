/**
 * File:        example_linear.h
 * Author:      Risto Heinsar
 * Created:     25.04.2025
 * Last edit:   25.04.2025
 *
 * Description: Defines macros and functions for the linear data structure
 */
#ifndef NAME_LIST_LINEAR
#define NAME_LIST_LINEAR

#define MAX_NAME             32
#define NAME_NOT_IN_ARRAY    -1

/* Specifies expected command line arguments */
#define ARGS_CNT              2
#define ARGS_EXEC       argv[0]
#define ARGS_FILE_NAME  argv[1]

#include <time.h>

typedef struct
{
    char name[MAX_NAME];
    int count;
} Person;

typedef struct
{
    clock_t start;
    clock_t read;
    clock_t print;
    clock_t free;
} TimePoints;


/**
 * Description:    Gets the index of the name if it is a part of the array
 *
 * Parameters:     db - Array of names to compare against
 *                 n - Number of names in the array
 *                 name - needle, checked if it is a part of the array
 *
 * Return:         Index of the name if it is found in the array.
 *                 NAME_NOT_IN_ARRAY is returned if it is not found.
 */
int GetNameIndex(Person *db, int n, char *name);


/**
 * Description:    Reads the data from a file and stores it to a dynamic array,
 *
 * Parameters:     db - pointer to the data will be assigned here after reading
 *                 file - string with the name (path) of the file to read from
 *
 * Return:         Number of nonrecurrent names
 */
int ReadData(Person **db, char *file);


/**
 * Description:    prints out the number of members, followed by the
 *                 members themselves.
 *
 * Parameters:     db - data array with names
 *                 n - number of members in the data array
 *
 * Return:         none
 */
void PrintData(Person *db, int n, const char *file);



void FreeData(Person *db);
void PrintTime(TimePoints t);

#endif // NAME_LIST_LINEAR
