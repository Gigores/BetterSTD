#include "btrstd/string.h"
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
