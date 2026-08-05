#include <stdio.h>
#include <stdlib.h>

#include "Headers.h"
int ReadValuesFromFile(int *array, int size, char *InputFile)
{
    int count = 0;
    FILE *file = fopen(InputFile, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", InputFile);
        return EXIT_FAILURE;
    }

    while (count < size) {
        int ret = fscanf(file, "%d\n", array + count);
        if (ret != 1) {
            break;
        }
        count++;
    }

    fclose(file);
    return count;
}



void printAddresesAndNumbers(int *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%p, %d\n", array + i, *(array + i));
    }
}

void FindMinMax(int *array, int size, int **min, int **max)
{
    *min = array;
    *max = array;

    for (int i = 1; i < size; i++) {
        if (*(array + i) < **min) {
            *min = array + i;
        }
        if (*(array + i) > **max) {
            *max = array + i;
        }
    }
}



