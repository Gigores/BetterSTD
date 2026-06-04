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
void StringView_revertLeft(string_view_t *string, unsigned int charCount)
{
    for (unsigned int i = 0; i < charCount; i++)
    {
        if (string->start == 0) return;
        size_t offset = 0;
        while (string->start > offset + 1 && isUtf8Continuation(
            (unsigned char)string->data[string->start - offset - 1])
        ) offset++;
        size_t charSize = utf8CharLen((unsigned char)string->data[string->start - offset - 1]);
        if (charSize == 0 || charSize > string->start) return;
        string->start  -= charSize;
        string->length += charSize;
    }
}
void StringView_revertRight(string_view_t *string, unsigned int charCount)
{
    for (unsigned int i = 0; i < charCount; i++)
    {
        if (string->start + string->length >= string->capacity) return;
        size_t charSize = utf8CharLen((unsigned char)string->data[string->start + string->length]);
        if (charSize == 0) return;
        if (string->start + string->length + charSize > string->capacity) return;
        string->length += charSize;
    }
}
size_t StringView_byteCount(string_view_t *string)
{
    return string->length;
}
size_t StringView_len(string_view_t *string)
{
    size_t count = 0;
    char *currentChar = string->data + string->start;
    while (currentChar < string->data + string->capacity)
    {
        currentChar += utf8CharLen(*currentChar);
        count++;
    }
    return count;
}
bool StringView_isEmpty(string_view_t *string)
{
    return !StringView_len(string);
}
const char *StringView_charAt(string_view_t *string, int index)
{
    size_t len = StringView_len(string);
    if (index < 0) index = len + index;
    char *pointer = string->data + string->start;
    if ((size_t) index > len) return NULL;
    for (size_t counter = 0; counter < (size_t) index; counter++)
        if (counter > len)
            return NULL;
        else
            pointer += utf8CharLen(*pointer);
    return pointer;
}
bool StringView_endsWithView(string_view_t *string, string_view_t *postfixToSearch);
bool StringView_startsWithView(string_view_t *string, string_view_t *prefixToSearch);
bool StringView_endsWithString(string_view_t *string, string_view_t *postfixToSearch);
bool StringView_startsWithString(string_view_t *string, string_view_t *prefixToSearch);
bool StringView_endsWithCString(string_view_t *string, string_view_t *postfixToSearch);
bool StringView_startsWithCString(string_view_t *string, string_view_t *prefixToSearch);
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
