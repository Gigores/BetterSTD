#include "btrstd/error.h"
#include "btrstd/logger.h"
#include "stdarg.h"
#include "stdio.h"
#include <stdlib.h>


#define COLOR_RESET "\e[0m"
#define COLOR_INTER "\e[38;5;246m"
#define COLOR_FATAL "\e[38;5;196m"
#define COLOR_NUMBER "\e[38;5;39m"

void BTR_panicImpl(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    BTR_vlogImpl(file, line, func, LOG_FATAL, format, args);
    va_end(args);
    fflush(NULL);
    abort();
}
void BTR_panicImplIf(const char *file, int line, const char *func, bool condition, const char *format, ...)
{
    if (!condition) return;
    va_list args;
    va_start(args, format);
    BTR_vlogImpl(file, line, func, LOG_FATAL, format, args);
    va_end(args);
}
