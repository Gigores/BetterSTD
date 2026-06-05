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

void BTR_Logger_log(btr_logger_t *, btr_log_level_t logLevel, const char *formatString, ...);

