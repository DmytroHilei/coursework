#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Recursion.h"

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Please enter a number\n");
    }

    int number = atoi(argv[1]);

    clock_t start = clock();

    int sum = recursionSum(number);

    clock_t end = clock();

    printf("The sum (%d) =  %d\n", number, sum);

    float time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time spent = %.2f\n", time_spent);

    return 0;

}
