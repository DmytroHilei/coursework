/**
 * File:        file_helper.c
 * Author:      Risto Heinsar
 * Created:     27.02.2026
 * Modified:    27.02.2026
 *
 * Description: Impementations for functions to download files off the internet
 *              using libcurl. 
 *              Includes helper functions for CSV to space delimiting and 
 *              leech protection based on file modified time.
 * 
 *              -lcurl required to compile.
 * 
 *              NB! DO NOT MODIFY FOR THE LAB TASK!
 */
 
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <curl/curl.h>
#include "file_helper.h"

/**
 * Downloads the covid opendata. If file exists, prompts before 
 * downloading it again.
 */
bool DownloadFileToDisk(const char *url, 
                        const char *outFileName, 
                        enum Type destFileType, 
                        bool skipOverwritePrompt)
{
    /* Grab output file information and check if it exists */
    struct stat fileInfo;
    if (lstat(outFileName, &fileInfo) != -1)
    {
        /* Grab current system time */
        time_t currentTime = time(NULL);
        
        /* Avoid spamming the download links too often */
        if (currentTime - fileInfo.st_mtime <= REDOWNLOAD_TIME_THRESHOLD)
            return false;
    }
    
    /* For manual use, skips re-download if deemed unnecessary by user */
    if (!skipOverwritePrompt && CheckIfFileExists(outFileName))
    {
        printf("Warning! The file ""%s"" already exists!\n", outFileName);
        printf("Are you sure you wish to download again?\nEnter [Y] or [N]\n");
        
        if (PromptYesNo() == false)
            return false;
    }
    
    DownloadFile(url, outFileName);
    
    if (destFileType == CSV_TO_SPACE_DELIMITED)
        ReplaceCharInFile(outFileName, ',', ' ');
        
    return true;
}

/**
 * Prompts for user confirmation until received
 */
bool PromptYesNo(void)
{
    while (true)
    {
        char c = (char)getchar();
        if (c == '\n')
            continue;
        if (c == 'y' || c == 'Y')
            return true;
        else if (c == 'n' || c == 'N')
            return false;
        else
            printf("Please enter y or n!\n");
    }
}

/**
 * Checks if file exists
 */
bool CheckIfFileExists(const char *fileName)
{
    return (!access(fileName, F_OK));
}

/**
 * file downloader curl sample
 * https://curl.se/libcurl/c/url2file.html
 */
size_t WriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    return fwrite(ptr, size, nmemb, (FILE *)stream);;
}

/**
 * file downloader curl sample
 * https://curl.se/libcurl/c/url2file.html
 */
void DownloadFile(const char *url, const char *fileName)
{
    curl_global_init(CURL_GLOBAL_ALL);
 
    /* init the curl session */
    CURL *curl_handle = curl_easy_init();
 
    /* set URL to get here */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
 
    /* Switch on full protocol/debug output while testing */
    //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
    /* disable progress meter, set to 0L to enable it */
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
    /* send all data to this function   */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteData);
 
    /* open the file */
    FILE *fOutput = fopen(fileName, "wb");
    if (fOutput) {
 
        /* write the page body to this file handle */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fOutput);
 
        /* get it! */
        curl_easy_perform(curl_handle);
 
        /* close the header file */
        fclose(fOutput);
    }
 
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
 
    curl_global_cleanup();
}

/**
 * Replaces all characters matching to key to replacement in the given file.
 */
void ReplaceCharInFile(const char *fName, char key, char replacement)
{
    FILE *fp = fopen(fName, "r+");
    
    if (fp != NULL)
    {
        int ch;
        while ((ch = fgetc(fp)) != EOF)
        {
            if (ch == key)
            {
                fseek(fp, ftell(fp) - 1, SEEK_SET);
                fputc(replacement, fp);
            }
        }
        fclose(fp);
    }
}

