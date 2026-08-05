#include <stdio.h>
#include "FileHandaling.h"
#include "Logger.h"

float calculateAverage(struct Subject *subject)
{
    int sum = 0;
    if (subject->grade_count == 0) {
        Logger(LOG_ERROR, "Error calculating average grade");
        return 0.0f;
    }
    for (int i = 0; i < subject->grade_count; i++)
    {
        sum += subject->grades[i];
    }

    return (float)sum / (float)subject->grade_count;
}
void printSubjects(struct Subject subjects[], int count) {
    for (int i = 0; i < count; i++) {
        printf("Subject: %s\n", subjects[i].name);
        if (subjects[i].grade_count == 0) {
            printf("Grades: N/A\nAverage: N/A\n\n");
            continue;
        }
        printf("Grades: ");
        for (int j = 0; j < subjects[i].grade_count; j++) {
            printf("%d, ", subjects[i].grades[j]);
        }

        float average = calculateAverage(&subjects[i]);

        printf("\nAverage: %.2f\n\n",
               average);
    }
}