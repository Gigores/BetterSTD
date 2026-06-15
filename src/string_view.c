#include "btrstd/string_view.h"
#include "btrstd/error.h"
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
btr_string_view_t BTR_StringView_fromCString(const char *chars)
{
    BTR_panicIf(!chars, "`chars` is NULL");
    return (btr_string_view_t) {
        .data     = (char *) chars,
        .start    = 0,
        .length   = strlen(chars),
        .capacity = strlen(chars),
    };
}
void BTR_StringView_cropLeft(btr_string_view_t *string, unsigned int charCount)
{
    BTR_panicIf(!string, "`string` is NULL");
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t curCharSize = utf8CharLen(*(string->data + string->start));
        if (curCharSize == 0 || curCharSize > string->length) break;
        string->start  += curCharSize;
        string->length -= curCharSize;
    }
}
void BTR_StringView_cropRight(btr_string_view_t *string, unsigned int charCount)
{
    BTR_panicIf(!string, "`string` is NULL");
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t offset = 0;
        while (isUtf8Continuation(*(string->data + string->start + string->length - offset - 1)))
            offset++;
        size_t curCharSize =
            utf8CharLen(*(string->data + string->start + string->length - offset - 1));
        string->length -= curCharSize;
    }
}
void BTR_StringView_revertLeft(btr_string_view_t *string, unsigned int charCount)
{
    BTR_panicIf(!string, "`string` is NULL");
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
void BTR_StringView_revertRight(btr_string_view_t *string, unsigned int charCount)
{
    BTR_panicIf(!string, "`string` is NULL");
    for (unsigned int i = 0; i < charCount; i++)
    {
        if (string->start + string->length >= string->capacity) return;
        size_t charSize = utf8CharLen((unsigned char)string->data[string->start + string->length]);
        if (charSize == 0) return;
        if (string->start + string->length + charSize > string->capacity) return;
        string->length += charSize;
    }
}
size_t BTR_StringView_byteCount(btr_string_view_t *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    return string->length;
}
size_t BTR_StringView_len(btr_string_view_t *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    size_t count = 0;
    char *currentChar = string->data + string->start;
    while (currentChar < string->data + string->capacity)
    {
        currentChar += utf8CharLen(*currentChar);
        count++;
    }
    return count;
}
bool BTR_StringView_isEmpty(btr_string_view_t *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    return !BTR_StringView_len(string);
}
const char *BTR_StringView_charAt(btr_string_view_t *string, int index)
{
    BTR_panicIf(!string, "`string` is NULL");
    size_t len = BTR_StringView_len(string);
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
bool BTR_StringView_endsWithView(btr_string_view_t *string, btr_string_view_t *postfix)
{
    BTR_panicIf(!string || !postfix, "`string` or `postfix` is NULL");
    size_t len = BTR_StringView_byteCount(string);
    size_t lenPostfix = BTR_StringView_byteCount(postfix);
    size_t counter = 0;
    while (
        *(string->data + string->start + len - counter) ==
        *(postfix->data + postfix->start + lenPostfix - counter) &&
        counter < lenPostfix
    ) counter++;
    if (counter < lenPostfix)
        return false;
    return true;
}
bool BTR_StringView_startsWithView(btr_string_view_t *string, btr_string_view_t *prefix)
{
    BTR_panicIf(!string || !prefix, "`string` or `prefix` is NULL");
    size_t counter = 0;
    size_t byteCount = BTR_StringView_byteCount(prefix);
    while (
        *(string->data + string->start + counter) ==
        *(prefix->data + prefix->start + counter) &&
        counter < byteCount
    ) counter++;
    if (counter < byteCount)
        return false;
    return true;
}
bool BTR_StringView_endsWithCString(btr_string_view_t *string, const char *postfix)
{
    BTR_panicIf(!string || !postfix, "`string` or `postfix` is NULL");
    btr_string_view_t view = BTR_StringView_fromCString(postfix);
    return BTR_StringView_endsWithView(string, &view);
}
bool BTR_StringView_startsWithCString(btr_string_view_t *string, const char *prefix)
{
    BTR_panicIf(!string || !prefix, "`string` or `prefix` is NULL");
    btr_string_view_t view = BTR_StringView_fromCString(prefix);
    return BTR_StringView_startsWithView(string, &view);
}
btr_string_view_t BTR_StringView_findView(btr_string_view_t *string, btr_string_view_t *substring)
{
    BTR_panicIf(!string || !substring, "`string` or `substring` is NULL");
    size_t substringSize = BTR_StringView_byteCount(substring);
    size_t maxCount = BTR_StringView_byteCount(string) - substringSize;
    for (size_t count = 0; count < maxCount; count++)
    {
        if (!memcmp(
            string->data + string->start + count,
            substring->data + substring->start,
            substringSize
        )) return (btr_string_view_t) {
            .data = string->data,
            .start = string->start + count,
            .length = substringSize,
            .capacity = string->capacity,
        };
    }
    return (btr_string_view_t) {0};
}
btr_string_view_t BTR_StringView_findCString(btr_string_view_t *string, const char *substring)
{
    BTR_panicIf(!string || !substring, "`string` or `substring` is NULL");
    btr_string_view_t view = BTR_StringView_fromCString(substring);
    return BTR_StringView_findView(string, &view);
}
btr_string_view_t BTR_StringView_substring(
    btr_string_view_t *string,
    unsigned int start,
    unsigned int count
) {
    BTR_panicIf(!string, "`string` is NULL");
    size_t byteStart = 0, byteCount = 0;
    size_t counter = 0;
    size_t byteOffset = 0;
    while (
        string->data + string->start + byteOffset <
        string->data + string->start + string->length
    ) {
        if (counter == start) byteStart = byteOffset;
        if (counter == start + count) byteCount = byteOffset - byteStart;
        byteOffset += utf8CharLen(*(string->data + string->start + byteOffset));
        counter++;
    }
    return (btr_string_view_t) {
        .data = string->data,
        .capacity = string->capacity,
        .start = string->start + byteStart,
        .length = byteCount,
    };
}
void BTR_StringView_trimLeft(btr_string_view_t *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    while (isspace(*(string->data + string->start)))
        BTR_StringView_cropLeft(string, 1);
}
void BTR_StringView_trimRight(btr_string_view_t *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    while (isspace(*(string->data + string->start + string->length - 1)))
        BTR_StringView_cropRight(string, 1);
}
void BTR_StringView_trim(btr_string_view_t *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    BTR_StringView_trimLeft(string);
    BTR_StringView_trimRight(string);
}
int BTR_StringView_compare(btr_string_view_t *a, btr_string_view_t *b)
{
    BTR_panicIf(!a || !b, "`a` or `b` is NULL");
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
