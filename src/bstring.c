#include "btrstd/bstring.h"
#include "string.h"
#include "stdlib.h"

#include "stdio.h"

static size_t utf8CharLen(unsigned char c)
{
    if ((c & 0x80) == 0)    return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 0;
}
btr_string_t BTR_String_new(const char *chars)
{
    char *newChars = malloc(strlen(chars));
    if (!newChars) return (btr_string_t) {0};
    strncpy(newChars, chars, strlen(chars));
    btr_string_t string = {
        .data     = newChars,
        .start    = 0,
        .length   = strlen(chars),
        .capacity = strlen(chars),
    };
    return string;
}
void BTR_String_cropLeft(btr_string_t *string, unsigned int charCount)
{
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t curCharSize = utf8CharLen(*(string->data + string->start));
        if (curCharSize == 0 || curCharSize > string->length) break;
        string->start  += curCharSize;
        string->length -= curCharSize;
    }
}
int BTR_String_compare(btr_string_t *a, btr_string_t *b)
{
    return strcmp(
        a->data + a->start,
        b->data + b->start
    );
}
void BTR_String_free(btr_string_t *string)
{
    free(string->data);
}
