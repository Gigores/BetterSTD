#include "btrstd/logger.h"
#include "stdarg.h"
#include "stdio.h"
#include "time.h"

#define COL_RESET "\e[0m"
#define COL_INTER "\e[38;5;246m"

static btr_logger_t BTR_g_logger = {
    .minLogLevel = LOG_DEBUG,
    .file = NULL,
    .logFile = true,
    .logTime = true,
    .logFunc = true,
    .logLine = true,
};

void BTR_setLogger(btr_logger_t logger)
{
    BTR_g_logger = logger;
}
btr_logger_t *BTR_getLogger(void)
{
    return &BTR_g_logger;
}

const char *BTR_LogLevel_toString(btr_log_level_t level)
{
    switch (level) {
#define X(LEVEL, COLOR) case (LOG_##LEVEL): return #LEVEL;
        BTR_LOG_LEVELS
#undef X
        default: return "INVALID_LOG_LEVEL";
    }
}
const char *BTR_LogLevel_getColor(btr_log_level_t level)
{
    switch (level) {
#define X(LEVEL, COLOR) case (LOG_##LEVEL): return COLOR;
        BTR_LOG_LEVELS
#undef X
        default: return COL_RESET;
    }
}
static void doLogTime(btr_logger_t *logger)
{
    time_t currentTime = time(NULL);
    struct tm *t = localtime(&currentTime);
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", t);
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    long millis = ts.tv_nsec / 1000000;

    printf(COL_INTER"["COL_RESET"%s.%03ld"COL_INTER"] ", buffer, millis);
    if (logger->file) fprintf(logger->file, "[%s.%03ld] ", buffer, millis);
}
static void doLogLevel(btr_logger_t *logger, btr_log_level_t logLevel)
{
    const char *logLevelString = BTR_LogLevel_toString(logLevel);
    const char *logLevelColor = BTR_LogLevel_getColor(logLevel);

    printf(COL_INTER"[%s%s"COL_INTER"] ", logLevelColor, logLevelString);
    if (logger->file) fprintf(logger->file, "[%s] ", logLevelString);
}
static void doLogFile(btr_logger_t *logger, const char *file, int line, const char *func)
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
void BTR_logImpl(const char *file, int line, const char *func, btr_log_level_t logLevel, const char *formatString, ...)
{
    btr_logger_t *logger = &BTR_g_logger;

    if (logLevel < logger->minLogLevel) return;

    if (logger->logTime) doLogTime(logger);
    doLogLevel(logger, logLevel);
    if (logger->logFile) doLogFile(logger, file, line, func);

    printf(COL_INTER": "COL_RESET);
    if (logger->file) fprintf(logger->file, ": ");
    
    va_list args;
    va_start(args, formatString);
    vprintf(formatString, args);
    if (logger->file) vfprintf(logger->file, formatString, args);
    va_end(args);

    printf("\n");
    if (logger->file) fprintf(logger->file, "\n");
}
