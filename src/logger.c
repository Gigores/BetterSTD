#include "btrstd/logger.h"
#include "stdarg.h"
#include "stdio.h"
#include "time.h"

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
void BTR_Logger_log(btr_logger_t *logger, btr_log_level_t logLevel, const char *logPrefix, const char *formatString, ...)
{
    if (logLevel < logger->minLogLevel) return;

    const char *logLevelString = BTR_LogLevel_toString(logLevel);
    const char *logLevelColor = BTR_LogLevel_getColor(logLevel);
    time_t currentTime = time(NULL);
    struct tm *t = localtime(&currentTime);
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", t);
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    long millis = ts.tv_nsec / 1000000;
    printf(BTR_TERM_COLOR_INTER"["BTR_TERM_COLOR_RESET"%s.%03ld"BTR_TERM_COLOR_INTER"] [%s%s"BTR_TERM_COLOR_INTER"] ["BTR_TERM_COLOR_RESET"%s"BTR_TERM_COLOR_INTER"]: "BTR_TERM_COLOR_RESET, buffer, millis, logLevelColor, logLevelString, logPrefix);
    if (logger->file) fprintf(logger->file, "[%s.%03ld] [%s] [%s]: ", buffer, millis, logLevelString, logPrefix);
    
    va_list args;
    va_start(args, formatString);
    vprintf(formatString, args);
    if (logger->file) vfprintf(logger->file, formatString, args);
    va_end(args);

    printf("\n");
    if (logger->file) fprintf(logger->file, "\n");
}
