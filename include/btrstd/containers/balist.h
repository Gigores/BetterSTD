#pragma once

#include "stddef.h"
#include "stdbool.h"

// Borrowing Array List

typedef struct {
    void **data;
    size_t count;
    size_t capacity;
} btr_balist_t;

// Creates a new borrowing array list from an array of given values.
// Example:
// ```c
// int a = 20;
// int b = 26;
// int c = 43;
// btr_balist_t list = BTR_BAList_make({&a, &b, &c}, 3);
// ```
btr_balist_t BTR_BAList_make(void *items[], size_t itemCount);
// Creates a new empty Borrowing Array List.
// Might return a zero-initialized list if allocation goes wrong.
btr_balist_t BTR_BAList_makeEmpty(size_t capacity);
// Creates a new borrowing array list from another borrowing array list with the same data.
btr_balist_t BTR_BAList_clone(const btr_balist_t *list);
// Appends the data to the end of the borrowing array list.
void BTR_BAList_append(btr_balist_t *, void *data);
// Prepends the data to the beginning of the borrowing array list.
void BTR_BAList_prepend(btr_balist_t *, void *data);
// Inserts an item into the borrowing array list so that it's index is the specified one.
void BTR_BAList_insert(btr_balist_t *, void *data, long index);
// Pops the data of the specified index from a borrowing array list and returns it.
// Can accept negative indexes.
void *BTR_BAList_pop(btr_balist_t *, long index);
// Returns the data of the specified index of the borrowing array list.
// Returns `NULL` if the index is invalid.
// Returns `NULL` if the pointer to the borrowing array list is invalid.
// Can accept negative indexes.
void *BTR_BAList_get(const btr_balist_t *, long index);
// Returns the first item of the borrowing array list.
// Returns `NULL` if the list is empty.
// Returns `NULL` if the pointer to the borrowing array list is invalid.
void *BTR_BAList_first(const btr_balist_t *);
// Returns the last item of the borrowing array list.
// Returns `NULL` if the list is empty.
// Returns `NULL` if the pointer to the borrowing array list is invalid.
void *BTR_BAList_last(const btr_balist_t *);
// Returns the index of the first occurance of the item.
// Returns -1 if the item wasn't found.
// Returns -1 if the pointer to the borrowing array list is invalid.
long BTR_BAList_indexOf(btr_balist_t *list, void *value, bool (*cmp)(const void *, const void *));
size_t BTR_BAList_len(const btr_balist_t *);
bool BTR_BAList_isEmpty(const btr_balist_t *);
void BTR_BAList_reverse(btr_balist_t *);
// Deallocates the borrowing array list.
void BTR_BAList_free(btr_balist_t *);
void BTR_BAList_clear(btr_balist_t *);

// Example usage:
// ```c
// btr_balist_t list = {...};  // a list of strings
// BTR_BALIST_FOREACH(list, item)
//     printf("%s\n", (char *)item);
// ```
#define BTR_BALIST_FOREACH(LIST, i) \
    void *i; \
    for (size_t _i = 0; _i < (LIST)->count && ((i = (LIST)->data[_i]), 1); _i++)

// Example usage:
// ```c
// btr_balist_t list = {...};  // a list of strings
// BTR_BALIST_ENUMERATE(list, item, index)
//     printf("%zu: %s\n", index, (char *)item);
// ```
#define BTR_BALIST_ENUMERATE(LIST, i, n) \
    void *i; \
    for (size_t n = 0; n < (LIST)->count && ((i = (LIST)->data[n]), 1); n++)

// Example usage:
// ```c
// int a = 14, b = 43, c = 34;
// btr_balist_t list = BALIST(&a, &b, &c);
// ```
#define BTR_BALIST(...)                                    \
    BTR_BAList_make(                                       \
        (void *[]){ __VA_ARGS__ },                         \
        sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *) \
    )
