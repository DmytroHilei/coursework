


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Header.h"

int main()
{

    PersonArray arr = ReadData("v1.txt");

    if (!arr.data) {
        printf("Failed to read data\n");
        return 1;
    }

    qsort(arr.data, arr.used, sizeof(Person), cmp);

    printBestDudes(arr);

    FreeData(arr);

    return 0;
}
