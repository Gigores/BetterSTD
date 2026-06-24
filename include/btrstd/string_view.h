#pragma once

#include "stdbool.h"
#include "stddef.h"

// String View

// String view is an object that represents a string. But unlike regular C strings,
// which are pointers to character array, terminated by a \0 character, string view
// doesn't need to be terminated.
// String view stores its length, as well as a start offset, which helps efficiently
// slice and manipulate memory without destructive operations.
//     strView{length, start, data}
//                │      │      └▷  [This is a test string!0]
//                │      └▷               |
//                └▷                      ---------
// When printed: "is a test"
// It also doesn't own the string it points to, which helps save memory.

// This string view implementation fully supports UTF-8.
// All operations are performed over UTF-8 characters, unless specified otherwise.

typedef struct BTR_StringView {
    char *data;
    size_t start;
    size_t length;
    size_t capacity;
} btr_string_view_s;

// Creates a string view from an array of bytes.
// The string in question will not own the given data.
btr_string_view_s BTR_StringView_fromCString(const char *chars);
// Crops the given count of characters from the left of the string view.
// "გვიპყრობდა" -> crop 3 -> "პყრობდა"
void BTR_StringView_cropLeft(btr_string_view_s *, unsigned int charCount);
// Crops the given count of characters from the right of the string view.
// "გვიპყრობდა" -> crop 3 -> "გვიპყრო"
void BTR_StringView_cropRight(btr_string_view_s *, unsigned int charCount);
// Reverts the given count of characters from the left of the string view, that were
// previously cropped.
// "გვიპყრობდა" -> crop 3 -> "პყრობდა" -> revert 2 "ვიპყრობდა"
void BTR_StringView_revertLeft(btr_string_view_s *, unsigned int charCount);
// Reverts the given count of characters from the right of the string view, that were
// previously cropped.
// "გვიპყრობდა" -> crop 3 -> "გვიპყრო" -> revert 2 -> "გვიპყრობდ"
void BTR_StringView_revertRight(btr_string_view_s *, unsigned int charCount);
// Returns the size of a view IN BYTES.
size_t BTR_StringView_byteCount(btr_string_view_s *);
// Returns the size of a view IN CHARACTERS.
size_t BTR_StringView_len(btr_string_view_s *);
// Tells if the string view is empty.
bool BTR_StringView_isEmpty(btr_string_view_s *);
// Returns a pointer to a beginning of a UTF-8 character.
// Returns `NULL` if the pointer is invalid.
const char *BTR_StringView_charAt(btr_string_view_s *, int index);
// Tells if the string view ends with another string view.
bool BTR_StringView_endsWithView(btr_string_view_s *, btr_string_view_s *postfixToSearch);
// Tells if the string view starts with another string view.
bool BTR_StringView_startsWithView(btr_string_view_s *, btr_string_view_s *prefixToSearch);
// Tells if the string view ends with a given C string.
bool BTR_StringView_endsWithCString(btr_string_view_s *, const char *postfixToSearch);
// Tells if the string view starts with a given C string.
bool BTR_StringView_startsWithCString(btr_string_view_s *, const char *prefixToSearch);
// Returns the first instance of a substring in a string view.
// Returns zero-initialised view if didn't find the substring.
btr_string_view_s BTR_StringView_findView(btr_string_view_s *, btr_string_view_s *substring);
// Returns the first instance of a substring in a string view.
// Returns zero-initialised view if didn't find the substring.
btr_string_view_s BTR_StringView_findCString(btr_string_view_s *, const char *substring);
// Returns a substring with the given start and length.
btr_string_view_s BTR_StringView_substring
    (btr_string_view_s *, unsigned int start, unsigned int count);
// Trims whitespaces from the left of the view.
void BTR_StringView_trimLeft(btr_string_view_s *);
// Trims whitespaces from the right of the view.
void BTR_StringView_trimRight(btr_string_view_s *);
// Trims whitespaces from left and right of the view.
void BTR_StringView_trim(btr_string_view_s *);
// Compares the two given string views.
// Returns 0 if they are equal.
int BTR_StringView_compare(btr_string_view_s *, btr_string_view_s *);
// Converts the given string view to double.
double BTR_StringView_parseDouble(btr_string_view_s *sv);
// Converts the given string view to long.
long BTR_StringView_parseLong(btr_string_view_s *sv, int base);

// These two macros help with printing these strings out with printf.
// Example usage:
// ```c
// printf("Printing > "STRING_FORMAT"\n", STRING_ARGS(str));
// ```
#define BTR_STRING_FORMAT "%.*s"
#define BTR_STRING_ARGS(STRING) STRING.length, STRING.data + STRING.start

#define BTR_StringView_endsWith(STRING, POSTFIX) _Generic((POSTFIX), \
    btr_string_view_s *: BTR_StringView_endsWithView,                \
    const char *: BTR_StringView_endsWithCString,                    \
    char *: BTR_StringView_endsWithCString                           \
)(STRING, POSTFIX)

#define BTR_StringView_startsWith(STRING, POSTFIX) _Generic((POSTFIX), \
    btr_string_view_s *: BTR_StringView_startsWithView,                \
    const char *: BTR_StringView_startsWithCString,                    \
    char *: BTR_StringView_startsWithCString                           \
)(STRING, POSTFIX)

#define BTR_StringView_find(STRING, POSTFIX) _Generic((POSTFIX), \
    btr_string_view_s *: BTR_StringView_findView,                \
    const char *: BTR_StringView_findCString,                    \
    char *: BTR_StringView_findCString                           \
)(STRING, POSTFIX)

#ifdef BTR_NO_PREFIX

#define StringView BTR_StringView

typedef btr_string_view_s string_view_s;

#define StringView_fromCString       BTR_StringView_fromCString
#define StringView_cropLeft          BTR_StringView_cropLeft
#define StringView_cropRight         BTR_StringView_cropRight
#define StringView_revertLeft        BTR_StringView_revertLeft
#define StringView_revertRight       BTR_StringView_revertRight
#define StringView_byteCount         BTR_StringView_byteCount
#define StringView_len               BTR_StringView_len
#define StringView_isEmpty           BTR_StringView_isEmpty
#define StringView_charAt            BTR_StringView_charAt

#define StringView_endsWithView      BTR_StringView_endsWithView
#define StringView_startsWithView    BTR_StringView_startsWithView

#define StringView_endsWithCString   BTR_StringView_endsWithCString
#define StringView_startsWithCString BTR_StringView_startsWithCString

#define StringView_findView          BTR_StringView_findView
#define StringView_findCString       BTR_StringView_findCString

#define StringView_substring         BTR_StringView_substring

#define StringView_trimLeft          BTR_StringView_trimLeft
#define StringView_trimRight         BTR_StringView_trimRight
#define StringView_trim              BTR_StringView_trim

#define StringView_compare           BTR_StringView_compare
#define StringView_parseDouble       BTR_StringView_parseDouble
#define StringView_parseLong         BTR_StringView_parseLong

#define STRING_FORMAT                BTR_STRING_FORMAT
#define STRING_ARGS                  BTR_STRING_ARGS

#define StringView_endsWith          BTR_StringView_endsWith
#define StringView_startsWith        BTR_StringView_startsWith
#define StringView_find              BTR_StringView_find

#endif // BTR_NO_PREFIX
