#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Recursion.h"

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        printf("Please enter a number\n");
    }

    int base = atoi(argv[1]);
    int exponent = atoi(argv[2]);

    clock_t start = clock();

    float power = recursivePower(base, exponent);

    clock_t end = clock();
    float time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("The power (%d, %d) = %.2f\n", base, exponent, power);
    printf("Time spent = %.2f\n", time_spent);

    return 0;

}
