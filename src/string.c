#include "btrstd/string.h"
#include "btrstd/utf8.h"
#include "_util.c"
#include "string.h"
#include "stdio.h"
#include "ctype.h"

static size_t btr_string_replace_bytes(
    btr_string_s *this,
    const char *from, size_t fromLen,
    const char *to, size_t toLen
) {
    if (fromLen == 0) return 0;
    size_t count = 0;
    size_t i = 0;
    while (i + fromLen <= this->data.count)
    {
        if (memcmp((char *)this->data.data + i, from, fromLen) == 0)
        {
            for (size_t j = 0; j < fromLen; j++)
                BTR_OAList_pop(&this->data, i, NULL);
            for (size_t j = 0; j < toLen; j++)
                *(char *)BTR_OAList_insert(&this->data, i + j) = to[j];
            i += toLen;
            count++;
        }
        else i++;
    }
    return count;
}

btr_string_s BTR_String_fromCString(const char *string, btr_allocator_s *allocator)
{
    btr_allocator_s *theAllocator = getAllocator(allocator);
    btr_oalist_s data = BTR_OAList_make(8, sizeof(char), theAllocator);
    size_t len = strlen(string);
    for (size_t i = 0; i < len; i++)
        *(char *)BTR_OAList_append(&data) = string[i];
    return (btr_string_s) {
        .data = data,
    };
}
btr_string_s BTR_String_clone(btr_string_s *string, btr_allocator_s *allocator)
{
    btr_oalist_s data = BTR_OAList_clone(&string->data, allocator);
    return (btr_string_s) {
        .data = data,
    };
}
btr_string_s BTR_String_fromStringView(btr_string_view_s string, btr_allocator_s *allocator)
{
    btr_allocator_s *theAllocator = getAllocator(allocator);
    btr_oalist_s data = BTR_OAList_make(BTR_StringView_len(string), sizeof(char), theAllocator);
    memcpy(data.data, string.data, string.length);
    data.count += string.length;
    return (btr_string_s) {
        .data = data,
    };
}
btr_string_s BTR_String_new(btr_allocator_s *allocator)
{
    return (btr_string_s) {
        .data = BTR_OAList_make(8, sizeof(char), allocator),
    };
}

void BTR_String_appendCString(btr_string_s *this, const char *data)
{
    size_t len = strlen(data);
    for (size_t i = 0; i < len; i++)
        *(char *)BTR_OAList_append(&this->data) = data[i];
}
void BTR_String_appendChar(btr_string_s *this, char data)
{
    *(char *)BTR_OAList_append(&this->data) = data;
}
void BTR_String_appendString(btr_string_s *this, btr_string_s *data)
{
    for (size_t i = 0; i < data->data.count; i++)
        *(char *)BTR_OAList_append(&this->data) = *(char *)BTR_unwrap(BTR_OAList_get(&data->data, i));
}
void BTR_String_appendStringView(btr_string_s *this, btr_string_view_s data)
{
    for (size_t i = 0; i < data.length; i++)
        *(char *)BTR_OAList_append(&this->data) = *(data.data + i);
}
void BTR_String_prependCString(btr_string_s *this, const char *data)
{
    for (int i = strlen(data) - 1; i >= 0; i--)
        *(char *)BTR_OAList_prepend(&this->data) = data[i];
}
void BTR_String_prependChar(btr_string_s *this, char data)
{
    *(char *)BTR_OAList_prepend(&this->data) = data;
}
void BTR_String_prependString(btr_string_s *this, btr_string_s *data)
{
    for (int i = data->data.count - 1; i >= 0; i--)
        *(char *)BTR_OAList_prepend(&this->data) = *(char *)BTR_unwrap(BTR_OAList_get(&data->data, i));
}
void BTR_String_prependStringView(btr_string_s *this, btr_string_view_s data)
{
    for (int i = data.length - 1; i >= 0; i--)
        *(char *)BTR_OAList_prepend(&this->data) = *(data.data + i);
}

