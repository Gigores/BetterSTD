#include "btrstd/logger.h"
#include "btrstd/error.h"
#include "btrstd/datetime.h"
#include "stdarg.h"
#include "stdio.h"


#define COL_RESET "\e[0m"
#define COL_INTER "\e[38;5;246m"

static btr_logger_s BTR_g_logger = {
    .minLogLevel = BTR_LOG_DEBUG,
    .file = NULL,
    .logFile = true,
    .logTime = true,
    .logFunc = true,
    .logLine = true,
};

void BTR_setLogger(btr_logger_s logger)
{
    BTR_g_logger = logger;
}
btr_logger_s *BTR_getLogger(void)
{
    return &BTR_g_logger;
}

const char *BTR_LogLevel_toString(btr_log_level_e level)
{
    switch (level) {
#define X(LEVEL, COLOR) case (BTR_LOG_##LEVEL): return #LEVEL;
        BTR_LOG_LEVELS
#undef X
        default: BTR_panic("invalid log level %d", level);
    }
}
const char *BTR_LogLevel_getColor(btr_log_level_e level)
{
    switch (level) {
#define X(LEVEL, COLOR) case (BTR_LOG_##LEVEL): return COLOR;
        BTR_LOG_LEVELS
#undef X
        default: BTR_panic("invalid log level %d", level);
    }
}
static void doLogTime(btr_logger_s *logger)
{
    btr_datetime_s date = BTR_DateTime_localNow();
    printf(COL_INTER"["COL_RESET"%02d:%02d:%02d.%03d"COL_INTER"] ", date.hour, date.minute, date.second, date.nanosecond / 1000000);
    if (logger->file) fprintf(logger->file, "[%02d:%02d:%02d.%03d] ", date.hour, date.minute, date.second, date.nanosecond / 1000000);
}
static void doLogLevel(btr_logger_s *logger, btr_log_level_e logLevel)
{
    const char *logLevelString = BTR_LogLevel_toString(logLevel);
    const char *logLevelColor = BTR_LogLevel_getColor(logLevel);

    printf(COL_INTER"[%s%s"COL_INTER"] ", logLevelColor, logLevelString);
    if (logger->file) fprintf(logger->file, "[%s] ", logLevelString);
}
static void doLogFile(btr_logger_s *logger, const char *file, int line, const char *func)
{
    printf(COL_INTER"["COL_RESET"%s", file);
    if (logger->logFunc) printf(COL_INTER"@"COL_RESET"%s", func);
    if (logger->logLine) printf(COL_INTER":"COL_RESET"%d", line);
    printf(COL_INTER"] ");
    if (!logger->file) return;
    fprintf(logger->file, "[%s", file);
    if (logger->logFunc) fprintf(logger->file, "@%s", func);
    if (logger->logLine) fprintf(logger->file, ":%d", line);
    fprintf(logger->file, "] ");
}
void BTR_vlogImpl(
    const char *file,
    int line,
    const char *func,
    btr_log_level_e logLevel,
    const char *formatString,
    va_list args
) {
    btr_logger_s *logger = &BTR_g_logger;

    if (logLevel < logger->minLogLevel) return;

    if (logger->logTime) doLogTime(logger);
    doLogLevel(logger, logLevel);
    if (logger->logFile) doLogFile(logger, file, line, func);

    printf(COL_INTER": "COL_RESET);
    if (logger->file) fprintf(logger->file, ": ");
    
    va_list args_copy;
    va_copy(args_copy, args);
    vprintf(formatString, args);
    if (logger->file) vfprintf(logger->file, formatString, args_copy);
    va_end(args_copy);

    printf("\n");
    if (logger->file) fprintf(logger->file, "\n");
}

void BTR_logImpl(
    const char *file,
    int line,
    const char *func,
    btr_log_level_e logLevel,
    const char *formatString,
    ...
) {
    va_list args;
    va_start(args, formatString);
    BTR_vlogImpl(file, line, func, logLevel, formatString, args);
    va_end(args);
}
