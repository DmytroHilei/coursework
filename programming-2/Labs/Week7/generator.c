/**
 * File:        generator.c
 * Author:      Risto Heinsar
 * Created:     12.03.2015
 * Modified     14.03.2023
 *
 * Description: This is a random data generator
 * 
 * Note:        This is a starter code for a lab task
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "name_pools.h"
#include "generator.h"

#include <time.h>

int main(void)
{
    srand(time(NULL));

    Settings s;
    InitDefaultSettings(&s);

    /* Names are assigned from the pools in the header*/
    const char *firstNames[] = FIRST_NAME_POOL;
    const char *lastNames[] = LAST_NAME_POOL;
    const char *curriculumCodes[] = CURRICULUM_CODES;

    /* Find the counts for the pool sizes */
    int firstNamePoolSize = sizeof(firstNames) / sizeof(char *);
    int lastNamePoolSize = sizeof(lastNames) / sizeof(char *);
    int curriculumCodePoolSize = sizeof(curriculumCodes) / sizeof(char *);

    printf("The database contains:\n"
           "\t%4d first names\n"
           "\t%4d last names\n"
           "\t%4d curriculum codes\n",
           firstNamePoolSize, lastNamePoolSize, curriculumCodePoolSize);

    // int choice = chooseOutputFormat();

    s.num_entries = askForNumberOfCandidates();

    PrintSettings(&s);
    EditSettings(&s);



    struct candidate *candidates = malloc(sizeof(struct candidate) * s.num_entries);
    if (candidates == NULL) {
        printf("Not enough memory.\n");
        free(candidates);
        return EXIT_FAILURE;
    }

    int val;
    for (int i = 0; i < s.num_entries; i++) {
        if (s.gen_first_name) {
            candidates[i].first_name = firstNames[GetRand(0, firstNamePoolSize - 1)];
        }
        else {
            candidates[i].first_name = "";
        }
        if (s.gen_last_name) {
            candidates[i].last_name  = lastNames[GetRand(0, lastNamePoolSize - 1)];
        }
        else {
            candidates[i].last_name = "";
        }
        if (s.gen_curriculum) {
            candidates[i].curriculum = curriculumCodes[GetRand(0, curriculumCodePoolSize - 1)];
        }
        else {
            candidates[i].curriculum = "";
        }
        if (s.gen_points) {
            val = GetRand(100, 300);
            candidates[i].points = (float)val / 10.0;
        }
        else {
            candidates[i].points = 0.0;
        }

        candidates[i].index = i;
    }

    qsort(candidates, s.num_entries, sizeof(struct candidate), ComparCandidate);


    char full_filename[150];
    snprintf(full_filename, sizeof(full_filename), "%s.%s",
        s.filename,
        s.format == FORMAT_CSV ? "csv" : "txt");

    WriteToFile(full_filename, candidates, s.num_entries, s.format);



    free(candidates);



    return EXIT_SUCCESS;
}

/**
 * Description:    Generates a random number in range provided, ends inclusive
 *
 * Parameters:     min, minimum value
 *                 max, maximum vale
 *
 * Return:         integer, generated random number
 */
int GetRand(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}


int ComparCandidate(const void *a, const void *b)
{
    // Temporary cast pointers for readability
    struct candidate *pA = (struct candidate *)a;
    struct candidate *pB = (struct candidate *)b;

    // Get comparison for last names
    int ret = strcmp(pA->last_name, pB->last_name);

    // When last names match, return difference by first name
    if (ret == 0)
        return strcmp(pA->first_name, pB->first_name);

    // return difference by last name
    return ret;
}

/* int chooseOutputFormat() {
    int outputIndex;
    int val;

    do {
        printf("Enter 0 to use .txt format and 1 to use CSV format:");
        val = scanf("%d", &outputIndex);
        if (val != 1 || outputIndex != 1 || outputIndex != 0) {
            printf("Wrong input. Please try again.\n");
        }
    } while (val != 1 || outputIndex != 1 || outputIndex != 0);

    return outputIndex;
} */

int askForNumberOfCandidates() {
    int numCandidates;
    int val;
    do {
        printf("Enter the number of the candidates: ");
        val = scanf("%d", &numCandidates);
        if (val != 1) {
            printf("Wrong input. Please try again.\n");
        }
    } while (val != 1);

    return numCandidates;
}


void WriteToFile(const char *filename, const struct candidate *arr, int n, OutputFormat format)
{
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Failed to open file");
        return;
    }

    if (format == FORMAT_CSV) {
        // header
        fprintf(f, "index,last_name,first_name,curriculum,points\n");
    }

    for (int i = 0; i < n; i++) {
        if (format == FORMAT_CSV) {
            fprintf(f, "%d,%s,%s,%s,%.1f\n",
                arr[i].index,
                arr[i].last_name,
                arr[i].first_name,
                arr[i].curriculum,
                arr[i].points
            );
        } else {
            fprintf(f, "%d %s %s %s %.1f\n",
                arr[i].index,
                arr[i].last_name,
                arr[i].first_name,
                arr[i].curriculum,
                arr[i].points
            );
        }
    }

    fclose(f);
}

void InitDefaultSettings(Settings *s)
{
    s->gen_first_name = 1;
    s->gen_last_name = 1;
    s->gen_curriculum = 1;
    s->gen_points = 1;

    s->num_entries = 10;

    s->min_points = 100; // 10.0
    s->max_points = 300; // 30.0

    snprintf(s->filename, sizeof(s->filename), "output");

    s->format = FORMAT_TXT;
}




void PrintSettings(const Settings *s)
{
    printf("=== SETTINGS ===\n");
    printf("First name:   %s\n", s->gen_first_name ? "Yes" : "No");
    printf("Last name:    %s\n", s->gen_last_name ? "Yes" : "No");
    printf("Curriculum:   %s\n", s->gen_curriculum ? "Yes" : "No");
    printf("Points:       %s\n", s->gen_points ? "Yes" : "No");

    printf("Entries:      %d\n", s->num_entries);
    printf("Points range: %.1f - %.1f\n", s->min_points / 10.0, s->max_points / 10.0);

    printf("Format:       %s\n", s->format == FORMAT_CSV ? "CSV" : "TXT");
    printf("Filename:     %s\n", s->filename);
}

void EditSettings(Settings *s)
{
    int choice;

    printf("Change settings? (1 = yes, 0 = no): ");
    scanf("%d", &choice);

    if (!choice) return;

    printf("Generate first name (1/0): ");
    scanf("%d", &s->gen_first_name);

    printf("Generate last name (1/0): ");
    scanf("%d", &s->gen_last_name);

    printf("Generate curriculum (1/0): ");
    scanf("%d", &s->gen_curriculum);

    printf("Generate points (1/0): ");
    scanf("%d", &s->gen_points);

    printf("Min points (e.g. 100 for 10.0): ");
    scanf("%d", &s->min_points);

    printf("Max points (e.g. 300 for 30.0): ");
    scanf("%d", &s->max_points);

    printf("Format (1 = TXT, 2 = CSV): ");
    int f;
    scanf("%d", &f);
    s->format = (f == 2) ? FORMAT_CSV : FORMAT_TXT;

    printf("Filename (without extension): ");
    scanf("%s", s->filename);
}


