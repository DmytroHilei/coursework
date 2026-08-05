/**
 * File:         qsort_task.c
 * Author:       Risto Heinsar
 * Created:      10.02.2015
 * Modified:     26.01.2026
 *
 * Description:  Base code for qsort lab task.
 */
#include <stdio.h>
#include <stdlib.h>
#include "qsort_task.h"
#include "employee_db.h"
#include "array_helpers.h"

int main(void)
{
    int intArr[] = {15, 25, 3, 19, 22, 17, -54, 0, 9};
    int nInts = sizeof(intArr) / sizeof(int);
    
    float floatArr[] = {76.60f, 11.2f, 76.40f, 235.4f, 76.50f, 341.6f};
    int nFloat = sizeof(floatArr)/sizeof(float);

    struct Employee workForce[] = {{"Sirje",  "Vakra",  15.4f,  0},
                                   {"Mark",   "Rebane", 10.3f,  5},
                                   {"Doris",  "Rebane", 10.2f,  3},
                                   {"Anneli", "Oja",     7.3f,  0},
                                   {"Andres", "Rebane", 22.5f, 10}};
                                   
    int numOfWorkers = sizeof(workForce)/sizeof(struct Employee);

    while (1)
    {
        switch (ChooseMenuAction())
        {
            case 1:
                qsort(intArr, nInts, sizeof(int), ComparFuncInt);
                puts("Integer array: ");
                PrintIntegerArray(intArr, nInts);
                
                break;
            case 2:
                qsort(floatArr, nFloat, sizeof(float), ComparFuncFloat);
                puts("Decimal array: ");
                PrintFloatArray(floatArr, nFloat);
                
                break;
            case 3:
                //~ Sort and print the structures based on employment length
                qsort(workForce, numOfWorkers, sizeof(struct Employee), ComparFuncStructEmploymentLength);
                puts("Employees sorted by employment length: ");
                PrintStructArray(workForce, numOfWorkers);
                
                break;
            case 4:
                //~ Sort and print the structures based on first name
                qsort(workForce, numOfWorkers, sizeof(struct Employee), ComparFuncStructFirstName);
                puts("Employees sorted by employment first name: ");
                PrintStructArray(workForce, numOfWorkers);
                
                break;
            case 5:
                //~ Sort and print the structures based on last name.
                //~ If they are the same, sort them by first name.
                qsort(workForce, numOfWorkers, sizeof(struct Employee), ComparFuncStructLastFirstName);
                puts("Employees sorted by employment last and first name: ");
                PrintStructArray(workForce, numOfWorkers);
                
                
                break;
            case 0:
                /* Exit */
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "Error: Unknown command\n");
        }
    }
    return EXIT_SUCCESS;
}


unsigned ChooseMenuAction(void)
{
    unsigned ans;
    printf("\n\nSelect your action!\n\n");
    printf("1. Sort and display integer array (intArr)\n");
    printf("2. Sort and display float array (floatArr)\n");
    printf("3. Sort and display structures ordered by employment length\n");
    printf("4. Sort and display structures ordered first name\n");
    printf("5. Sort and display structures ordered by last and first.\n");
    printf("0. Exit\n\n");
    
    printf("> ");
    scanf("%u", &ans);
    return ans;
}
