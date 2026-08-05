//
// Created by giley on 4/18/2026.
//

#include <stdio.h>

#include "Recursion.h"

int main() {
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = 6;
    int key = 7;

    int found = binarySearch(arr, 0, n - 1, key);

    if (found)
        printf("Found\n");
    else
        printf("Not found\n");

    return 0;
}
