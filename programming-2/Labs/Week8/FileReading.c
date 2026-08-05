#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Header.h"

PersonArray ReadData(const char *fileName)
{
    PersonArray arr;
    arr.used = 0;
    arr.capacity = CAPACITY;

    FILE *f = fopen(fileName, "r");
    if (!f) {
        printf("File error\n");
        arr.data = NULL;
        return arr;
    }

    arr.data = malloc(arr.capacity * sizeof(Person));
    if (!arr.data) {
        printf("Memory error\n");
        fclose(f);
        return arr;
    }

    char fNameBuf[100];
    char lNameBuf[100];
    char currBuf[LEN_CURRICULUM + 1];
    int idx;
    float pts;

    while (fscanf(f, "%d %99s %99s %4s %f",
                  &idx, lNameBuf, fNameBuf, currBuf, &pts) == 5)
    {
        // expand
        if (arr.used == arr.capacity) {
            arr.capacity *= 2;
            Person *temp = realloc(arr.data, arr.capacity * sizeof(Person));
            if (!temp) {
                printf("Memory error\n");
                goto error;
            }
            arr.data = temp;
        }

        // allocate strings
        arr.data[arr.used].fName = malloc(strlen(fNameBuf) + 1);
        arr.data[arr.used].lName = malloc(strlen(lNameBuf) + 1);

        if (!arr.data[arr.used].fName || !arr.data[arr.used].lName) {
            free(arr.data[arr.used].fName);
            free(arr.data[arr.used].lName);
            printf("Memory error\n");
            goto error;
        }

        // copy
        arr.data[arr.used].index = idx;
        strcpy(arr.data[arr.used].fName, fNameBuf);
        strcpy(arr.data[arr.used].lName, lNameBuf);
        strcpy(arr.data[arr.used].curriculum, currBuf);
        arr.data[arr.used].points = pts;

        arr.used++;
    }

    // shrink
    if (arr.used > 0) {
        Person *temp = realloc(arr.data, arr.used * sizeof(Person));
        if (temp)
            arr.data = temp;
    }

    fclose(f);
    return arr;

error:
    for (int i = 0; i < arr.used; i++) {
        free(arr.data[i].fName);
        free(arr.data[i].lName);
    }
    free(arr.data);
    fclose(f);

    arr.data = NULL;
    arr.used = 0;
    arr.capacity = 0;
    return arr;
}



int cmp(const void *a, const void *b)
{
    float p1 = ((Person*)a)->points;
    float p2 = ((Person*)b)->points;

    if (p2 > p1) return 1;
    if (p2 < p1) return -1;
    return 0;
}

void printBestDudes(PersonArray arr)
{
    printf("Ind   fName        lName        Code   Points\n");

    int iacb = 0, earb = 0, mveb = 0;

    for (int i = 0; i < arr.used; i++) {
        Person *s = &arr.data[i];

        if (strcmp(s->curriculum, "IACB") == 0 && iacb < 7) {
            PrintStudent(s);
            iacb++;
        }
        else if (strcmp(s->curriculum, "EARB") == 0 && earb < 7) {
            PrintStudent(s);
            earb++;
        }
        else if (strcmp(s->curriculum, "MVEB") == 0 && mveb < 7) {
            PrintStudent(s);
            mveb++;
        }

        if (iacb == 7 && earb == 7 && mveb == 7)
            break;
    }

    printf("\nEARB: %d / 7\n", earb);
    printf("IACB: %d / 7\n", iacb);
    printf("MVEB: %d / 7\n", mveb);
}
void PrintStudent(Person *s)
{
    printf("%4d   %-12s %-12s %-5s %5.1f\n",
           s->index,
           s->fName,
           s->lName,
           s->curriculum,
           s->points);
}

void FreeData(PersonArray arr)
{
    for (int i = 0; i < arr.used; i++) {
        free(arr.data[i].fName);
        free(arr.data[i].lName);
    }
    free(arr.data);
}

