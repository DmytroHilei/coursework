#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "applicants.h"

#define DATA_FILE  "training_applicants.txt"
#define FREE_SEATS 5

/* ---------- ranking by admission points (descending) ---------- */

/* qsort cannot pass extra context, so the array being ranked is
   shared with the comparator through this file-scope pointer. */
static const Applicant *gRankBase;

static int CompareByPointsDesc(const void *a, const void *b)
{
    int indexA = *(const int *)a;
    int indexB = *(const int *)b;
    double pointsA = AdmissionPoints(&gRankBase[indexA]);
    double pointsB = AdmissionPoints(&gRankBase[indexB]);

    if (pointsA < pointsB) return 1;        /* no x - y: avoids overflow */
    if (pointsA > pointsB) return -1;
    return 0;
}

/* Returns a new array of applicant indexes, best score first.
   The applicant array itself is never reordered, so indexes stay
   stable between menu choices. Caller must free() the result. */
static int *BuildRanking(const Applicant *arr, int count)
{
    int *order = malloc(sizeof(int) * count);
    if (order == NULL) return NULL;
    for (int i = 0; i < count; i++) {
        order[i] = i;
    }
    gRankBase = arr;
    qsort(order, count, sizeof(int), CompareByPointsDesc);
    return order;
}

/* ---------- printing ---------- */

static void PrintApplicant(const Applicant *a)
{
    printf("%-11s %-11s  math=%3d  phys=%3d  el=%3d  avg=%6.2f  pay=%s\n",
           a->firstName, a->lastName,
           a->math, a->physics, a->electronics,
           AdmissionPoints(a),
           a->agreedToPay ? "yes" : "no");
}

/* ---------- subtask 1 ---------- */
static void PrintAllData(const Applicant *arr, int count)
{
    printf("\n--- All applicants ---\n");
    for (int i = 0; i < count; i++) {
        printf("%2d. ", i);
        PrintApplicant(&arr[i]);
    }
}

/* ---------- subtask 2 ---------- */
static void HighestAverageSubject(const Applicant *arr, int count)
{
    double mathSum = 0, physicsSum = 0, electronicsSum = 0;
    for (int i = 0; i < count; i++) {
        mathSum        += arr[i].math;
        physicsSum     += arr[i].physics;
        electronicsSum += arr[i].electronics;
    }
    double mathAvg        = mathSum / count;
    double physicsAvg     = physicsSum / count;
    double electronicsAvg = electronicsSum / count;

    printf("\n--- Average grade per subject ---\n");
    printf("  math:        %6.2f\n", mathAvg);
    printf("  physics:     %6.2f\n", physicsAvg);
    printf("  electronics: %6.2f\n", electronicsAvg);

    const char *best = "math";
    double bestAvg = mathAvg;
    if (physicsAvg > bestAvg)     { bestAvg = physicsAvg;     best = "physics"; }
    if (electronicsAvg > bestAvg) { bestAvg = electronicsAvg; best = "electronics"; }
    printf("Highest average: %s (%.2f)\n", best, bestAvg);
}

