

/**
 * Description:    Scan for two integer values and stores the values to the
 *                 locations passed as pointers. It will also print
 *                 out the two locations where the values are stored.
 *
 * Parameters:     a - pointer to the first value
 *                 b - pointer to the second value
 *
 * Return:         none
 */

// TODO: Write your function here!

#include <stdio.h>

#include "Headers.h"


void readValues (int* x, int* y) {
    int type;

    printf("Addresses in function %s\n", __FUNCTION__);
    printf("x @ %p\ny @ %p\n", x, y);

    do {
        printf("Please, enter value x:");
        type = scanf("%d", x);
        if (type != 1) {
            fprintf(stderr, "Wrong input, try again");
        }
        while (getchar() != '\n');
    } while (type != 1);
    putchar('\n');

    do {
        printf("Please, enter value y:");
        type = scanf("%d", y);
        if (type != 1) {
            fprintf(stderr, "Wrong input, try again\n");
        }
        while (getchar() != '\n');
    } while (type != 1);

    putchar('\n');
}

/**
 * Description:    Swaps the two values pointed by the pointers passed to this
 *                 function. It will also print out the locations of the two
 *                 values that are being swapped.
 *
 * Parameters:     a - pointer to the first value
 *                 b - pointer to the second value
 *
 * Return:         none
 */

// TODO: Write your function here

void swap(int* x, int* y) {

    printf("Addresses in function %s\n", __FUNCTION__);
    printf("x @ %p\ny @ %p\n", x, y);

    int temp = *x;
    *x = *y;
    *y = temp;
}
