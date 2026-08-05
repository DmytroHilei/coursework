#include <stdio.h>

#include "HeadersEnums.h"
#include "Headers_Functions.h"
#include "Settings.h"

/*
 * Function:    main
 * Description: Entry point of the program. Loads settings from the
 *              config file, parses command line arguments, reads
 *              resident data from the input file, prompts the user
 *              for the current date, and prints upcoming birthdays
 *              to the terminal and/or output file.
 *
 * Input:       argc - number of command line arguments
 *              argv - array of command line argument strings
 *
 * Output:      Returns SUCCESS on successful execution,
 *              NO_FILE if the input or output file could not be opened.
 */



int main(int argc, char* argv[])
{
    printHelp();
    loadSettings("config.conf");
    parseArguments(argc, argv);

    FILE* file = fopen(settings.inputFile, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", settings.inputFile);
        return NO_FILE;
    }

    resident residents[MAX_RESIDENTS];
    Date birthdays[MAX_RESIDENTS];

    int count = 0;


    while (count < MAX_RESIDENTS && readPerson(file, residents + count) == SUCCESS_READING_FILE)
    {
        if (convertIDToBirth(residents[count].id, birthdays + count) != SUCCESS_CONVERTING_BIRTH_DAY)
        {
            fprintf(stderr, "Error converting ID to Birth day\n");
            continue;
        }
        count++;

    }
    fclose(file);

    Date currentDate;
    int isValid = 0;

    while (!isValid)
    {
        printf("Enter date (dd.mm.yyyy): ");

        if (scanf("%d.%d.%d",
                  &currentDate.day,
                  &currentDate.month,
                  &currentDate.year) != GOOD_OUTPUT_OF_SCANF)
        {
            fprintf(stderr, "Invalid date format. Expected dd.mm.yyyy\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            continue;
        }

        int validDay = isValidDay(currentDate);

        if (currentDate.month < MIN_MONTH ||
            currentDate.month > MAX_MONTH ||
            validDay != GOOD_DAY)
        {
            fprintf(stderr, "Error: invalid date values. Try again.\n");
            continue;
        }

        isValid = 1;
    }
    FILE *output = NULL;
    if (settings.writeToFile) {
        output = fopen(settings.outputFile, "w");
        if (output == NULL) {
            fprintf(stderr, "Error opening file %s\n", settings.outputFile);
            return NO_FILE;
        }
    }

    printGuysWithSoonBirthday(output, birthdays, residents, count, &currentDate);

    if (output)
        fclose(output);

    return SUCCESS;
}
