#include "stddef.h"
#include "stdbool.h"
#include "stddef.h"

typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
} string_view_t;

// Creates a string view from an array of bytes.
// The string in question will not own the given data.
string_view_t StringView_fromCString(const char *chars);
// Crops the given count of characters from the left of the string view.
// "გვიპყრობდა" -> crop 3 -> "პყრობდა"
void StringView_cropLeft(string_view_t *, unsigned int charCount);
// Crops the given count of characters from the right of the string view.
// "გვიპყრობდა" -> crop 3 -> "გვიპყრო"
void StringView_cropRight(string_view_t *, unsigned int charCount);
// Reverts the given count of characters from the left of the string view, that were previously cropped.
// "გვიპყრობდა" -> crop 3 -> "პყრობდა" -> revert 2 "ვიპყრობდა"
void StringView_revertLeft(string_view_t *, unsigned int charCount);
// Reverts the given count of characters from the right of the string view, that were previously cropped.
// "გვიპყრობდა" -> crop 3 -> "გვიპყრო" -> revert 2 -> "გვიპყრობდ"
void StringView_revertRight(string_view_t *, unsigned int charCount);
// Returns the size of a view IN BYTES.
size_t StringView_byteCount(string_view_t *);
// Returns the size of a view IN CHARACTERS.
size_t StringView_len(string_view_t *);
// Tells if the string view is empty.
bool StringView_isEmpty(string_view_t *);
// Returns a pointer to a beginning of a UTF-8 character.
// Returns `NULL` if the pointer is invalid.
const char *StringView_charAt(string_view_t *, int index);
// Tells if the string view ends with another string view.
bool StringView_endsWithView(string_view_t *, string_view_t *postfixToSearch);
// Tells if the string view starts with another string view.
bool StringView_startsWithView(string_view_t *, string_view_t *prefixToSearch);
// Tells if the string view ends with a given C string.
bool StringView_endsWithCString(string_view_t *, const char *postfixToSearch);
// Tells if the string view starts with a given C string.
bool StringView_startsWithCString(string_view_t *, const char *prefixToSearch);
// Returns the first instance of a substring in a string view.
// Returns zero-initialised view if didn't find the substring.
string_view_t StringView_findView(string_view_t *, string_view_t *substring);
// Returns the first instance of a substring in a string view.
// Returns zero-initialised view if didn't find the substring.
string_view_t StringView_findCString(string_view_t *, const char *substring);
// Returns a substring with the given start and length.
string_view_t StringView_substring(string_view_t *, unsigned int start, unsigned int count);
// Trims whitespaces from the left of the view.
void StringView_trimLeft(string_view_t *);
// Trims whitespaces from the right of the view.
void StringView_trimRight(string_view_t *);
// Trims whitespaces from left and right of the view.
void StringView_trim(string_view_t *);
// Compares the two given string views.
// Returns 0 if they are equal.
int StringView_compare(string_view_t *, string_view_t *);

// These two macros help with printing these strings out with printf.
// Example usage:
// ```c
// printf("Printing > "STRING_FORMAT"\n", STRING_ARGS(str));
// ```
// Can accept both string_t and string_view_t.
#define STRING_FORMAT "%.*s"
#define STRING_ARGS(STRING) STRING.length, STRING.data + STRING.start

#define StringView_endsWith(STRING, POSTFIX) _Generic((POSTFIX), \
    string_view_t *: StringView_endsWithView,                    \
    const char *: StringView_endsWithCString,                    \
    char *: StringView_endsWithCString                           \
)(STRING, POSTFIX)

#define StringView_startsWith(STRING, POSTFIX) _Generic((POSTFIX), \
    string_view_t *: StringView_startsWithView,                    \
    const char *: StringView_startsWithCString,                    \
    char *: StringView_startsWithCString                           \
)(STRING, POSTFIX)

#define StringView_find(STRING, POSTFIX) _Generic((POSTFIX), \
    string_view_t *: StringView_findView,                    \
    const char *: StringView_findCString,                    \
    char *: StringView_findCString                           \
)(STRING, POSTFIX)

#define StringView_contains(STRING, POSTFIX) _Generic((POSTFIX), \
    string_view_t *: StringView_containsView,                    \
    const char *: StringView_containsCString,                    \
    char *: StringView_containsCString                           \
)(STRING, POSTFIX)
