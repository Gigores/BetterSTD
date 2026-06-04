#include "stddef.h"
#include "stdbool.h"
#include "stddef.h"

// Owns the data
typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
} string_t;

// Doesn't own the data
typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
} string_view_t;

/**
 * Creates a string from an array of bytes.
 * Can return a zero-initialised structure if fails to allocate memory.
 */
string_t String_clone(const char *chars);
/**
 * Crops the given count of characters from the left of the string.
 * "გვიპყრობდა" -> crop 3 -> "პყრობდა"
 */
void String_cropLeft(string_t *, unsigned int charCount);
/**
 * Crops the given count of characters from the right of the string.
 * "გვიპყრობდა" -> crop 3 -> "გვიპყრო"
 */
void String_cropRight(string_t *, unsigned int charCount);
/**
 * Compares the two given strings.
 * Returns 0 if they are equal.
 */
int String_compare(string_t *, string_t *);
/**
 * Compares the given string and a string view.
 * Returns 0 if they are equal.
 */
int String_compareView(string_t *, string_view_t *);
string_view_t String_getView(string_t *);
/**
 * Frees the memory of a string.
 */
void String_free(string_t *);

/**
 * Creates a string view from an array of bytes.
 * The string in question will not own the given data.
 */
string_view_t StringView_fromCString(const char *chars);
/**
 * Crops the given count of characters from the left of the string view.
 * "გვიპყრობდა" -> crop 3 -> "პყრობდა"
 */
void StringView_cropLeft(string_view_t *, unsigned int charCount);
/**
 * Crops the given count of characters from the right of the string view.
 * "გვიპყრობდა" -> crop 3 -> "გვიპყრო"
 */
void StringView_cropRight(string_view_t *, unsigned int charCount);
/**
 * Reverts the given count of characters from the left of the string view, that were previously cropped.
 * "გვიპყრობდა" -> crop 3 -> "პყრობდა" -> revert 2 "ვიპყრობდა"
 */
void StringView_revertLeft(string_view_t *, unsigned int charCount);
/**
 * Reverts the given count of characters from the right of the string view, that were previously cropped.
 * "გვიპყრობდა" -> crop 3 -> "გვიპყრო" -> revert 2 -> "გვიპყრობდ"
 */
void StringView_revertRight(string_view_t *, unsigned int charCount);
/**
 * Returns the size of a view IN BYTES.
 */
size_t StringView_byteCount(string_view_t *);
/**
 * Returns the size of a view IN CHARACTERS.
 */
size_t StringView_len(string_view_t *);
/**
 * Tells if the string view is empty.
 */
bool StringView_isEmpty(string_view_t *);
/**
 * Returns a pointer to a beginning of a UTF-8 character.
 * Returns `NULL` if the pointer is invalid.
 */
const char *StringView_charAt(string_view_t *, int index);
bool StringView_endsWithView(string_view_t *, string_view_t *postfixToSearch);
bool StringView_startsWithView(string_view_t *, string_view_t *prefixToSearch);
bool StringView_endsWithString(string_view_t *, string_view_t *postfixToSearch);
bool StringView_startsWithString(string_view_t *, string_view_t *prefixToSearch);
bool StringView_endsWithCString(string_view_t *, string_view_t *postfixToSearch);
bool StringView_startsWithCString(string_view_t *, string_view_t *prefixToSearch);
string_view_t StringView_find(string_view_t *, string_view_t *substring);
bool StringView_contains(string_view_t *, string_view_t *substring);
string_view_t StringView_substring(string_view_t *, unsigned int start, unsigned int count);
void StringView_trimLeft(string_view_t *);
void StringView_trimRight(string_view_t *);
void StringView_trim(string_view_t *);
/**
 * Compares the two given string views.
 * Returns 0 if they are equal.
 */
int StringView_compare(string_view_t *, string_view_t *);

/**
 * These two macros help with printing these strings out with printf.
 * Example usage:
 * ```c
 * printf("Printing > "STRING_FORMAT"\n", STRING_ARGS(str));
 * ```
 * Can accept both string_t and string_view_t.
 */
#define STRING_FORMAT "%.*s"
#define STRING_ARGS(STRING) STRING.length, STRING.data + STRING.start

#define StringView_endsWith(STRING, POSTFIX) _Generic((POSTFIX), \
    string_view_t: StringView_endsWithView,                      \
    string_t: StringView_endsWithString,                         \
    const char *: StringView_endsWithCString                     \
)(STRING, POSTFIX)
