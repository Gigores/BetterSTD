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
// Converts the given String to c-style null terminated string.
// The result string must be freed.
// The `allocator` parameter can be set as `NULL`, in this case it will use the same allocator which the string uses.
char *BTR_String_toCString(btr_string_s *, btr_allocator_s *allocator);

#define BTR_String_from(T) _Generic((T), \
    const char *: BTR_String_fromCString, \
    char *: BTR_String_fromCString, \
    btr_string_s *: BTR_String_clone, \
    btr_string_view_s: BTR_String_fromStringView, \
)(T)
