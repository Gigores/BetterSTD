#include "stddef.h"
#include "stdbool.h"

// Owns the data
typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
} btr_string_t;

// Doesn't own the data
typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
} btr_string_view_t;

/**
 * Creates a string from an array of bytes.
 * Can return a zero-initialised structure if fails to allocate memory.
 */
btr_string_t BTR_String_clone(const char *chars);
/**
 * Crops the given count of characters from the left of the string.
 * "გვიპყრობდა" -> crop 3 -> "პყრობდა"
 */
void BTR_String_cropLeft(btr_string_t *, unsigned int charCount);
int BTR_String_compare(btr_string_t *, btr_string_t *);
int BTR_String_compareWithView(btr_string_t *, btr_string_view_t *);
/**
 * Frees the memory of a string.
 */
void BTR_String_free(btr_string_t *);

/**
 * Creates a string view from an array of bytes.
 * The string in question will not own the given data.
 */
btr_string_view_t BTR_StringView_fromCString(const char *chars);
/**
 * Crops the given count of characters from the left of the string view.
 * "გვიპყრობდა" -> crop 3 -> "პყრობდა"
 */
void BTR_StringView_cropLeft(btr_string_view_t *, unsigned int charCount);
int BTR_StringView_compare(btr_string_view_t *, btr_string_view_t *);

#define BTR_STRING_FORMAT "%.*s"
#define BTR_STRING_ARGS(STRING) STRING.length, STRING.data + STRING.start
