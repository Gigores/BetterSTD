#include "stddef.h"
#include "stdbool.h"
#include "stddef.h"

// This string view implementation fully supports UTF-8.
// All operations are performed over UTF-8 characters, unless specified otherwis.

typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
} btr_string_view_t;

// Creates a string view from an array of bytes.
// The string in question will not own the given data.
btr_string_view_t BTR_StringView_fromCString(const char *chars);
// Crops the given count of characters from the left of the string view.
// "გვიპყრობდა" -> crop 3 -> "პყრობდა"
void BTR_StringView_cropLeft(btr_string_view_t *, unsigned int charCount);
// Crops the given count of characters from the right of the string view.
// "გვიპყრობდა" -> crop 3 -> "გვიპყრო"
void BTR_StringView_cropRight(btr_string_view_t *, unsigned int charCount);
// Reverts the given count of characters from the left of the string view, that were previously cropped.
// "გვიპყრობდა" -> crop 3 -> "პყრობდა" -> revert 2 "ვიპყრობდა"
void BTR_StringView_revertLeft(btr_string_view_t *, unsigned int charCount);
// Reverts the given count of characters from the right of the string view, that were previously cropped.
// "გვიპყრობდა" -> crop 3 -> "გვიპყრო" -> revert 2 -> "გვიპყრობდ"
void BTR_StringView_revertRight(btr_string_view_t *, unsigned int charCount);
// Returns the size of a view IN BYTES.
size_t BTR_StringView_byteCount(btr_string_view_t *);
// Returns the size of a view IN CHARACTERS.
size_t BTR_StringView_len(btr_string_view_t *);
// Tells if the string view is empty.
bool BTR_StringView_isEmpty(btr_string_view_t *);
// Returns a pointer to a beginning of a UTF-8 character.
// Returns `NULL` if the pointer is invalid.
const char *BTR_StringView_charAt(btr_string_view_t *, int index);
// Tells if the string view ends with another string view.
bool BTR_StringView_endsWithView(btr_string_view_t *, btr_string_view_t *postfixToSearch);
// Tells if the string view starts with another string view.
bool BTR_StringView_startsWithView(btr_string_view_t *, btr_string_view_t *prefixToSearch);
// Tells if the string view ends with a given C string.
bool BTR_StringView_endsWithCString(btr_string_view_t *, const char *postfixToSearch);
// Tells if the string view starts with a given C string.
bool BTR_StringView_startsWithCString(btr_string_view_t *, const char *prefixToSearch);
// Returns the first instance of a substring in a string view.
// Returns zero-initialised view if didn't find the substring.
btr_string_view_t BTR_StringView_findView(btr_string_view_t *, btr_string_view_t *substring);
// Returns the first instance of a substring in a string view.
// Returns zero-initialised view if didn't find the substring.
btr_string_view_t BTR_StringView_findCString(btr_string_view_t *, const char *substring);
// Returns a substring with the given start and length.
btr_string_view_t BTR_StringView_substring(btr_string_view_t *, unsigned int start, unsigned int count);
// Trims whitespaces from the left of the view.
void BTR_StringView_trimLeft(btr_string_view_t *);
// Trims whitespaces from the right of the view.
void BTR_StringView_trimRight(btr_string_view_t *);
// Trims whitespaces from left and right of the view.
void BTR_StringView_trim(btr_string_view_t *);
// Compares the two given string views.
// Returns 0 if they are equal.
int BTR_StringView_compare(btr_string_view_t *, btr_string_view_t *);

// These two macros help with printing these strings out with printf.
// Example usage:
// ```c
// printf("Printing > "STRING_FORMAT"\n", STRING_ARGS(str));
// ```
// Can accept both string_t and string_view_t.
#define BTR_STRING_FORMAT "%.*s"
#define BTR_STRING_ARGS(STRING) STRING.length, STRING.data + STRING.start

#define BTR_StringView_endsWith(STRING, POSTFIX) _Generic((POSTFIX), \
    btr_string_view_t *: BTR_StringView_endsWithView,                   \
    const char *: BTR_StringView_endsWithCString,                        \
    char *: BTR_StringView_endsWithCString                               \
)(STRING, POSTFIX)

#define BTR_StringView_startsWith(STRING, POSTFIX) _Generic((POSTFIX), \
    btr_string_view_t *: BTR_StringView_startsWithView,                   \
    const char *: BTR_StringView_startsWithCString,                        \
    char *: BTR_StringView_startsWithCString                               \
)(STRING, POSTFIX)

#define BTR_StringView_find(STRING, POSTFIX) _Generic((POSTFIX), \
    btr_string_view_t *: BTR_StringView_findView,                   \
    const char *: BTR_StringView_findCString,                        \
    char *: BTR_StringView_findCString                               \
)(STRING, POSTFIX)

#define BTR_StringView_contains(STRING, POSTFIX) _Generic((POSTFIX), \
    btr_string_view_t *: BTR_StringView_containsView,                    \
    const char *: BTR_StringView_containsCString,                        \
    char *: BTR_StringView_containsCString                               \
)(STRING, POSTFIX)

#ifdef BTR_NO_PREFIX

typedef btr_string_view_t string_view_t;

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

#define STRING_FORMAT                BTR_STRING_FORMAT
#define STRING_ARGS                  BTR_STRING_ARGS

#define StringView_endsWith          BTR_StringView_endsWith
#define StringView_startsWith        BTR_StringView_startsWith
#define StringView_find              BTR_StringView_find
#define StringView_contains          BTR_StringView_contains

#endif // BTR_NO_PREFIX
