//
// Created by dmytro-hilei on 3/16/26.
//

#ifndef WEEK7_FILES_H
#define WEEK7_FILES_H

#define MAX_GRADES 20
#define MAX_SUBJECTS 50

#define FILEPATHTXT "data/data_grades.txt"
#define FILEPATHCSV "data/data_grades.csv"

struct Subject {
    char name[50];
    int grades[20];
    int grade_count;
};

int readSubjects(FILE *fp, struct Subject subjects[]);
int readCSV(FILE *fp, struct Subject subjects[]);
#endif //WEEK7_FILES_H