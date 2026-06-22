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
// Creates a new owning array list from the given one.
btr_oalist_s BTR_OAList_clone(const btr_oalist_s *list, btr_allocator_s *allocator);
// Appends the data to the end of the owning array list.
// Returns a pointer to the new item, contents of which can be modified.
void *BTR_OAList_append(btr_oalist_s *);
// Appends the data to the beginning of the owning array list.
// Returns a pointer to the new item, contents of which can be modified.
void *BTR_OAList_prepend(btr_oalist_s *);
// Inserts an item so that its index is the specified one.
// Returns a pointer to the new item, contents of which can be modified.
// Panics if the index is invalid.
void *BTR_OAList_insert(btr_oalist_s *, long index);
// Removes an item from a list and copies it into the buffer.
// The buffer can be NULL. In this case, the data will not be copied.
// Panics if the index is invalid.
void BTR_OAList_pop(btr_oalist_s *, long index, void *buffer);
// Returns a pointer to an item of the given index in the owning array list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_r BTR_OAList_get(const btr_oalist_s *, long index);
// Returns a pointer to the first item in the owning array list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_r BTR_OAList_first(const btr_oalist_s *);
// Returns a pointer to the last item in the owning array list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_r BTR_OAList_last(const btr_oalist_s *);
// Returns the index of the first occurrence of the item.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_NOT_FOUND` if the item wasn't found.
btr_container_idx_r BTR_OAList_indexOf
    (btr_oalist_s *list, void *value, bool (*cmp)(const void *, const void *));
// Returns the amount of items in the owning array list.
size_t BTR_OAList_len(const btr_oalist_s *);
// Tells if the given owning array list is empty.
bool BTR_OAList_isEmpty(const btr_oalist_s *);
// Reverses the order of items in the borrowing array list.
void BTR_OAList_reverse(btr_oalist_s *);
// Deallocates the owning array list.
void BTR_OAList_free(btr_oalist_s *);
// Deallocates the owning array list and initializes it again with as empty.
void BTR_OAList_clear(btr_oalist_s *);

#define BTR_OALIST_FOREACH(LIST, i) \
    void *i; \
    for (size_t _i = 0; _i < (LIST)->count && ((i = (LIST)->data + _i * (LIST)->itemSize), 1); _i++)

#define BTR_OALIST_ENUMERATE(LIST, i, n) \
    void *i; \
    for (size_t n = 0; n < (LIST)->count && ((i = (LIST)->data + n * (LIST)->itemSize), 1); n++)

#define BTR_OALIST_OF(T) BTR_OAList_make(8, sizeof(T), NULL)

#define BTR_OALIST(T, ...)                        \
    BTR_OAList_makeFrom(                          \
        (T[]){ __VA_ARGS__ },                     \
        sizeof((T[]){ __VA_ARGS__ }) / sizeof(T), \
        sizeof(T),                                \
        NULL                                      \
    )

#ifdef BTR_NO_PREFIX

#define OAList BTR_OAList

typedef btr_oalist_s oalist_s;

#define OAList_makeFrom BTR_OAList_makeFrom
#define OAList_make     BTR_OAList_make
#define OAList_clone    BTR_OAList_clone
#define OAList_append   BTR_OAList_append
#define OAList_prepend  BTR_OAList_prepend
#define OAList_insert   BTR_OAList_insert
#define OAList_pop      BTR_OAList_pop
#define OAList_get      BTR_OAList_get
#define OAList_first    BTR_OAList_first
#define OAList_last     BTR_OAList_last
#define OAList_indexOf  BTR_OAList_indexOf
#define OAList_len      BTR_OAList_len
#define OAList_isEmpty  BTR_OAList_isEmpty
#define OAList_reverse  BTR_OAList_reverse
#define OAList_free     BTR_OAList_free
#define OAList_clear    BTR_OAList_clear

#define OALIST_FOREACH   BTR_OALIST_FOREACH
#define OALIST_ENUMERATE BTR_OALIST_ENUMERATE
#define OALIST_OF        BTR_OALIST_OF
#define OALIST           BTR_OALIST

#endif  // BTR_NO_PREFIX
