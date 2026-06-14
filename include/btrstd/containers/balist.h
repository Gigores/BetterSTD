#pragma once

#include "stddef.h"
#include "stdbool.h"

#include "btrstd/containers/generic_container_data.h"
#include "btrstd/allocator.h"

// Borrowing Array List

// Array list, also known as "dynamic array", is an array that automatically scales up or down
// when needed.

// Allocator required capabilities:
//   allocate, deallocate

typedef struct {
    void **data;
    size_t count;
    size_t capacity;
    btr_allocator_t *allocator;
} btr_balist_t;

// Creates a new borrowing array list from an array of given values.
// Example:
// ```c
// int a = 20;
// int b = 26;
// int c = 43;
// btr_balist_t list = BTR_BAList_make({&a, &b, &c}, 3);
// ```
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_balist_t BTR_BAList_makeFrom(void *items[], size_t itemCount, btr_allocator_t *allocator);
// Creates a new empty Borrowing Array List.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_balist_t BTR_BAList_make(size_t capacity, btr_allocator_t *allocator);
// Creates a new borrowing array list from another borrowing array list with the same data.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_balist_t BTR_BAList_clone(const btr_balist_t *list, btr_allocator_t *allocator);
// Appends the data to the end of the borrowing array list.
void BTR_BAList_append(btr_balist_t *, void *data);
// Prepends the data to the beginning of the borrowing array list.
void BTR_BAList_prepend(btr_balist_t *, void *data);
// Inserts an item into the borrowing array list so that its index is the specified one.
void BTR_BAList_insert(btr_balist_t *, void *data, long index);
// Pops the data of the specified index from a borrowing array list.
// Can accept negative indexes.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the index is invalid.
btr_container_ptr_result_t BTR_BAList_pop(btr_balist_t *, long index);
// Returns the data of the specified index of the borrowing array list.
// Can accept negative indexes.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the index is invalid.
btr_container_ptr_result_t BTR_BAList_get(const btr_balist_t *, long index);
// Returns the first item of the borrowing array list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_result_t BTR_BAList_first(const btr_balist_t *);
// Returns the last item of the borrowing array list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_result_t BTR_BAList_last(const btr_balist_t *);
// Returns the index of the first occurrence of the item.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_NOT_FOUND` if the item wasn't found.
btr_container_idx_result_t BTR_BAList_indexOf(btr_balist_t *list, void *value, bool (*cmp)(const void *, const void *));
// Returns the amount of items in the borrowing array list.
size_t BTR_BAList_len(const btr_balist_t *);
// Tells if the borrowing array list is empty.
bool BTR_BAList_isEmpty(const btr_balist_t *);
// Reverses the order of items in the borrowing array list.
void BTR_BAList_reverse(btr_balist_t *);
// Deallocates the borrowing array list.
void BTR_BAList_free(btr_balist_t *);
// Exactly the same as `BAList_free`
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
#define BTR_BALIST(...)                                     \
    BTR_BAList_makeFrom(                                    \
        (void *[]){ __VA_ARGS__ },                          \
        sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *), \
        NULL                                                \
    )

#ifdef BTR_NO_PREFIX

typedef btr_balist_t balist_t;

#define BAList_make      BTR_BAList_make
#define BAList_makeFrom  BTR_BAList_makeFrom
#define BAList_clone     BTR_BAList_clone
#define BAList_append    BTR_BAList_append
#define BAList_prepend   BTR_BAList_prepend
#define BAList_insert    BTR_BAList_insert
#define BAList_pop       BTR_BAList_pop
#define BAList_get       BTR_BAList_get
#define BAList_first     BTR_BAList_first
#define BAList_last      BTR_BAList_last
#define BAList_indexOf   BTR_BAList_indexOf
#define BAList_len       BTR_BAList_len
#define BAList_isEmpty   BTR_BAList_isEmpty
#define BAList_reverse   BTR_BAList_reverse
#define BAList_free      BTR_BAList_free
#define BAList_clear     BTR_BAList_clear

#define BALIST_FOREACH   BTR_BALIST_FOREACH
#define BALIST_ENUMERATE BTR_BALIST_ENUMERATE
#define BALIST           BTR_BALIST

#endif  // BTR_NO_PREFIX