void BTR_String_insertCString(btr_string_s *this, const char *data, int index)
{
    if (index < 0) index = this->data.count + index;
    size_t curByteIndex = 0;
    for (size_t curStringIndex = 0; curStringIndex < (size_t)index; curStringIndex++)
        curByteIndex += BTR_UTF8_charLen(((char *)this->data.data)[curByteIndex]);
    for (size_t i = 0; i < strlen(data); i++)
    {
        *(char *)BTR_OAList_insert(&this->data, curByteIndex) = data[i];
        curByteIndex++;
    }
}
void BTR_String_insertChar(btr_string_s *this, char data, int index)
{
    if (index < 0) index = this->data.count + index;
    size_t curByteIndex = 0;
    for (size_t curStringIndex = 0; curStringIndex < (size_t)index; curStringIndex++)
        curByteIndex += BTR_UTF8_charLen(((char *)this->data.data)[curByteIndex]);
    *(char *)BTR_OAList_insert(&this->data, curByteIndex) = data;
}
void BTR_String_insertString(btr_string_s *this, btr_string_s *data, int index)
{
    if (index < 0) index = this->data.count + index;
    size_t curByteIndex = 0;
    for (size_t curStringIndex = 0; curStringIndex < (size_t)index; curStringIndex++)
        curByteIndex += BTR_UTF8_charLen(((char *)this->data.data)[curByteIndex]);
    for (size_t i = 0; i < data->data.count; i++)
    {
        *(char *)BTR_OAList_insert(&this->data, curByteIndex) = ((char *)data->data.data)[i];
        curByteIndex++;
    }
}
void BTR_String_insertStringView(btr_string_s *this, btr_string_view_s data, int index)
{
    if (index < 0) index = this->data.count + index;
    size_t curByteIndex = 0;
    for (size_t curStringIndex = 0; curStringIndex < (size_t)index; curStringIndex++)
        curByteIndex += BTR_UTF8_charLen(((char *)this->data.data)[curByteIndex]);
    for (size_t i = 0; i < data.length; i++)
    {
        *(char *)BTR_OAList_insert(&this->data, curByteIndex) = *(data.data + i);
        curByteIndex++;
    }
}

