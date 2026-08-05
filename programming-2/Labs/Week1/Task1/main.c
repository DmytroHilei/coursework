/**
* File:           t1_swap_template.c
 * Author:         Risto Heinsar
 * Created:        19.08.2014
 * Last edit:      29.01.2023
 *
 * Description:    Lab task template.
 *                 Read and swap two numbers using pointers.
 */

#include <stdio.h>

#include "Headers.h"
// TODO: Write your prototypes for the functions here



int main(void)
{
    int x, y;
    int* x_Adress = &x;
    int* y_Adress = &y;

    // TODO: Print the addresses of the variables   DONE
    printf("Addresses in function %s\n", __FUNCTION__);
    printf("x @ %p\ny @ %p\n", x_Adress, y_Adress);

    // TODO: Call your first function that scans for the values
    readValues(x_Adress, y_Adress);

    // Print out the values to verify that the numbers are scanned correctly.
    printf("\n\nOriginal values in %s\nx = %d\ny = %d\n\n", __FUNCTION__, x, y);

    // TODO: Call the function that swaps the values
    swap(x_Adress, y_Adress);

    // Print out the values to confirm that swapping worked
    printf("Updated values in %s\nx = %d\ny = %d\n", __FUNCTION__, x, y);
    return 0;
 }

