#include "stddef.h"

typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
} btr_string_t;

/**
 * Creates a string from an array of bytes.
 * Can return a zero-initialised structure if fails to allocate memory.
 */
btr_string_t BTR_String_new(const char *chars);
/**
 * Crops the given count of characters from the left of the string.
 * ""
 */
void BTR_String_cropLeft(btr_string_t *, unsigned int charCount);
int BTR_String_compare(btr_string_t *, btr_string_t *);
/**
 * Frees the memory of a string.
 */
void BTR_String_free(btr_string_t *);

#define BTR_STRING_FORMAT "%.*s"
#define BTR_STRING_ARGS(STRING) STRING.length, STRING.data + STRING.start
