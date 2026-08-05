/**
 * File:         employee_db.c
 * Author:       Risto Heinsar
 * Created:      10.02.2015
 * Modified:     25.02.2023
 *
 * Description:  Library to work with employee struct
 */
#include "employee_db.h"
#include <string.h>
#include <stdio.h> 


// TODO: implement the functions in the header

// Print struct array

void PrintStructArray(struct Employee *staff, int len) {
    for (int i = 0; i < len; i++) {
        printf("First name: %s, Last name %s, Wage: %.2f Experience: %d\n", staff[i].fName, staff[i].lName, staff[i].wage, staff[i].yearsEmployed);
    }
}


int ComparFuncStructEmploymentLength(const void *x, const void *y)
{
    const struct Employee *e1 = (const struct Employee *)x;
    const struct Employee *e2 = (const struct Employee *)y;

    return e1->yearsEmployed - e2->yearsEmployed;
}


int ComparFuncStructFirstName(const void *x, const void *y)
{
    const struct Employee *e1 = (const struct Employee *)x;
    const struct Employee *e2 = (const struct Employee *)y;

    return strcmp(e1->fName, e2->fName);
}

int ComparFuncStructLastFirstName(const void *x, const void *y) {
    const struct Employee *e1 = (const struct Employee *)x;
    const struct Employee *e2 = (const struct Employee *)y;

    int cmp = strcmp(e1->lName, e2->lName);
    if (cmp != 0) {
        return cmp;
    }
    return strcmp(e1->fName, e2->fName);
}

