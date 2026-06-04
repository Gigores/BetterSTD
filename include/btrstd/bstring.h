#include "stddef.h"
#include "stdbool.h"

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
