#include <stdio.h>
#include <time.h>
#include "Logger.h"

#include <string.h>

#include "FileHandaling.h"

static LogLevel currentLevel = LOG_INFO;
static char logFileName[256];

static const char *levelToString(LogLevel level) {
    switch (level)
    {
        case LOG_ERROR: return "ERROR";
        case LOG_WARNING: return "WARNING";
        case LOG_INFO: return "INFO";
        default: return "OFF";
    }
}


void Logger(LogLevel level, const char *message) {
    if (currentLevel == LOG_OFF) {
        return;
    }
    if (level > currentLevel) {
        return;
    }

    FILE *log = fopen(logFileName, "a");
    if (!log) {
        fprintf(stderr, "Problem opening log file\n");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(log,
        "%02d-%02d-%04d %02d:%02d:%02d [%s] %s\n",
        t->tm_mday,
        t->tm_mon + 1,
        t->tm_year + 1900,
        t->tm_hour,
        t->tm_min,
        t->tm_sec,
        levelToString(level),
        message);
    fflush(log);
    fclose(log);

}

void LoggerSetLoggingLevel(LogLevel level)
{
    currentLevel = level;
}


void LoggerSetOutputName(const char *filename)
{
    strncpy(logFileName, filename, sizeof(logFileName) - 1);
    logFileName[sizeof(logFileName) - 1] = '\0';
}