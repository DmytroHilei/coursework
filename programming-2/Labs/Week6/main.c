#include <stdbool.h>
#include <stdio.h>

#include "FileHandaling.h"
#include "Logger.h"
#include "printing.h"

int main(int argc, char *argv[]) {
    bool isTXT = false;
    bool isCSV = false;

    if (argc < 2) {
        printf("No argument given, using TXT mode\n");
        isTXT = true;
    }
    else {
        if (argv[1][0] == '-' && argv[1][1] == 't')
            isTXT = true;

        else if (argv[1][0] == '-' && argv[1][1] == 'c')
            isCSV = true;
    }
    struct Subject subjects[MAX_SUBJECTS];

    LoggerSetOutputName("data/program.log");
    LoggerSetLoggingLevel(LOG_INFO);

    Logger(LOG_INFO, "Program started");
    int count = 0;
    if (isTXT) {
        FILE *fp = fopen(FILEPATHTXT, "r");
        Logger(LOG_INFO, "Opening TXT file");

        if (!fp) {
            Logger(LOG_ERROR, "Error opening TXT file");
            return 1;
        }

        count = readSubjects(fp, subjects);
        Logger(LOG_INFO, "Closing TXT input file");
        fclose(fp);
    }
    if (isCSV) {
        FILE *fp2 = fopen(FILEPATHCSV, "r");
        Logger(LOG_INFO, "Opening CSV file");
        if (!fp2) {
            Logger(LOG_ERROR, "Error opening CSV file");
            return 1;
        }
        count = readCSV(fp2, subjects);
        Logger(LOG_INFO, "Closing CSV input file");
        fclose(fp2);
    }



    printSubjects(subjects, count);


    return 0;
}
