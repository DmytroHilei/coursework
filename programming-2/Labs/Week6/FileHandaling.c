#include <stdio.h>
#include "FileHandaling.h"

#include "Logger.h"

int readSubjects(FILE *fp, struct Subject subjects[])
{
    int subject_count = 0;

    while (fscanf(fp, "%49s %d",
                  subjects[subject_count].name,
                  &subjects[subject_count].grade_count) == 2)
    {
        int count = subjects[subject_count].grade_count;

        for (int i = 0; i < count; i++)
        {
            if (fscanf(fp, "%d", &subjects[subject_count].grades[i]) != 1)
            {
                Logger(LOG_ERROR, "Error reading file");
                printf("Error reading grade\n");
                return subject_count;
            }
        }

        subject_count++;
        if (subject_count > MAX_SUBJECTS) {
            Logger(LOG_ERROR, "Reading aborted: subject array limit reached");
            break;
        }
    }

    char msg[100];
    snprintf(msg, sizeof(msg),
             "Reading completed. Lines read: %d", subject_count);

    Logger(LOG_INFO,msg);

    return subject_count;
}

int readCSV(FILE *fp, struct Subject subjects[]) {
    int subject_count = 0;

    while (fscanf(fp, "%49[^,],%d",
                  subjects[subject_count].name,
                  &subjects[subject_count].grade_count) == 2)
    {
        if (subject_count > MAX_SUBJECTS) {
            Logger(LOG_ERROR, "Reading aborted: subject array limit reached");
            break;
        }

        int count = subjects[subject_count].grade_count;

        for (int i = 0; i < count; i++)
        {
            if (fscanf(fp, ",%d", &subjects[subject_count].grades[i]) != 1)
            {
                Logger(LOG_ERROR, "Error reading file");
                printf("Error reading grade\n");
                return subject_count;
            }
        }
        fscanf(fp, "\n");

        subject_count++;
    }

    char msg[100];
    snprintf(msg, sizeof(msg),
             "Reading completed. Lines read: %d", subject_count);

    Logger(LOG_INFO,msg);

    return subject_count;
}

