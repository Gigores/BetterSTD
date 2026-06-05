#include "btrstd/logger.h"
#include "stdarg.h"
#include "stdio.h"

#define BTR_TERM_COLOR_RESET "\e[0m"
#define BTR_TERM_COLOR_INTER "\e[38;5;246m"

const char *BTR_LogLevel_toString(btr_log_level_t level)
{
    switch (level) {
#define X(LEVEL, COLOR) case (LEVEL): return #LEVEL;
        BTR_LOG_LEVELS
#undef X
        default: return "INVALID_LOG_LEVEL";
    }
}
const char *BTR_LogLevel_getColor(btr_log_level_t level)
{
    switch (level) {
#define X(LEVEL, COLOR) case (LEVEL): return COLOR;
        BTR_LOG_LEVELS
#undef X
        default: return BTR_TERM_COLOR_RESET;
    }
}
void BTR_Logger_log(btr_logger_t *logger, btr_log_level_t logLevel, const char *formatString, ...)
{
    if (logLevel < logger->minLogLevel) return;

    const char *logLevelString = BTR_LogLevel_toString(logLevel);
    const char *logLevelColor = BTR_LogLevel_getColor(logLevel);
    printf(BTR_TERM_COLOR_INTER"[%s%s"BTR_TERM_COLOR_INTER"] "BTR_TERM_COLOR_RESET, logLevelColor, logLevelString);
    if (logger->file) fprintf(logger->file, "[%s] ", logLevelString);
    
    va_list args;
    va_start(args, formatString);
    vprintf(formatString, args);
    if (logger->file) vfprintf(logger->file, formatString, args);
    va_end(args);

    printf("\n");
    if (logger->file) fprintf(logger->file, "\n");
}
