#pragma once

#include "stdio.h"

#define BTR_LOG_LEVELS        \
    X(LOG_DEBUG, "\e[38;5;245m")   \
    X(LOG_INFO, "\e[38;5;14m")    \
    X(LOG_WARN, "\e[0;33m")    \
    X(LOG_ERROR, "\e[38;5;202m") \
    X(LOG_FATAL, "\e[38;5;196m")

typedef enum : char {
#define X(LEVEL, COLOR) LEVEL,
    BTR_LOG_LEVELS
#undef X
} btr_log_level_t;

const char *BTR_LogLevel_toString(btr_log_level_t);
const char *BTR_LogLevel_getColor(btr_log_level_t);

typedef struct {
    btr_log_level_t minLogLevel;
    FILE *file;
} btr_logger_t;

void BTR_Logger_log(btr_logger_t *, btr_log_level_t logLevel, const char *logPrefix, const char *formatString, ...);

#ifdef BTR_NO_PREFIX

#define LOG_LEVELS BTR_LOG_LEVELS
typedef btr_log_level_t log_level_t;

#define LogLevel_toString BTR_LogLevel_toString
#define LogLevel_getColor BTR_LogLevel_getColor

typedef btr_logger_t logger_t;
#define Logger_log BTR_Logger_log

#endif  // BTR_NO_PREFIX
