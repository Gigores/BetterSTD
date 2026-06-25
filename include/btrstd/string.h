#pragma once

#include "btrstd/containers/owning_array_list.h"
#include "btrstd/string_view.h"

// Growable UTF-8 encoded heap-allocated string which is not not null-terminated.

typedef struct BTR_String {
    btr_oalist_s data;
} btr_string_s;

btr_string_s BTR_String_fromCString(const char *);
btr_string_s BTR_String_clone(btr_string_s *);
btr_string_s BTR_String_fromStringView(btr_string_view_s);
btr_string_s BTR_String_new();
btr_string_view_s BTR_String_getView(btr_string_s *);
void BTR_String_free(btr_string_s *);
char *BTR_String_toCString(btr_string_s *);

#define BTR_String_from(T) _Generic((T), \
    const char *: BTR_String_fromCString, \
    char *: BTR_String_fromCString, \
    btr_string_s *: BTR_String_clone, \
    btr_string_view_s: BTR_String_fromStringView, \
)(T)
