//
// Created by giley on 3/31/2026.
//

#ifndef HEADER_H
#define HEADER_H

#define LEN_CURRICULUM 4


#define FILE_NAME_FIRST "v1.txt"
#define FILE_NAME_SECOND "v2.txt"

#define CAPACITY 8
typedef struct {
    int index;
    char *fName;
    char *lName;
    char curriculum[5];
    float points;
} Person;

typedef struct {
    Person *data;
    int used;
    int capacity;
} PersonArray;



void PrintStudent(Person *s);
void printBestDudes(PersonArray arr);
int cmp(const void *a, const void *b);
PersonArray ReadData(const char *fileName);
void FreeData(PersonArray arr);

#endif //HEADER_H
