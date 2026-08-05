/**
 * File:         emplyee_db.h
 * Author:       Risto Heinsar
 * Created:      10.02.2015
 * Modified:     26.01.2026
 *
 * Description:  Library description for employee data helper functions
 */
 
#ifndef EMPLOYEE_DB_H
#define EMPLOYEE_DB_H

#define LEN 32

struct Employee
{
    char fName[LEN];
    char lName[LEN];
    float wage;
    int yearsEmployed;
};

/* Print function for employees*/
void PrintStructArray(struct Employee *staff, int len);

/* Qsort compare functions for sorting based on one member */
int ComparFuncStructEmploymentLength(const void *x, const void *y);
int ComparFuncStructFirstName(const void *x, const void *y);

/* Qsort compare functions for sorting based on two members */
int ComparFuncStructLastFirstName(const void *x, const void *y);

#endif //EMPLOYEE_DB_H
