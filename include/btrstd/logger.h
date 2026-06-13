#pragma once

#include "stdio.h"
#include "stdbool.h"
#include "stdarg.h"

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
void BTR_vlogImpl(const char *file, int line, const char *func, btr_log_level_t logLevel, const char *formatString, va_list args);

#define BTR_log(logLevel, formatString, ...) BTR_logImpl(__FILE__, __LINE__, __FUNCTION__, logLevel, formatString __VA_OPT__(,) __VA_ARGS__)

// int
#define BTR_debugInt(variable) BTR_log(LOG_DEBUG, #variable " = %d", variable)
// unsigned int
#define BTR_debugUInt(variable) BTR_log(LOG_DEBUG, #variable " = %u", variable)
// long
#define BTR_debugLong(variable) BTR_log(LOG_DEBUG, #variable " = %ld", variable)
// unsigned long
#define BTR_debugULong(variable) BTR_log(LOG_DEBUG, #variable " = %lu", variable)
// long long
#define BTR_debugLongLong(variable) BTR_log(LOG_DEBUG, #variable " = %lld", variable)
// unsigned long long
#define BTR_debugULongLong(variable) BTR_log(LOG_DEBUG, #variable " = %llu", variable)
// char
#define BTR_debugChar(variable) BTR_log(LOG_DEBUG, #variable " = %c", variable)
// char * | const char *
#define BTR_debugString(variable) BTR_log(LOG_DEBUG, #variable " = %s", variable)
// void * | ...
#define BTR_debugPointer(variable) BTR_log(LOG_DEBUG, #variable " = %p", variable)
// float | double
#define BTR_debugFloat(variable) BTR_log(LOG_DEBUG, #variable " = %f", variable)
// size_t
#define BTR_debugSize(variable) BTR_log(LOG_DEBUG, #variable " = %zu", variable)
// bool
#define BTR_debugBool(variable) BTR_log(LOG_DEBUG, #variable " = %s", variable ? "true" : "false")

#ifdef BTR_NO_PREFIX

#define LOG_LEVELS BTR_LOG_LEVELS
typedef btr_log_level_t log_level_t;

#define setLogger BTR_setLogger
#define getLogger BTR_getLogger

#define LogLevel_toString BTR_LogLevel_toString
#define LogLevel_getColor BTR_LogLevel_getColor

typedef btr_logger_t logger_t;
#define log BTR_log
#define vlogImpl BTR_vlogImpl

#define debugInt BTR_debugInt
#define debugUInt BTR_debugUInt
#define debugLong BTR_debugLong
#define debugULong BTR_debugULong
#define debugLongLong BTR_debugLongLong
#define debugULongLong BTR_debugULongLong
#define debugChar BTR_debugChar
#define debugString BTR_debugString
#define debugPointer BTR_debugPointer
#define debugFloat BTR_debugFloat
#define debugSize BTR_debugSize
#define debugBool BTR_debugBool

#endif  // BTR_NO_PREFIX
