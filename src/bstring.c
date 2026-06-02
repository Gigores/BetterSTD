#include "btrstd/bstring.h"
#include "string.h"
#include "stdlib.h"

btr_string_t BTR_String_new(const char *chars)
{
    char *newChars = malloc(strlen(chars));
    if (!newChars) return (btr_string_t) {0};
    strncpy(newChars, chars, strlen(chars));
    btr_string_t string = {
        .data = newChars,
        .start = 0,
        .length = strlen(chars),
        .capacity = strlen(chars),
    };
    return string;
}
void BTR_String_free(btr_string_t *string)
{
    free(string->data);
}
