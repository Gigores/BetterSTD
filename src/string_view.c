#include "btrstd/string_view.h"
#include "btrstd/error.h"
#include "btrstd/utf8.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

#include "stdio.h"

btr_string_view_s BTR_StringView_fromCString(const char *chars)
{
    BTR_panicIf(!chars, "`chars` is NULL");
    return (btr_string_view_s) {
        .data     = (char *) chars,
        .length   = strlen(chars),
    };
}
void BTR_StringView_cropLeft(btr_string_view_s *string, unsigned int charCount)
{
    BTR_panicIf(!string, "`string` is NULL");
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t curCharSize = BTR_UTF8_charLen(*(string->data));
        if (curCharSize == 0 || curCharSize > string->length) break;
        string->data   += curCharSize;
        string->length -= curCharSize;
    }
}
void BTR_StringView_cropRight(btr_string_view_s *string, unsigned int charCount)
{
    BTR_panicIf(!string, "`string` is NULL");
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t offset = 0;
        while (BTR_UTF8_isContinuation(*(string->data + string->length - offset - 1)))
            offset++;
        size_t curCharSize =
            BTR_UTF8_charLen(*(string->data + string->length - offset - 1));
        string->length -= curCharSize;
    }
}
void BTR_StringView_revertLeft(btr_string_view_s *string, unsigned int charCount)
{
    BTR_panicIf(!string, "`string` is NULL");
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t offset = 0;
        while (BTR_UTF8_isContinuation(
            (unsigned char)string->data[-offset - 1])
        ) offset++;
        size_t charSize = BTR_UTF8_charLen((unsigned char)string->data[-offset - 1]);
        if (charSize == 0) return;
        string->data   -= charSize;
        string->length += charSize;
    }
}
void BTR_StringView_revertRight(btr_string_view_s *string, unsigned int charCount)
{
    BTR_panicIf(!string, "`string` is NULL");
    for (unsigned int i = 0; i < charCount; i++)
    {
        size_t charSize = BTR_UTF8_charLen((unsigned char)string->data[string->length]);
        if (charSize == 0) return;
        string->length += charSize;
    }
}
size_t BTR_StringView_byteCount(btr_string_view_s *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    return string->length;
}
size_t BTR_StringView_len(btr_string_view_s *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    size_t count = 0;
    char *currentChar = string->data;
    while (currentChar < string->data + string->length)
    {
        currentChar += BTR_UTF8_charLen(*currentChar);
        count++;
    }
    return count;
}
bool BTR_StringView_isEmpty(btr_string_view_s *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    return !BTR_StringView_len(string);
}
const char *BTR_StringView_charAt(btr_string_view_s *string, int index)
{
    BTR_panicIf(!string, "`string` is NULL");
    size_t len = BTR_StringView_len(string);
    if (index < 0)
        index = (int) len + index;
    if ((size_t) index > len || index < 0)
        return NULL;
    char *pointer = string->data;
    for (size_t counter = 0; counter < (size_t) index; counter++)
        if (counter > len)
            return NULL;
        else
            pointer += BTR_UTF8_charLen((unsigned char)*pointer);
    return pointer;
}
bool BTR_StringView_endsWithView(btr_string_view_s *string, btr_string_view_s *postfix)
{
    BTR_panicIf(!string || !postfix, "`string` or `postfix` is NULL");
    size_t len = BTR_StringView_byteCount(string);
    size_t lenPostfix = BTR_StringView_byteCount(postfix);
    size_t counter = 0;
    while (
        counter < lenPostfix &&
        *(string->data + len - counter - 1) ==
        *(postfix->data + lenPostfix - counter - 1)
    ) counter++;
    if (counter < lenPostfix)
        return false;
    return true;
}
bool BTR_StringView_startsWithView(btr_string_view_s *string, btr_string_view_s *prefix)
{
    BTR_panicIf(!string || !prefix, "`string` or `prefix` is NULL");
    size_t counter = 0;
    size_t byteCount = BTR_StringView_byteCount(prefix);
    while (
        counter < byteCount &&
        *(string->data + counter) ==
        *(prefix->data + counter)
    ) counter++;
    if (counter < byteCount)
        return false;
    return true;
}
bool BTR_StringView_endsWithCString(btr_string_view_s *string, const char *postfix)
{
    BTR_panicIf(!string || !postfix, "`string` or `postfix` is NULL");
    btr_string_view_s view = BTR_StringView_fromCString(postfix);
    return BTR_StringView_endsWithView(string, &view);
}
bool BTR_StringView_startsWithCString(btr_string_view_s *string, const char *prefix)
{
    BTR_panicIf(!string || !prefix, "`string` or `prefix` is NULL");
    btr_string_view_s view = BTR_StringView_fromCString(prefix);
    return BTR_StringView_startsWithView(string, &view);
}
btr_string_view_s BTR_StringView_findView(btr_string_view_s *string, btr_string_view_s *substring)
{
    BTR_panicIf(!string || !substring, "`string` or `substring` is NULL");
    size_t substringSize = BTR_StringView_byteCount(substring);
    BTR_panicIf(substringSize > BTR_StringView_byteCount(string), "Substring too long");
    size_t maxCount = BTR_StringView_byteCount(string) - substringSize;
    for (size_t count = 0; count < maxCount; count++)
    {
        if (!memcmp(
            string->data + count,
            substring->data,
            substringSize
        )) return (btr_string_view_s) {
            .data = string->data + count,
            .length = substringSize,
        };
    }
    return (btr_string_view_s) {0};
}
btr_string_view_s BTR_StringView_findCString(btr_string_view_s *string, const char *substring)
{
    BTR_panicIf(!string || !substring, "`string` or `substring` is NULL");
    btr_string_view_s view = BTR_StringView_fromCString(substring);
    return BTR_StringView_findView(string, &view);
}
btr_string_view_s BTR_StringView_substring(
    btr_string_view_s *string,
    unsigned int start,
    unsigned int count
) {
    BTR_panicIf(!string, "`string` is NULL");
    size_t byteStart = 0, byteCount = 0;
    size_t counter = 0;
    size_t byteOffset = 0;
    while (
        string->data + byteOffset <
        string->data + string->length
    ) {
        if (counter == start) byteStart = byteOffset;
        if (counter == start + count) byteCount = byteOffset - byteStart;
        byteOffset += BTR_UTF8_charLen(*(string->data + byteOffset));
        counter++;
    }
    if (byteCount == 0 && counter > start)
        byteCount = string->length - byteStart;
    return (btr_string_view_s) {
        .data = string->data + byteStart,
        .length = byteCount,
    };
}
void BTR_StringView_trimLeft(btr_string_view_s *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    while (isspace(*(unsigned char *)(string->data)))
        BTR_StringView_cropLeft(string, 1);
}
void BTR_StringView_trimRight(btr_string_view_s *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    while (isspace(*(unsigned char *)(string->data + string->length - 1)))
        BTR_StringView_cropRight(string, 1);
}
void BTR_StringView_trim(btr_string_view_s *string)
{
    BTR_panicIf(!string, "`string` is NULL");
    BTR_StringView_trimLeft(string);
    BTR_StringView_trimRight(string);
}
int BTR_StringView_compare(btr_string_view_s *a, btr_string_view_s *b)
{
    BTR_panicIf(!a || !b, "`a` or `b` is NULL");
    size_t minLen = a->length < b->length ? a->length : b->length;
    int cmp = memcmp(a->data, b->data, minLen);
    if (cmp != 0)
        return cmp;
    if (a->length < b->length)
        return -1;
    if (a->length > b->length)
        return 1;
    return 0;
}
double BTR_StringView_parseDouble(btr_string_view_s *sv)
{
    char buffer[1024];
    BTR_panicIf(sv->length > sizeof(buffer), "String view too large");
    memcpy(buffer, sv->data, sv->length);
    buffer[sv->length] = '\0';
    return strtod(buffer, NULL);
}
long BTR_StringView_parseLong(btr_string_view_s *sv, int base)
{
    char buffer[1024];
    BTR_panicIf(sv->length > sizeof(buffer), "String view too large");
    memcpy(buffer, sv->data, sv->length);
    buffer[sv->length] = '\0';
    return strtol(buffer, NULL, base);
}