/* ---------- subtask 3 ---------- */
static void FindByName(const Applicant *arr, int count)
{
    char firstBuf[64], lastBuf[64];
    printf("Enter student's first and last name: ");
    if (scanf("%63s %63s", firstBuf, lastBuf) != 2) {
        printf("Invalid name.\n");
        return;
    }
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i].firstName, firstBuf) == 0 &&
            strcmp(arr[i].lastName,  lastBuf)  == 0) {
            PrintApplicant(&arr[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No such student.\n");
    }
}

/* ---------- subtask 4 ---------- */
static void ListOrderedByPoints(const Applicant *arr, int count)
{
    int *order = BuildRanking(arr, count);
    if (order == NULL) { printf("Out of memory.\n"); return; }

    printf("\n--- Applicants ordered by admission points ---\n");
    for (int i = 0; i < count; i++) {
        printf("%2d. ", i + 1);
        PrintApplicant(&arr[order[i]]);
    }
    free(order);
}

/* ---------- subtask 5 ---------- */
static void GroupByPayment(const Applicant *arr, int count)
{
    int agreed = 0, refused = 0;

    printf("\n--- Agreed to pay ---\n");
    for (int i = 0; i < count; i++) {
        if (arr[i].agreedToPay) {
            printf("  ");
            PrintApplicant(&arr[i]);
            agreed++;
        }
    }
    printf("--- Did NOT agree to pay ---\n");
    for (int i = 0; i < count; i++) {
        if (!arr[i].agreedToPay) {
            printf("  ");
            PrintApplicant(&arr[i]);
            refused++;
        }
    }
    printf("Agreed: %d    Did not agree: %d\n", agreed, refused);
}

/* ---------- shared free-seat rule ---------- */

/* Returns a new array of size count: 1 = free seat, 0 = not.
   Top FREE_SEATS by points; ties with the last one are also free.
   A manually added student (manualIndex >= 0) always gets a free
   seat without removing anyone: if they already qualified, one
   extra applicant is promoted instead. Caller must free() it. */
static int *ComputeFreeSeats(const Applicant *arr, int count, int manualIndex)
{
    int *isFree = calloc(count, sizeof(int));
    int *order  = BuildRanking(arr, count);
    if (isFree == NULL || order == NULL) {
        free(isFree);
        free(order);
        return NULL;
    }

    if (count <= FREE_SEATS) {
        for (int i = 0; i < count; i++) {
            isFree[i] = 1;
        }
    } else {
        double cutoff = AdmissionPoints(&arr[order[FREE_SEATS - 1]]);
        for (int i = 0; i < count; i++) {
            if (AdmissionPoints(&arr[i]) >= cutoff) {
                isFree[i] = 1;
            }
        }
    }

    if (manualIndex >= 0 && manualIndex < count) {
        if (isFree[manualIndex]) {
            /* Already free -> give the next-best applicant an extra seat. */
            for (int i = 0; i < count; i++) {
                if (!isFree[order[i]]) {
                    isFree[order[i]] = 1;
                    break;
                }
            }
        } else {
            isFree[manualIndex] = 1;        /* extra seat, nobody loses theirs */
        }
    }

    free(order);
    return isFree;
}

/* ---------- subtask 6 ---------- */
static int AskManualStudent(const Applicant *arr, int count)
{
    PrintAllData(arr, count);
    printf("Enter index of the student to add for free: ");
    int index;
    if (scanf("%d", &index) != 1 || index < 0 || index >= count) {
        printf("Invalid index.\n");
        return -1;
    }
    printf("Added %s %s -> guaranteed free seat.\n",
           arr[index].firstName, arr[index].lastName);
    return index;
}

/* ---------- subtask 7 ---------- */
static void PrintAcceptedList(const Applicant *arr, int count, int manualIndex)
{
    int *isFree = ComputeFreeSeats(arr, count, manualIndex);
    int *order  = BuildRanking(arr, count);
    if (isFree == NULL || order == NULL) {
        free(isFree);
        free(order);
        printf("Out of memory.\n");
        return;
    }

    printf("\n--- Accepted students ---\n");

    /* Free seats first, best score first. */
    for (int i = 0; i < count; i++) {
        int idx = order[i];
        if (!isFree[idx]) continue;
        if (idx == manualIndex) {
            printf("  [FREE - added manually] ");
        } else {
            printf("  [FREE]                  ");
        }
        PrintApplicant(&arr[idx]);
    }

    /* Then everyone without a free seat who agreed to pay. */
    for (int i = 0; i < count; i++) {
        int idx = order[i];
        if (isFree[idx] || !arr[idx].agreedToPay) continue;
        printf("  [PAID]                  ");
        PrintApplicant(&arr[idx]);
    }

    free(isFree);
    free(order);
}

/* ---------- menu ---------- */
int main(void)
{
    Applicant *applicants = NULL;
    int count = 0;

    if (!ReadApplicants(DATA_FILE, &applicants, &count)) {
        fprintf(stderr, "Could not read %s\n", DATA_FILE);
        return 1;
    }

    int manualIndex = -1;          /* student manually given a free seat */
    int choice;

    do {
        printf("\n==== Training course admission ====\n"
               "1 - Print all data\n"
               "2 - Subject with the highest average grade\n"
               "3 - Find a student by name\n"
               "4 - List students ordered by admission points\n"
               "5 - Group students by agreement to pay\n"
               "6 - Manually add a student to the accepted list\n"
               "7 - Create the list of accepted students\n"
               "0 - Exit\n"
               "Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a number.\n");
            break;
        }

        switch (choice) {
            case 1: PrintAllData(applicants, count);                   break;
            case 2: HighestAverageSubject(applicants, count);          break;
            case 3: FindByName(applicants, count);                     break;
            case 4: ListOrderedByPoints(applicants, count);            break;
            case 5: GroupByPayment(applicants, count);                 break;
            case 6: manualIndex = AskManualStudent(applicants, count); break;
            case 7: PrintAcceptedList(applicants, count, manualIndex);  break;
            case 0: printf("Goodbye.\n");                              break;
            default: printf("Unknown option.\n");                      break;
        }
    } while (choice != 0);

    FreeApplicants(applicants, count);
    return 0;
}