void BTR_String_cropLeft(btr_string_s *this, size_t count)
{
    for (size_t i = 0; i < count; i++)
        for (
            int j = BTR_UTF8_charLen(*(char *)BTR_unwrap(BTR_OAList_first(&this->data))) - 1;
            j >= 0;
            j--
        ) BTR_OAList_pop(&this->data, 0, NULL);
}
void BTR_String_cropRight(btr_string_s *this, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        size_t byteCount = BTR_OAList_len(&this->data);
        if (byteCount == 0) break;
        size_t length = 1;
        char curChar = *(char *)BTR_unwrap(BTR_OAList_get(&this->data, BTR_OAList_len(&this->data) - 1));
        while (BTR_UTF8_isContinuation(curChar) && length < byteCount)
        {
            length++;
            curChar = *(char *)BTR_unwrap(BTR_OAList_get(&this->data, BTR_OAList_len(&this->data) - length - 1));
        }
        for (size_t i = 0; i < length; i++)
            BTR_OAList_pop(&this->data, -1, NULL);
    }
}
const char *BTR_String_charAt(btr_string_s *this, long index)
{
    btr_string_view_s view = BTR_String_getView(this);
    return BTR_StringView_charAt(view, index);
}
size_t BTR_String_len(btr_string_s *this)
{
    btr_string_view_s view = BTR_String_getView(this);
    return BTR_StringView_len(view);
}
bool BTR_String_isEmpty(btr_string_s *this)
{
    return BTR_String_len(this) == 0;
}
void BTR_String_pop(btr_string_s *this, int index, char *buffer)
{
    size_t len = BTR_String_len(this);
    if (index < 0) index = (int) len + index;
    char *rawData = this->data.data;
    size_t byteOffset = 0;
    for (size_t curIndex = 0; curIndex < (size_t)index; curIndex++)
        byteOffset += BTR_UTF8_charLen((unsigned char)rawData[byteOffset]);

    size_t charLen = BTR_UTF8_charLen((unsigned char)rawData[byteOffset]);
    for (size_t i = 0; i < charLen; i++)
        BTR_OAList_pop(&this->data, byteOffset, (buffer) ? buffer + i : NULL);
}
void BTR_String_remove(btr_string_s *this, size_t start, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        size_t charlen = BTR_UTF8_charLen(*BTR_String_charAt(this, start));
        for (size_t j = 0; j < charlen; j++)
            BTR_OAList_pop(&this->data, start, NULL);
    }
}
void BTR_String_removeBytes(btr_string_s *this, size_t start, size_t count)
{
    for (size_t i = 0; i < count; i++)
        BTR_OAList_pop(&this->data, start, NULL);
}
void BTR_String_clear(btr_string_s *this)
{
    BTR_OAList_clear(&this->data);
}
void BTR_String_reserve(btr_string_s *this, size_t itemCount) {
    BTR_OAList_reserve(&this->data, itemCount);
}
void BTR_String_reserveNew(btr_string_s *this, size_t itemCount) {
    BTR_OAList_reserveNew(&this->data, itemCount);
}
void BTR_String_cropCapacity(btr_string_s *this) {
    BTR_OAList_cropCapacity(&this->data);
}
btr_string_view_s BTR_String_getView(btr_string_s *this)
{
    return (btr_string_view_s)
    {
        .data = this->data.data,
        .length = this->data.count,
    };
}
int BTR_String_compare(btr_string_s *a, btr_string_s *b)
{
    btr_string_view_s sva = BTR_String_getView(a);
    btr_string_view_s svb = BTR_String_getView(b);
    return BTR_StringView_compare(sva, svb);
}
int BTR_String_compareView(btr_string_s *a, btr_string_view_s b)
{
    btr_string_view_s sva = BTR_String_getView(a);
    return BTR_StringView_compare(sva, b);
}
void BTR_String_free(btr_string_s *this) {
    BTR_OAList_free(&this->data);
}
char *BTR_String_freeToCString(btr_string_s *this, btr_allocator_s *allocator)
{
    char *result = BTR_String_toCString(this, allocator);
    BTR_String_free(this);
    return result;
}
char *BTR_String_toCString(btr_string_s *this, btr_allocator_s *allocator)
{
    btr_allocator_s *theAllocator = (allocator) ? allocator : this->data.allocator;
    char *result = BTR_expect(
        BTR_Allocator_allocate(theAllocator, (this->data.count + 1) * sizeof(char)),
        "Allocation failed"
    );
    memcpy(result, this->data.data, this->data.count * sizeof(char));
    result[this->data.count] = '\0';
    return result;
}

size_t BTR_String_replaceCString(btr_string_s *this, const char *from, const char *to)
{
    return btr_string_replace_bytes(this, from, strlen(from), to, strlen(to));
}
size_t BTR_String_replaceString(btr_string_s *this, btr_string_s from, btr_string_s to)
{
    return btr_string_replace_bytes(this, from.data.data, from.data.count, to.data.data, to.data.count);
}
size_t BTR_String_replaceStringView(btr_string_s *this, btr_string_view_s from, btr_string_view_s to)
{
    return btr_string_replace_bytes(
        this,
        from.data, from.length,
        to.data, to.length
    );
}

void BTR_String_toUpper(btr_string_s *this)
{
    char *data = this->data.data;
    for (size_t i = 0; i < this->data.count; i++)
        if ((unsigned char)data[i] < 0x80)
            data[i] = toupper((unsigned char)data[i]);
}
void BTR_String_toLower(btr_string_s *this)
{
    char *data = this->data.data;
    for (size_t i = 0; i < this->data.count; i++)
        if ((unsigned char)data[i] < 0x80)
            data[i] = tolower((unsigned char)data[i]);
}
