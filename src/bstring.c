#include "btrstd/bstring.h"
#include "string.h"
#include "stdlib.h"

static size_t utf8CharLen(unsigned char c)
{
    if ((c & 0x80) == 0)    return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 0;
}
static bool isUtf8Continuation(unsigned char c)
{
    return (c & 0xC0) == 0x80;
}
string_t String_clone(const char *chars)
{
    char *newChars = malloc(strlen(chars));
    if (!newChars) return (string_t) {0};
    strncpy(newChars, chars, strlen(chars));
    string_t string = {
        .data     = newChars,
        .start    = 0,
        .length   = strlen(chars),
        .capacity = strlen(chars),
    };
    return string;
}
void String_cropLeft(string_t *string, unsigned int charCount)
{
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t curCharSize = utf8CharLen(*(string->data + string->start));
        if (curCharSize == 0 || curCharSize > string->length) break;
        string->start  += curCharSize;
        string->length -= curCharSize;
    }
}
void String_cropRight(string_t *string, unsigned int charCount)
{
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t offset = 0;
        while (isUtf8Continuation(*(string->data + string->start + string->length - offset - 1)))
            offset++;
        size_t curCharSize = utf8CharLen(*(string->data + string->start + string->length - offset - 1));
        string->length -= curCharSize;
    }
}
int String_compare(string_t *a, string_t *b)
{
    size_t minLen = a->length < b->length ? a->length : b->length;
    int cmp = memcmp(a->data + a->start, b->data + b->start, minLen);
    if (cmp != 0)
        return cmp;
    if (a->length < b->length)
        return -1;
    if (a->length > b->length)
        return 1;
    return 0;
}
int String_compareView(string_t *a, string_view_t *b)
{
    size_t minLen = a->length < b->length ? a->length : b->length;
    int cmp = memcmp(a->data + a->start, b->data + b->start, minLen);
    if (cmp != 0)
        return cmp;
    if (a->length < b->length)
        return -1;
    if (a->length > b->length)
        return 1;
    return 0;
}
void String_free(string_t *string)
{
    free(string->data);
}
string_view_t StringView_fromCString(const char *chars)
{
    return (string_view_t) {
        .data     = (char *) chars,
        .start    = 0,
        .length   = strlen(chars),
        .capacity = strlen(chars),
    };
}
void StringView_cropLeft(string_view_t *string, unsigned int charCount)
{
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t curCharSize = utf8CharLen(*(string->data + string->start));
        if (curCharSize == 0 || curCharSize > string->length) break;
        string->start  += curCharSize;
        string->length -= curCharSize;
    }
}
void StringView_cropRight(string_view_t *string, unsigned int charCount)
{
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t offset = 0;
        while (isUtf8Continuation(*(string->data + string->start + string->length - offset - 1)))
            offset++;
        size_t curCharSize = utf8CharLen(*(string->data + string->start + string->length - offset - 1));
        string->length -= curCharSize;
    }
}
int StringView_compare(string_view_t *a, string_view_t *b)
{
    size_t minLen = a->length < b->length ? a->length : b->length;
    int cmp = memcmp(a->data + a->start, b->data + b->start, minLen);
    if (cmp != 0)
        return cmp;
    if (a->length < b->length)
        return -1;
    if (a->length > b->length)
        return 1;
    return 0;
}
