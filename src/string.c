#include "btrstd/string.h"
#include "btrstd/utf8.h"
#include "_util.c"
#include "string.h"
#include "stdio.h"

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
    btr_oalist_s data = BTR_OAList_make(BTR_StringView_len(&string), sizeof(char), theAllocator);
    memcpy(data.data, string.data + string.start, string.length);
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
        *(char *)BTR_OAList_append(&this->data) = *(data.data + data.start + i);
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
        *(char *)BTR_OAList_prepend(&this->data) = *(data.data + data.start + i);
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
        size_t length = 1;
        char curChar = *(char *)BTR_unwrap(BTR_OAList_get(&this->data, BTR_OAList_len(&this->data) - 1));
        while (BTR_UTF8_isContinuation(curChar)) {
            length++;
            curChar = *(char *)BTR_unwrap(BTR_OAList_get(&this->data, BTR_OAList_len(&this->data) - length - 1));
        }
        for (size_t i = 0; i < length; i++)
            BTR_OAList_pop(&this->data, -1, NULL);
    }
}
const char *BTR_String_charAt(btr_string_s *this, int index)
{
    btr_string_view_s view = BTR_String_getView(this);
    return BTR_StringView_charAt(&view, index);
}
size_t BTR_String_len(btr_string_s *this)
{
    btr_string_view_s view = BTR_String_getView(this);
    return BTR_StringView_len(&view);
}
void BTR_String_pop(btr_string_s *this, int index, char *buffer)
{
    size_t len = BTR_String_len(this);
    if (index < 0) index = (int) len + index;
    size_t charIndex = 0;
    for (size_t curIndex = 0; curIndex < (size_t)index; curIndex++)
        charIndex += BTR_UTF8_charLen(*BTR_String_charAt(this, charIndex));
    size_t charLen = BTR_UTF8_charLen(*BTR_String_charAt(this, charIndex));
    for (size_t i = 0; i < charLen; i++)
        BTR_OAList_pop(&this->data, charIndex, (buffer) ? buffer + i : NULL);
}
btr_string_view_s BTR_String_getView(btr_string_s *this)
{
    return (btr_string_view_s)
    {
        .data = this->data.data,
        .capacity = this->data.count,
        .length = this->data.count,
    };
}
int BTR_String_compare(btr_string_s *a, btr_string_s *b)
{
    btr_string_view_s sva = BTR_String_getView(a);
    btr_string_view_s svb = BTR_String_getView(b);
    return BTR_StringView_compare(&sva, &svb);
}
int BTR_String_compareView(btr_string_s *a, btr_string_view_s b)
{
    btr_string_view_s sva = BTR_String_getView(a);
    return BTR_StringView_compare(&sva, &b);
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
