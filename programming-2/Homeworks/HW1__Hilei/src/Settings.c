#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "settings.h"

ProgramSettings settings;


/*
 * Function:    loadSettings
 * Description: Loads program settings from a configuration file.
 *              Reads key=value pairs and assigns them to the global
 *              settings struct. If the file is not found, default
 *              values are used instead.
 * Input:       filename - path to the configuration file (e.g. "config.conf")
 * Output:      None
 */
void loadSettings(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Warning: config file not found, using defaults\n");

        strcpy(settings.inputFile, "Input.txt");
        strcpy(settings.outputFile, "output.txt");
        settings.printToTerminal = 1;
        settings.writeToFile = 1;

    }

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");

        if (!key || !value)
            continue;

        if (strcmp(key, "input_file") == 0)
            strncpy(settings.inputFile, value, MAX_PATH_LEN);

        else if (strcmp(key, "output_file") == 0)
            strncpy(settings.outputFile, value, MAX_PATH_LEN);

        else if (strcmp(key, "print_to_terminal") == 0)
            settings.printToTerminal = atoi(value);

        else if (strcmp(key, "write_to_file") == 0)
            settings.writeToFile = atoi(value);
    }

    fclose(file);
}

/*
 * Function:    parseArguments
 * Description: Parses command line arguments and overrides the
 *              corresponding settings loaded from the config file.
 *              Unknown arguments are reported to stderr.
 *
 * Supported arguments:
 *              -i <filename>    - sets the input file path
 *              -o <filename>    - sets the output file path
 *              --no-terminal    - disables printing to terminal
 *              --no-file        - disables writing to output file
 *
 * Input:       argc - number of command line arguments
 *              argv - array of command line argument strings
 * Output:      None
 */


void parseArguments(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
        {
            strncpy(settings.inputFile, argv[i + 1], MAX_PATH_LEN);
            printf("Input file set to: %s\n", settings.inputFile);
            i++;
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            strncpy(settings.outputFile, argv[i + 1], MAX_PATH_LEN);
            printf("Output file set to: %s\n", settings.outputFile);
            i++;
        }
        else if (strcmp(argv[i], "--no-terminal") == 0)
        {
            settings.printToTerminal = 0;
            printf("Terminal output disabled.\n");
        }
        else if (strcmp(argv[i], "--no-file") == 0)
        {
            settings.writeToFile = 0;
            printf("File output disabled.\n");
        }
        else
        {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            fprintf(stderr, "Usage:\n");
            fprintf(stderr, "  -i <filename>   Set input file\n");
            fprintf(stderr, "  -o <filename>   Set output file\n");
            fprintf(stderr, "  --no-terminal   Disable terminal output\n");
            fprintf(stderr, "  --no-file       Disable file output\n");
        }
    }
}
void printHelp() {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  -i <filename>   Set input file\n");
    fprintf(stderr, "  -o <filename>   Set output file\n");
    fprintf(stderr, "  --no-terminal   Disable terminal output\n");
    fprintf(stderr, "  --no-file       Disable file output\n");
}
