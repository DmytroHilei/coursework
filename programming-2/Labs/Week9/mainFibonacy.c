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

    int fibon = fibonacci(number);

    clock_t end = clock();

    printf("The fib(%d) = %d\n", number, fibon);

    float time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time spent %.2f s", time_spent);

    return 0;

}
