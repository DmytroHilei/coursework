/**
 * File:        example_linear.c
 * Author:      Risto Heinsar
 * Created:     21.04.2021
 * Last edit:   25.04.2025
 *
 * Description: Sample code to find unique strings in a file.
 *              Uses a dynamically expanded array to store data and linear
 *              search for finding unique values.
 *
 * Usage:       ./program_name name_of_input_file
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "example_linear.h"

int main(int argc, char **argv)
{
    Person *data = NULL;
    TimePoints timePoints;

    if (argc != ARGS_CNT)
    {
        fprintf(stderr, "Usage: %s data_file!\n", ARGS_EXEC);
        exit(EXIT_FAILURE);
    }

    timePoints.start = clock();

    int n = ReadData(&data, ARGS_FILE_NAME);

    timePoints.read = clock();

    PrintData(data, n, "linear_output.txt");

    timePoints.print = clock();

    FreeData(data);

    timePoints.free = clock();

    printf("Found %d names\n", n);
    PrintTime(timePoints);

    return EXIT_SUCCESS;
}


/**
 * Prints all elements in the array
 */

void PrintData(Person *db, int n, const char *file)
{
    FILE *fp = fopen(file, "w");
    if (fp == NULL)
    {
        perror("Opening of output file failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%s %u\n", db[i].name, db[i].count);
    }

    fclose(fp);
}

/**
 * Reads strings from the provided file to a dynamic array. Repeating strings
 * will be ignored.
 */
int ReadData(Person **db, char *file)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        perror("Opening of input file failed");
        exit(EXIT_FAILURE);
    }

    /* Reading buffer */

    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    char combinedName[MAX_NAME];

    /* Number of nonrecurrent names stored in the array */
    int nameCnt = 0;

    Person *pData = NULL;
    while (fscanf(fp, "%*s %31s %31s %*s", firstName, lastName) == 2)
    {
        snprintf(combinedName, sizeof(combinedName), "%s %s", firstName, lastName);

        /* Get the index of the name if it's already in the array */
        int nameIdx = GetNameIndex(pData, nameCnt, combinedName);

        /* Only add the name to the array if it doesn't exist there yet '*/
        if (nameIdx == NAME_NOT_IN_ARRAY)
        {
            /* Expand the array */
            Person *pTemp = realloc(pData, sizeof(Person) * ((unsigned)nameCnt + 1));
            if (pTemp == NULL)
            {
                fprintf(stderr, "Error expanding memory!\n");
                free(pData);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            pData = pTemp;

            /* Copies the new string into the data struct */
            strcpy(pData[nameCnt].name, combinedName);

            pData[nameCnt].count = 1;
            /* Increases the count of unique names */
            nameCnt++;
        }
        else {
            pData[nameIdx].count++;
        }
    }

    fclose(fp);

    *db = pData;

    return nameCnt;
}

/**
 * Checks if the provided string already exists in the struct array
 */
int GetNameIndex(Person *db, int n, char *name)
{
    for (int i = 0; i < n; i++)
    {
        if (!strcmp(name, db[i].name))
        {
            /* The name was foumd in the data, returning name index */
            return i;
        }
    }

    /* The name was not found in the struct array */
    return NAME_NOT_IN_ARRAY;
}

void FreeData(Person *db)
{
    free(db);
}

void PrintTime(TimePoints t)
{
    printf("Reading:  %9.6lf s\n", (double)(t.read - t.start) / CLOCKS_PER_SEC);
    printf("Printing: %9.6lf s\n", (double)(t.print - t.read) / CLOCKS_PER_SEC);
    printf("Freeing:  %9.6lf s\n", (double)(t.free - t.print) / CLOCKS_PER_SEC);
    printf("Total:    %9.6lf s\n", (double)(t.free - t.start) / CLOCKS_PER_SEC);
}