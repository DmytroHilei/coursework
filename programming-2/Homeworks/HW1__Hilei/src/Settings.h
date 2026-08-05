#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_PATH_LEN 256

typedef struct
{
    char inputFile[MAX_PATH_LEN];
    char outputFile[MAX_PATH_LEN];

    int printToTerminal;   /* 1 = enabled, 0 = disabled */
    int writeToFile;       /* 1 = enabled, 0 = disabled */

} ProgramSettings;

extern ProgramSettings settings;

void loadSettings(const char *filename);
void parseArguments(int argc, char *argv[]);
void printHelp();
#endif

