#include <signal.h>
#include <stdio.h>

#include "Recursion.h"

int getNumber() {
    int number;
    printf("Enter a number: ");
    scanf("%d", &number);

    return number;
}

int recursionSum(int number) {
    if (number <= 0) {
        return 0;
    }

    if (number == 1) {
        return 1;
    }
    return number + recursionSum(number - 1);
}

float recursivePower(float base, int exp) {
    if (base == 0 && exp < 0) {
        printf("Error, can't divide by 0\n");
        return -1;
    }

    if (exp == 0) {
        return 1.0;
    }
    if (exp < 0) {
        return 1 / recursivePower(base, -exp);
    }
    float half = recursivePower(base, exp / 2);

    if (exp % 2 == 0) {
        return half * half;
    }
    else {
        return base * half * half; // O(log(n))
    }
}


int fibonacci(int number) {
    if (number <= 0) {
        return 0;
    }
    if (number == 1) {
        return 1;
    }
    return fibonacci(number - 1) + fibonacci(number - 2);
}

int binarySearch(int array[], int left, int right, int key) {
    if (left > right) {
        return -1;
    }
    int mid = (left + right) / 2;
    if (key == array[mid]) {
        return mid;
    }
    else if (key < array[mid]) {
        return binarySearch(array, left, mid - 1, key);
    }
    else {
        return binarySearch(array, mid + 1, right, key);
    }

}