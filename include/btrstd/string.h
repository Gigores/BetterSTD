#pragma once

#include "btrstd/containers/owning_array_list.h"
#include "btrstd/string_view.h"

// Growable UTF-8 encoded heap-allocated string which is not not null-terminated.

typedef struct BTR_String {
    btr_oalist_s data;
} btr_string_s;

// Creates a new String from a c-style string.
// The string gets copied without the null byte at the end.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_string_s BTR_String_fromCString(const char *, btr_allocator_s *allocator);
// Creates a new String with the same contents as the input String.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_string_s BTR_String_clone(btr_string_s *, btr_allocator_s *allocator);
// Creates a new String with the same contents as the input String View.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_string_s BTR_String_fromStringView(btr_string_view_s, btr_allocator_s *allocator);
// Creates a new empty string.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_string_s BTR_String_new(btr_allocator_s *allocator);

// APPEND
// Appends the given c-style null-terminated string to the end of the given String.
void BTR_String_appendCString(btr_string_s *, const char *data);
// Appends the given single character to the end of the given String.
void BTR_String_appendChar(btr_string_s *, char data);
// Appends the given String to the end of the given String.
void BTR_String_appendString(btr_string_s *, btr_string_s *data);
// Appends the given String View to the end of the given String.
void BTR_String_appendStringView(btr_string_s *, btr_string_view_s data);
// PREPEND
// Prepends the given c-style null-terminated string to the beginning of the given String.
void BTR_String_prependCString(btr_string_s *, const char *data);
// Prepends the given single character to the beginning of the given String.
void BTR_String_prependChar(btr_string_s *, char data);
// Prepends the given String to the beginning of the given String.
void BTR_String_prependString(btr_string_s *, btr_string_s *data);
// Prepends the given String View to the beginning of the given String.
void BTR_String_prependStringView(btr_string_s *, btr_string_view_s data);
// INSERT
// Inserts the given c-style null-terminated string for it's first character
// to be at the specified index in the given String.
void BTR_String_insertCString(btr_string_s *, const char *data, int index);
// Inserts the given single character for it's first character to be at the
// specified index in the given String.
void BTR_String_insertChar(btr_string_s *, char data, int index);
// Inserts the given String for it's first character to be at the specified
// index in the given String.
void BTR_String_insertString(btr_string_s *, btr_string_s *data, int index);
// Inserts the given String View for it's first character to be at the
// specified index in the given String.
void BTR_String_insertStringView(btr_string_s *, btr_string_view_s data, int index);

// REPLACE
// Replaces all instances of given c-style null-terminated string with
// another c-style null-terminated string.
size_t BTR_String_replaceCString(btr_string_s *, const char *from, const char *to);
// Replaces all instances of given string with another string.
size_t BTR_String_replaceString(btr_string_s *, btr_string_s from, btr_string_s to);
// Replaces all instances of given string view with another string view.
size_t BTR_String_replaceStringView(btr_string_s *, btr_string_view_s from, btr_string_view_s to);

// Crops the given amount of UTF-8 characters from
// the left of the given string.
void BTR_String_cropLeft(btr_string_s *, size_t count);
// Crops the given amount of UTF-8 characters from
// the right of the given string.
void BTR_String_cropRight(btr_string_s *, size_t count);
// Returns a pointer to a beginning of a UTF-8 character of the
// given index. Returns `NULL` if the pointer is invalid.
const char *BTR_String_charAt(btr_string_s *, int index);
// Returns the amount of UTF-8 characters in the string.
size_t BTR_String_len(btr_string_s *);
// Tells if the function is empty.
bool BTR_String_isEmpty(btr_string_s *);
// Deletes a UTF-8 character at the given index.
// The character (all of its bytes) is being copied.
// If the buffer is NULL, the character is not getting copied.
void BTR_String_pop(btr_string_s *, int index, char *buffer);
// Returns the given amount of UTF-8 characters at the given index.
void BTR_String_remove(btr_string_s *, size_t start, size_t count);
// Returns the given amount of bytes at the given index.
void BTR_String_removeBytes(btr_string_s *, size_t start, size_t count);
// Clears the string
void BTR_String_clear(btr_string_s *);

// Reallocates the String with new byte capacity of
// max(this.capacity, byteCount)
void BTR_String_reserve(btr_string_s *, size_t byteCount);
// Reallocates the String with new byte capacity of
// this.count + byteCount
void BTR_String_reserveNew(btr_string_s *, size_t byteCount);
// Reallocates the String with new byte capacity of
// this.count
void BTR_String_cropCapacity(btr_string_s *);

// Converts all lowercase characters into uppercase.
void BTR_String_toUpper(btr_string_s *);
// Converts all uppercase characters into lowercase.
void BTR_String_toLower(btr_string_s *);

// Returns a String View poining to the given String.
btr_string_view_s BTR_String_getView(btr_string_s *);
// Compares two Strings between each other.
// Returns 0 if they are equal.
int BTR_String_compare(btr_string_s *, btr_string_s *);
// Compares a String and a String View between each other.
// Returns 0 if they are equal.
int BTR_String_compareView(btr_string_s *, btr_string_view_s);
// Deallocates the given String.
void BTR_String_free(btr_string_s *);
// Deallocates the given String and allocates a new c-style null-terminated
// string with the same content.
// The `allocator` parameter can be set as `NULL`, in this case it will use the same allocator which the string uses.
char *BTR_String_freeToCString(btr_string_s *, btr_allocator_s *allocator);
// Converts the given String to c-style null terminated string.
// The result string must be freed.
// The `allocator` parameter can be set as `NULL`, in this case it will use the same allocator which the string uses.
char *BTR_String_toCString(btr_string_s *, btr_allocator_s *allocator);

#define BTR_String_from(T, allocator) _Generic((T), \
    const char *: BTR_String_fromCString,           \
    char *: BTR_String_fromCString,                 \
    btr_string_s *: BTR_String_clone,               \
    btr_string_view_s: BTR_String_fromStringView    \
)(T, allocator)
#define BTR_String_append(str, T) _Generic((T),    \
    const char *: BTR_String_appendCString,        \
    char *: BTR_String_appendCString,              \
    char: BTR_String_appendChar,                   \
    btr_string_s *: BTR_String_appendString,       \
    btr_string_view_s: BTR_String_appendStringView \
)(str, T)
#define BTR_String_prepend(str, T) _Generic((T),    \
    const char *: BTR_String_prependCString,        \
    char *: BTR_String_prependCString,              \
    char: BTR_String_prependChar,                   \
    btr_string_s *: BTR_String_prependString,       \
    btr_string_view_s: BTR_String_prependStringView \
)(str, T)
#define BTR_String_insert(str, T, index) _Generic((T), \
    const char *: BTR_String_insertCString,            \
    char *: BTR_String_insertCString,                  \
    char: BTR_String_insertChar,                       \
    btr_string_s *: BTR_String_insertString,           \
    btr_string_view_s: BTR_String_insertStringView     \
)(str, T, index)
#define BTR_String_replace(str, T, to) _Generic((T), \
    const char *: BTR_String_replaceCString,            \
    char *: BTR_String_replaceCString,                  \
    btr_string_s *: BTR_String_replaceString,           \
    btr_string_view_s: BTR_String_replaceStringView     \
)(str, T, to)
