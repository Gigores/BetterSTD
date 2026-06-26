#include "btrstd/utf8.h"

size_t BTR_UTF8_charLen(unsigned char c)
{
    if ((c & 0x80) == 0)    return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 0;
}
bool BTR_UTF8_isContinuation(unsigned char c)
{
    return (c & 0xC0) == 0x80;
}
