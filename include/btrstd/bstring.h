#include "stddef.h"
#include "stdbool.h"

typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
    bool owns;
} btr_string_t;

/**
 * Creates a string from an array of bytes.
 * The string in question will not own the given data.
 */
btr_string_t BTR_String_view(const char *chars);
/**
 * Creates a string from an array of bytes.
 * The string in question will own the given data, and should be freed later.
 * Can return a zero-initialised structure if fails to allocate memory.
 */
btr_string_t BTR_String_clone(const char *chars);
/**
 * Crops the given count of characters from the left of the string.
 * "გვიპყრობდა" -> crop 3 -> "პყრობდა"
 */
void BTR_String_cropLeft(btr_string_t *, unsigned int charCount);
int BTR_String_compare(btr_string_t *, btr_string_t *);
/**
 * Frees the memory of a string.
 */
void BTR_String_free(btr_string_t *);

#define BTR_STRING_FORMAT "%.*s"
#define BTR_STRING_ARGS(STRING) STRING.length, STRING.data + STRING.start
