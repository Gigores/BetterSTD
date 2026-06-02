#include "stddef.h"

typedef struct {
    char *data;
    unsigned int start;
    unsigned int length;
    unsigned int capacity;
} btr_string_t;

btr_string_t BTR_String_new(const char *chars);
void BTR_String_free(btr_string_t *);
