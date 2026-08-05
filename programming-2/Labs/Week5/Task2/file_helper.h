/**
 * File:        file_helper.h
 * Author:      Risto Heinsar
 * Created:     27.02.2026
 * Modified:    27.02.2026
 *
 * Description: Declarations for the helper functions used to download files.
 * 
 *              NB! DO NOT MODIFY FOR THE LAB TASK!
 */
 
#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <stdbool.h>

/* Specify time in seconds when a file can be downloaded again based on 
   when the file was last modified */
#define REDOWNLOAD_TIME_THRESHOLD 15

/* enumeration to choose delimiter type for the file that will be written */
enum Type {UNMODIFIED, CSV_TO_SPACE_DELIMITED};

/* Description: Downloads a file off the internet in the given url. 
 *              Includes spaming protection specified by 
 *              REDOWNLOAD_TIME_THRESHOLD. 
 * 
 *              NOTE: Using SPACE_DELIMITED only works for the most basic
 *              CSV files. DO NOT use it for anything complex, or will break!
 * 
 * Parameter:   url - address to download the file from
 *              outFileName - name on disk to write the contents
 *              destFileType - tore as is or try to convert CSV to space delim
 *              skipOverwritePrompt - false - asks user to overwrite the file. 
 *                                    true - automatically overwrites the file
 *                                    Disabling does not remove spam  protection
 * 
 * Example use cases:
 *              DownloadFileToDisk("https://www.example.org/", "index.html", UNMODIFIED, false);
 *              DownloadFileToDisk("https://transport.tallinn.ee/gps.txt", "gps_data.txt", CSV_TO_SPACE_DELIMITED, false);
 */
bool DownloadFileToDisk(const char *url, 
                        const char *outFileName, 
                        enum Type destFileType, 
                        bool skipOverwritePrompt);


/*********************************************
 *  INTERNAL FUNCTIONS PAST THIS POINT
 *  THEY DON'T NEED TO BE CALLED!
 ********************************************/
  
/**
 * Description: Asks for user input of y or n. Case insensitive. Returns only
 *              after suitable input is received. 
 *              Note: doesn't clean the buffer if there are additional chars.
 * 
 * Return:      true if input was [yY] or false if [nN]. 
 */
bool PromptYesNo(void);

/**
 * Description: Checks if the file is accessible on the drive for the user.
 * 
 * Parameter:   fileName - string with the file path to test
 * 
 * Return:      true if it's accessible, false if not
 */
bool CheckIfFileExists(const char *fileName);

/**
 * Description: Downloads the file and stores it on disk.
 *              Based on libcurl sample: https://curl.se/libcurl/c/url2file.html
 * 
 * Parameter:   url - string with the download URL
 *              fname - name of the output file
 */
void DownloadFile(const char *url, const char *fileName);

/**
 * https://curl.se/libcurl/c/url2file.html
 */
size_t WriteData(void *ptr, size_t size, size_t nmemb, void *stream);

/**
 * Description: Replaces one character with another throughout the given file.
 * 
 * Parameter:   fName  - string with the name of the file
 *              key - character that will be replaced
 *              replacement - character that will be written instead
 */
void ReplaceCharInFile(const char *fName, char key, char replacement);

#endif
