#include "btrstd/string_view.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

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
    if (index < 0)
        index = (int) len + index;
    if ((size_t) index > len || index < 0)
        return NULL;
    char *pointer = string->data + string->start;
    for (size_t counter = 0; counter < (size_t) index; counter++)
        if (counter > len)
            return NULL;
        else
            pointer += utf8CharLen((unsigned char)*pointer);
    return pointer;
}
bool StringView_endsWithView(string_view_t *string, string_view_t *postfix)
{
    size_t len = StringView_byteCount(string);
    size_t lenPostfix = StringView_byteCount(postfix);
    size_t counter = 0;
    while (*(string->data + string->start + len - counter) == *(postfix->data + postfix->start + lenPostfix - counter) && counter < lenPostfix)
        counter++;
    if (counter < lenPostfix)
        return false;
    return true;
}
bool StringView_startsWithView(string_view_t *string, string_view_t *prefix)
{
    size_t counter = 0;
    size_t byteCount = StringView_byteCount(prefix);
    while (*(string->data + string->start + counter) == *(prefix->data + prefix->start + counter) && counter < byteCount)
        counter++;
    if (counter < byteCount)
        return false;
    return true;
}
bool StringView_endsWithCString(string_view_t *string, const char *postfix)
{
    string_view_t view = StringView_fromCString(postfix);
    return StringView_endsWithView(string, &view);
}
bool StringView_startsWithCString(string_view_t *string, const char *prefix)
{
    string_view_t view = StringView_fromCString(prefix);
    return StringView_startsWithView(string, &view);
}
string_view_t StringView_findView(string_view_t *string, string_view_t *substring)
{
    size_t substringSize = StringView_byteCount(substring);
    size_t maxCount = StringView_byteCount(string) - substringSize;
    for (size_t count = 0; count < maxCount; count++)
    {
        if (!memcmp(
            string->data + string->start + count,
            substring->data + substring->start,
            substringSize
        )) return (string_view_t) {
            .data = string->data,
            .start = string->start + count,
            .length = substringSize,
            .capacity = string->capacity,
        };
    }
    return (string_view_t) {0};
}
string_view_t StringView_findCString(string_view_t *string, const char *substring)
{
    string_view_t view = StringView_fromCString(substring);
    return StringView_findView(string, &view);
}
string_view_t StringView_substring(string_view_t *string, unsigned int start, unsigned int count)
{
    size_t byteStart, byteCount;
    size_t counter = 0;
    size_t byteOffset = 0;
    while (string->data + string->start + byteOffset < string->data + string->start + string->length)
    {
        if (counter == start) byteStart = byteOffset;
        if (counter == start + count) byteCount = byteOffset - byteStart;
        byteOffset += utf8CharLen(*(string->data + string->start + byteOffset));
        counter++;
    }
    return (string_view_t) {
        .data = string->data,
        .capacity = string->capacity,
        .start = string->start + byteStart,
        .length = byteCount,
    };
}
void StringView_trimLeft(string_view_t *string)
{
    while (isspace(*(string->data + string->start)))
        StringView_cropLeft(string, 1);
}
void StringView_trimRight(string_view_t *string)
{
    while (isspace(*(string->data + string->start + string->length - 1)))
        StringView_cropRight(string, 1);
}
void StringView_trim(string_view_t *string)
{
    StringView_trimLeft(string);
    StringView_trimRight(string);
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
