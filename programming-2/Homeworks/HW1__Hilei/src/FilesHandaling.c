
#include <stdio.h>

#include "Headers_Functions.h"
#include "HeadersEnums.h"


/*
 * Function:    readPerson
 * Description: Reads a single resident record from the input file
 *              and stores it in the provided resident struct.
 *              Uses format macros (NAME_SCAN, SUR_SCAN, ID_SCAN) to
 *              read fields delimited by ';' character.
 *
 * Input:       file      - pointer to an opened input FILE
 *              residents - pointer to a resident struct to store the result
 *
 * Output:      Returns SUCCESS_READING_FILE on success,
 *              END_OF_FILE if end of file is reached,
 *              WRONG_DATA_FORMAT if the record format is invalid.
 */


int readPerson(FILE *file, resident *residents)
{
    int result = fscanf(file, NAME_SCAN SUR_SCAN ID_SCAN, residents -> firstName,
        residents->lastName,
        residents->id); //Important part. By writing [^;]; I basically tell to read until ; and ignore that
    if (result == EOF)
    {
        return END_OF_FILE;
    }
    if (result != GOOD_FPRINTF_OUTPUT)
    {
        fprintf(stderr, "Error, invalid data format in file\n");
        return WRONG_DATA_FORMAT;
    }
    return SUCCESS_READING_FILE;

}




