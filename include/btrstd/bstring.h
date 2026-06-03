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
int String_compare(string_t *, string_t *);
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
int StringView_compare(string_view_t *, string_view_t *);

#define STRING_FORMAT "%.*s"
#define STRING_ARGS(STRING) STRING.length, STRING.data + STRING.start
