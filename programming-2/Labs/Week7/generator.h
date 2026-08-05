/**
 * File:        generator.h
 * Author:      Risto Heinsar
 * Created:     12.03.2015
 * Modified     12.03.2023
 *
 * Description: Header file to provide the name pools required for the task.
 */
#ifndef GENERATOR_H
#define GENERATOR_H

typedef enum {
    FORMAT_TXT,
    FORMAT_CSV
} OutputFormat;

typedef struct {
    int gen_first_name;
    int gen_last_name;
    int gen_curriculum;
    int gen_points;

    int num_entries;

    int min_points; // у вигляді int: 100 = 10.0
    int max_points; // 300 = 30.0

    char filename[100];

    OutputFormat format;
} Settings;

struct candidate {
    const char* first_name;
    const char* last_name;
    const char* curriculum;
    float points;
    int index;
};

int GetRand(int numMin, int numMax);
int ComparCandidate(const void *a, const void *b);
int askForNumberOfCandidates();
void WriteToFile(const char *filename, const struct candidate *arr, int n, OutputFormat format);
int chooseOutputFormat();
void InitDefaultSettings(Settings *s);
void PrintSettings(const Settings *s);
void EditSettings(Settings *s);
#endif //GENERATOR_H
