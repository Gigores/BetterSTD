#include "btrstd/error.h"
#include "stdarg.h"
#include "stdio.h"
#include <stdlib.h>

#define COLOR_RESET "\e[0m"
#define COLOR_INTER "\e[38;5;246m"
#define COLOR_FATAL "\e[38;5;196m"
#define COLOR_NUMBER "\e[38;5;39m"

void BTR_panicImpl(const char *file, int line, const char *format, ...)
{
    fprintf(stderr, COLOR_INTER"["COLOR_FATAL"PANIC"COLOR_INTER"] ["COLOR_RESET"%s"COLOR_INTER":"COLOR_RESET"%d"COLOR_INTER"]: "COLOR_RESET, file, line);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputc('\n', stderr);
    fflush(NULL);
    abort();
}
void BTR_panicImplIf(const char *file, int line, bool condition, const char *format, ...)
{
    if (!condition) return;
    va_list args;
    va_start(args, format);
    BTR_panicImpl(file, line, format, args);
    va_end(args);
}
