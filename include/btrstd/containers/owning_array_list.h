#pragma once

#include "stddef.h"

#include "btrstd/containers/_generic_container_data.h"
#include "btrstd/allocator.h"

typedef struct BTR_OAList {
    void *data;
    size_t capacity;
    size_t count;
    size_t itemSize;
    btr_allocator_s *allocator;
} btr_oalist_s;

// Creates a new owning array list from an array of given values.
// Example:
// ```c
// btr_oalist_t list = BTR_OAList_make((int *){20, 26, 43}, 3, sizeof(int), NULL);
// ```
btr_oalist_s BTR_OAList_makeFrom(
    void *items,
    size_t itemCount,
    size_t itemSize,
    btr_allocator_s *allocator
);
// Creates a new empty owning array list.
btr_oalist_s BTR_OAList_make(
    size_t capacity,
    size_t itemSize,
    btr_allocator_s *allocator
);
btr_oalist_s BTR_OAList_clone(const btr_oalist_s *list, btr_allocator_s *allocator);
// Appends the data to the end of the owning linked list.
// Returns a pointer to the new item, contents of which can be modified.
void *BTR_OAList_append(btr_oalist_s *);
// Appends the data to the beginning of the owning linked list.
// Returns a pointer to the new item, contents of which can be modified.
void *BTR_OAList_prepend(btr_oalist_s *);
// Inserts an item so that its index is the specified one.
// Returns a pointer to the new item, contents of which can be modified.
void *BTR_OAList_insert(btr_oalist_s *, long index);
void BTR_OAList_pop(btr_oalist_s *, long index, void *buffer);
btr_container_ptr_r BTR_OAList_get(const btr_oalist_s *, long index);
btr_container_ptr_r BTR_OAList_first(const btr_oalist_s *);
btr_container_ptr_r BTR_OAList_last(const btr_oalist_s *);
btr_container_idx_r BTR_OAList_indexOf
    (btr_oalist_s *list, void *value, bool (*cmp)(const void *, const void *));
size_t BTR_OAList_len(const btr_oalist_s *);
bool BTR_OAList_isEmpty(const btr_oalist_s *);
void BTR_OAList_reverse(btr_oalist_s *);
// Deallocates the owning array list.
void BTR_OAList_free(btr_oalist_s *);
// Deallocates the owning array list and initializes it again with as empty.
void BTR_OAList_clear(btr_oalist_s *);

#define BTR_OALIST_FOREACH(LIST, i) \
    void *i; \
    for (size_t _i = 0; _i < (LIST)->count && ((i = (LIST)->data[_i * (LIST)->itemSize]), 1); _i++)

#define BTR_OALIST_ENUMERATE(LIST, i, n) \
    void *i; \
    for (size_t n = 0; n < (LIST)->count && ((i = (LIST)->data[n * (LIST)->itemSize]), 1); n++)

#define BTR_OALIST_OF(T) BTR_OAList_make(8, sizeof(T), NULL)

#define BTR_OALIST(T, ...)                        \
    BTR_OAList_makeFrom(                          \
        (T[]){ __VA_ARGS__ },                     \
        sizeof((T[]){ __VA_ARGS__ }) / sizeof(T), \
        sizeof(T),                                \
        NULL                                      \
    )
