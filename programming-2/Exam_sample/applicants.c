#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "applicants.h"

/* Allocates a private copy of a string. */
static char *CopyString(const char *src)
{
    char *dst = malloc(strlen(src) + 1);
    if (dst == NULL) return NULL;
    strcpy(dst, src);
    return dst;
}

double AdmissionPoints(const Applicant *a)
{
    return (a->math + a->physics + a->electronics) / 3.0;
}

void FreeApplicants(Applicant *arr, int count)
{
    for (int i = 0; i < count; i++) {
        free(arr[i].firstName);
        free(arr[i].lastName);
    }
    free(arr);
}

/* Appends one applicant, doubling capacity when the array is full. */
static int AppendApplicant(Applicant **arr, int *count, int *cap, Applicant item)
{
    if (*count == *cap) {
        int newCap = (*cap == 0) ? 4 : (*cap * 2);
        Applicant *tmp = realloc(*arr, sizeof(Applicant) * newCap);
        if (tmp == NULL) return 0;          /* old *arr still valid */
        *arr = tmp;
        *cap = newCap;
    }
    (*arr)[(*count)++] = item;
    return 1;
}

int ReadApplicants(const char *fileName, Applicant **outArr, int *outCount)
{
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) return 0;

    Applicant *arr = NULL;
    int count = 0, cap = 0;

    char firstBuf[64], lastBuf[64];
    int math, physics, electronics;
    char payChar;

    /* Line format:  First Last  math physics electronics  y/n */
    while (fscanf(fp, "%63s %63s %d %d %d %c",
                  firstBuf, lastBuf, &math, &physics, &electronics, &payChar) == 6) {
        Applicant a;
        a.firstName = CopyString(firstBuf);
        a.lastName  = CopyString(lastBuf);
        if (a.firstName == NULL || a.lastName == NULL) {
            free(a.firstName);
            free(a.lastName);
            fclose(fp);
            FreeApplicants(arr, count);
            return 0;
        }
        a.math        = math;
        a.physics     = physics;
        a.electronics = electronics;
        a.agreedToPay = (payChar == 'y' || payChar == 'Y');

        if (!AppendApplicant(&arr, &count, &cap, a)) {
            free(a.firstName);
            free(a.lastName);
            fclose(fp);
            FreeApplicants(arr, count);
            return 0;
        }
    }

    fclose(fp);
    *outArr   = arr;
    *outCount = count;
    return 1;
}