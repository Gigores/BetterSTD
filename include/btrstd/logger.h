#pragma once

#include "stdio.h"
#include "stdbool.h"

#define BTR_LOG_LEVELS       \
    X(DEBUG, "\e[38;5;245m") \
    X(INFO, "\e[38;5;14m")   \
    X(WARN, "\e[0;33m")      \
    X(ERROR, "\e[38;5;202m") \
    X(FATAL, "\e[38;5;196m")

typedef enum {
#define X(LEVEL, COLOR) LOG_##LEVEL,
    BTR_LOG_LEVELS
#undef X
} btr_log_level_t;

const char *BTR_LogLevel_toString(btr_log_level_t);
const char *BTR_LogLevel_getColor(btr_log_level_t);

typedef struct {
    btr_log_level_t minLogLevel;
    FILE *file;
    bool logTime;
    bool logFile;
    bool logFunc;
    bool logLine;
} btr_logger_t;

void BTR_setLogger(btr_logger_t);
btr_logger_t *BTR_getLogger(void);

void BTR_logImpl(const char *file, int line, const char *func, btr_log_level_t logLevel, const char *formatString, ...);

#define BTR_log(logLevel, formatString, ...) BTR_logImpl(__FILE__, __LINE__, __FUNCTION__, logLevel, formatString __VA_OPT__(,) __VA_ARGS__)

#ifdef BTR_NO_PREFIX

#define LOG_LEVELS BTR_LOG_LEVELS
typedef btr_log_level_t log_level_t;

#define setLogger BTR_setLogger
#define getLogger BTR_getLogger

#define LogLevel_toString BTR_LogLevel_toString
#define LogLevel_getColor BTR_LogLevel_getColor

typedef btr_logger_t logger_t;
#define log BTR_log

#endif  // BTR_NO_PREFIX
