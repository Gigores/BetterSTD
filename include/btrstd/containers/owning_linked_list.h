#pragma once

#include "btrstd/allocator.h"
#include "btrstd/containers/borrowing_linked_list.h"
#include "btrstd/containers/_generic_container_data.h"

typedef struct BTR_OLList {
    btr_bllist_s underlying;
    btr_allocator_s *allocator;
    size_t itemSize;
} btr_ollist_s;

// Creates a new owning linked list from an array of given values.
// Example:
// ```c
// btr_ollist_t list = BTR_OLList_make((int *){20, 26, 43}, 3, sizeof(int), NULL);
// ```
btr_ollist_s BTR_OLList_makeFrom(
    void *items,
    size_t itemCount,
    size_t itemSize,
    btr_allocator_s *allocator
);
// Creates a new empty owning linked list.
btr_ollist_s BTR_OLList_make(
    size_t itemSize,
    btr_allocator_s *allocator
);
// Creates a new owning linked list from the given one.
btr_ollist_s BTR_OLList_clone(const btr_ollist_s *list, btr_allocator_s *allocator);
// Appends the data to the end of the owning linked list.
// Returns a pointer to the new item, contents of which can be modified.
void *BTR_OLList_append(btr_ollist_s *);
// Appends the data to the beginning of the owning linked list.
// Returns a pointer to the new item, contents of which can be modified.
void *BTR_OLList_prepend(btr_ollist_s *);
// Inserts an item so that its index is the specified one.
// Returns a pointer to the new item, contents of which can be modified.
// Panics if the index is invalid.
void *BTR_OLList_insert(btr_ollist_s *, long index);
// Removes an item from a list and copies it into the buffer.
// The buffer can be NULL. In this case, the data will not be copied.
// Panics if the index is invalid.
void BTR_OLList_pop(btr_ollist_s *, long index, void *buffer);
// Returns a pointer to an item of the given index in the owning linked list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_r BTR_OLList_get(const btr_ollist_s *, long index);
// Returns a pointer to the first item in the owning linked list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_r BTR_OLList_first(const btr_ollist_s *);
// Returns a pointer to the last item in the owning linked list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_r BTR_OLList_last(const btr_ollist_s *);
// Returns the index of the first occurrence of the item.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_NOT_FOUND` if the item wasn't found.
btr_container_idx_r BTR_OLList_indexOf
    (btr_ollist_s *list, void *value, bool (*cmp)(const void *, const void *));
// Returns the amount of items in the owning linked list.
size_t BTR_OLList_len(const btr_ollist_s *);
// Tells if the given owning linked list is empty.
bool BTR_OLList_isEmpty(const btr_ollist_s *);
// Reverses the order of items in the borrowing linked list.
void BTR_OLList_reverse(btr_ollist_s *);
// Deallocates the owning linked list.
void BTR_OLList_free(btr_ollist_s *);
// Exactly the same as `OLList_free`
void BTR_OLList_clear(btr_ollist_s *);
// Creates an array with the same contents as the given owning
// linked list.
// The returned array must be freed.
// The `allocator` parameter can be set as `NULL`, in this case it will use the same allocator which the container uses.
void *BTR_OLList_toArray(btr_ollist_s *, btr_allocator_s *allocator);

// Example usage:
// ```c
// btr_ollist_t list = {...};  // a list of strings
// BTR_OLLIST_FOREACH(list, item)
//     printf("%s\n", (char *)item);
// ```
#define BTR_OLLIST_FOREACH(LIST, i) BTR_BLLIST_FOREACH(&(LIST)->underlying, i)
// Example usage:
// ```c
// btr_ollist_t list = {...};  // a list of strings
// BTR_OLLIST_ENUMERATE(list, item, index)
//     printf("%zu: %s\n", index, (char *)item);
// ```
#define BTR_OLLIST_ENUMERATE(LIST, i, n) BTR_BLLIST_ENUMERATE(&(LIST)->underlying, i, n)

#define BTR_OLLIST_OF(T) BTR_OLList_make(sizeof(T), NULL)

#define BTR_OLLIST(T, ...)                        \
    BTR_OLList_makeFrom(                          \
        (T[]){ __VA_ARGS__ },                     \
        sizeof((T[]){ __VA_ARGS__ }) / sizeof(T), \
        sizeof(T),                                \
        NULL                                      \
    )

#ifdef BTR_NO_PREFIX

#define OLList BTR_OLList

typedef btr_ollist_s ollist_s;

#define OLList_makeFrom BTR_OLList_makeFrom
#define OLList_make     BTR_OLList_make
#define OLList_clone    BTR_OLList_clone
#define OLList_append   BTR_OLList_append
#define OLList_prepend  BTR_OLList_prepend
#define OLList_insert   BTR_OLList_insert
#define OLList_pop      BTR_OLList_pop
#define OLList_get      BTR_OLList_get
#define OLList_first    BTR_OLList_first
#define OLList_last     BTR_OLList_last
#define OLList_indexOf  BTR_OLList_indexOf
#define OLList_len      BTR_OLList_len
#define OLList_isEmpty  BTR_OLList_isEmpty
#define OLList_reverse  BTR_OLList_reverse
#define OLList_free     BTR_OLList_free
#define OLList_clear    BTR_OLList_clear

#define OLLIST_FOREACH   BTR_OLLIST_FOREACH
#define OLLIST_ENUMERATE BTR_OLLIST_ENUMERATE
#define OLLIST_OF        BTR_OLLIST_OF
#define OLLIST           BTR_OLLIST

#endif  // BTR_NO_PREFIX
