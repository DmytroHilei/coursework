//
// Created by giley on 2/7/2026.
//

#ifndef HEADERS_H
#define HEADERS_H

#define MAX_FILE_LEN 256

enum FileCategory {
    CAT_ARCHIVE,
    CAT_DATA,
    CAT_DOCUMENTS,
    CAT_CODE,
    CAT_TEXT,
    CAT_IMAGES,
    CAT_OTHER,
    CAT_NO_EXT,
    CAT_COUNTER
};

void FixTrailingNewline(char *str);
int findLastPoint(char *str);
void PrintFileCategory(int* categoryCounters, int numOfCategories);
enum FileCategory GetFileCategory(char* extension);
#endif //HEADERS_H
