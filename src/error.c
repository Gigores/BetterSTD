#include "btrstd/error.h"
#include "stdarg.h"
#include "stdio.h"
#include <stdlib.h>

void BTR_panicNormal(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    BTR_panicErrCode(1, format, args);
    va_end(args);
}
void BTR_panicErrCode(int errCode, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    exit(errCode);
}
void BTR_panicNormalIf(bool condition, const char *format, ...)
{
    if (!condition) return;
    va_list args;
    va_start(args, format);
    BTR_panicNormal(format, args);
    va_end(args);
}
void BTR_panicErrCodeIf(bool condition, int errCode, const char *format, ...)
{
    if (!condition) return;
    va_list args;
    va_start(args, format);
    BTR_panicErrCode(errCode, format, args);
    va_end(args);
}
