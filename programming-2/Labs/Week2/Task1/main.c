#include <stdio.h>

#include "Headers.h"

int main(void) {
    char name[MAX_FILE_LEN];

    int categoryCounter[CAT_COUNTER] = {0}; //Set everything to 0

    while (fgets(name, MAX_FILE_LEN, stdin) != NULL) {
        FixTrailingNewline(name);
        int pos = findLastPoint(name);
        enum FileCategory type;

        if (pos == -1) {
            type = CAT_NO_EXT;
        }
        else {
            char* extension = name + pos;
            type = GetFileCategory(extension);
        }
        categoryCounter[type]++;
    }
    PrintFileCategory(categoryCounter, CAT_COUNTER);
    return 0;
}

