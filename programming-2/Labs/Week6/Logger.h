//
// Created by dmytro-hilei on 3/16/26.
//

#ifndef WEEK7_LOGGE_H
#define WEEK7_LOGGE_H
typedef enum {
    LOG_OFF,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
} LogLevel;

void Logger(LogLevel level, const char *message);

void LoggerSetOutputName(const char *filename);
void LoggerSetLoggingLevel(LogLevel level);
#endif //WEEK7_LOGGE_H