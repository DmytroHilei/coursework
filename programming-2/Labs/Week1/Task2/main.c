#include <stdio.h>
#include <stdlib.h>

#include "Headers.h"

#define LEN 10

int main(int argc, char *argv[])
{
    // Check command line arguments
    if (argc < 2) {
        fprintf(stderr, "Error: input file name missing\n");
        return EXIT_FAILURE;
    }

    char *inputFile = argv[1];

    // Allocate memory for array
    int *array = malloc(sizeof(int) * LEN);
    if (array == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        free(array);
        return EXIT_FAILURE;
    }

    // Read numbers from file
    int count = ReadValuesFromFile(array, LEN, inputFile);
    if (count <= 0) {
        fprintf(stderr, "Error: no integers were read from file\n");
        free(array);
        return EXIT_FAILURE;
    }

    // Show base address
    printf("Array address in main(): %p\n\n", array);

    // Print all read numbers
    printf("Printing data from file\n");
    printAddresesAndNumbers(array, count);

    // --- Find min and max ---
    int *minPtr = NULL;
    int *maxPtr = NULL;
    FindMinMax(array, count, &minPtr, &maxPtr);

    printf("\nMin: %d\nMax: %d\n", *minPtr, *maxPtr);

    // --- Part W01-3: demonstrate array as pointer ---
    if (count < 10) {
        fprintf(stderr, "Error: not enough elements for range printing\n");
        free(array);
        return EXIT_FAILURE;
    }

    printf("\nArray print from 0 to 9\n");
    printAddresesAndNumbers(array, 10);

    printf("\nArray print from 0 to 4\n");
    printAddresesAndNumbers(array, 5);

    printf("\nArray print from 3 to 7\n");
    printAddresesAndNumbers(array + 3, 5);

    // --- Extra task: print min/max locations ---
    int minIndex = (int)(minPtr - array);
    int maxIndex = (int)(maxPtr - array);

    printf("\nMin %d; address %p; index %d\n",
           *minPtr, (void*)minPtr, minIndex);

    printf("Max %d; address %p; index %d\n",
           *maxPtr, (void*)maxPtr, maxIndex);

    free(array);
    return EXIT_SUCCESS;
}

