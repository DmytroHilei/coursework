
#include <stdio.h>
#include <string.h>

#include "Headers.h"

void FixTrailingNewline(char *str) {
    while (*str) {
        if (*str == '\n') {
            *str = '\0';
            return;
        }
        str++;
    }

}
int findLastPoint(char *str) {
    int pos = -1;
    int count = 0;
    while (*str) {
        if (*str == '.') {
            pos = count;
        }
        str++;
        count++;
    }
    return pos;
}
void PrintFileCategory(int* categoryCounters, int numOfCategories) {
    const char *names[CAT_COUNTER] = {
    "Archives", "Data", "Document", "Code", "Text", "Images",
        "Other", "No extension"};

    for (int i = 0; i < numOfCategories; i++) {
        printf("%15s %8d\n", names[i], categoryCounters[i]);
    }
}

enum FileCategory GetFileCategory(char* extension)
{

    if (strcmp(extension, ".zip") == 0 ||
        strcmp(extension, ".rar") == 0 ||
        strcmp(extension, ".7z")  == 0 ||
        strcmp(extension, ".tar") == 0 ||
        strcmp(extension, ".gz")  == 0)
    {
        return CAT_ARCHIVE;
    }

    if (strcmp(extension, ".csv")  == 0 ||
             strcmp(extension, ".xls")  == 0 ||
             strcmp(extension, ".xlsx") == 0 ||
             strcmp(extension, ".ods")  == 0)
    {
        return CAT_DATA;
    }

    if (strcmp(extension, ".pdf")  == 0 ||
             strcmp(extension, ".doc")  == 0 ||
             strcmp(extension, ".docx") == 0 ||
             strcmp(extension, ".rtf")  == 0 ||
             strcmp(extension, ".odt")  == 0)
    {
        return CAT_DOCUMENTS;
    }

    if (strcmp(extension, ".c")   == 0 ||
             strcmp(extension, ".h")   == 0 ||
             strcmp(extension, ".cpp") == 0 ||
             strcmp(extension, ".hpp") == 0 ||
             strcmp(extension, ".py")  == 0)
    {
        return CAT_CODE;
    }

    if (strcmp(extension, ".txt") == 0)
    {
        return CAT_TEXT;
    }

    if (strcmp(extension, ".jpg")  == 0 ||
             strcmp(extension, ".jpeg") == 0 ||
             strcmp(extension, ".png")  == 0 ||
             strcmp(extension, ".svg")  == 0)
    {
        return CAT_IMAGES;
    }

    return CAT_OTHER;

}

