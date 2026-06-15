#pragma once

#include "stddef.h"
#include "stdbool.h"

#include "btrstd/containers/generic_container_data.h"
#include "btrstd/allocator.h"

// Borrowing Linked List

// Linked list is a data structure that has an array-like interface, but allows dynamic sizing.
// It uses a "node" structure to represent a specific index. The node itself stores not only its
// data, but also a pointer to the next node.
//     list{size, ptr}
//                 ↓
//               node{data, ptr}
//                           ↓
//                         node{data, ptr}
//                                     ↓
//                                    NULL

// Allocator required capabilities:
//   allocate, deallocate

typedef struct BLListNode {
    void *payload;
    struct BLListNode *next;
} btr_bllist_node_t;

typedef struct {
    btr_bllist_node_t *head;
    btr_allocator_t *allocator;
    size_t size;
} btr_bllist_t;

// Creates a new borrowing linked list from an array of given values.
// Example:
// ```c
// int a = 20;
// int b = 26;
// int c = 43;
// btr_bllist_t list = BTR_BLList_make({&a, &b, &c}, 3);
// ```
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_bllist_t BTR_BLList_makeFrom(void *items[], size_t itemCount, btr_allocator_t *allocator);
// Creates an empty Borrowing Linked List.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_bllist_t BTR_BLList_make(btr_allocator_t *allocator);
// Creates a new borrowing linked list from another borrowing linked list with the same data.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_bllist_t BTR_BLList_clone(const btr_bllist_t *list, btr_allocator_t *allocator);
// Appends the data to the end of the borrowing linked list.
void BTR_BLList_append(btr_bllist_t *, void *data);
// Prepends the data to the beginning of the borrowing linked list.
void BTR_BLList_prepend(btr_bllist_t *, void *data);
// Inserts an item so that its index is the specified one.
void BTR_BLList_insert(btr_bllist_t *, void *data, long index);
// Pops the data of the specified index from a borrowing linked list.
// Can accept negative indexes.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the index is invalid.
btr_container_ptr_result_t BTR_BLList_pop(btr_bllist_t *, long index);
// Returns the data of the specified index of the borrowing linked list.
// Can accept negative indexes.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the index is invalid.
btr_container_ptr_result_t BTR_BLList_get(const btr_bllist_t *, long index);
// Sets the item at the given index to a given value in the borrowing linked list.
// Can accept negative indexes.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
void BTR_BLList_set(btr_bllist_t *, void *data, long index);
// Returns the first item of the borrowing linked list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_result_t BTR_BLList_first(const btr_bllist_t *);
// Returns the last item of the borrowing linked list.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the list is empty.
btr_container_ptr_result_t BTR_BLList_last(const btr_bllist_t *);
// Returns the index of the first occurrence of the item.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_NOT_FOUND` if the item wasn't found.
btr_container_idx_result_t BTR_BLList_indexOf
    (btr_bllist_t *list, void *value, bool (*cmp)(const void *, const void *));
// Returns the amount of items in the borrowing linked list.
size_t BTR_BLList_len(const btr_bllist_t *);
// Tells if the borrowing linked list is empty.
bool BTR_BLList_isEmpty(const btr_bllist_t *);
// Reverses the order of items in the borrowing linked list.
void BTR_BLList_reverse(btr_bllist_t *);
// Deallocates the borrowing linked list.
void BTR_BLList_free(btr_bllist_t *);
// Exactly the same as `BLList_free`
void BTR_BLList_clear(btr_bllist_t *);
// Example usage:
// ```c
// btr_bllist_t list = {...};  // a list of strings
// BTR_BLLIST_FOREACH(list, item)
//     printf("%s\n", (char *)item);
// ```
#define BTR_BLLIST_FOREACH(LIST, i)           \
    void *i;                                  \
    for (                                     \
        btr_bllist_node_t *_n = (LIST)->head; \
        _n != NULL && ((i = _n->payload), 1); \
        _n = _n->next                         \
    )
// Example usage:
// ```c
// btr_bllist_t list = {...};  // a list of strings
// BTR_BLLIST_ENUMERATE(list, item, index)
//     printf("%zu: %s\n", index, (char *)item);
// ```
#define BTR_BLLIST_ENUMERATE(LIST, i, n)      \
    size_t n = 0;                             \
    void *i;                                  \
    for (                                     \
        btr_bllist_node_t *_i = (LIST)->head; \
        _i != NULL && ((i = _i->payload), 1); \
        _i = _i->next, n++                    \
    )

// Example usage:
// ```c
// int a = 14, b = 43, c = 34;
// btr_bllist_t list = BLLIST(&a, &b, &c);
// ```
#define BTR_BLLIST(...)                                     \
    BTR_BLList_makeFrom(                                    \
        (void *[]){ __VA_ARGS__ },                          \
        sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *), \
        NULL                                                \
    )

#ifdef BTR_NO_PREFIX

typedef btr_bllist_node_t bllist_node_t;
typedef btr_bllist_t bllist_t;

#define BLList_makeFrom BTR_BLList_makeFrom
#define BLList_make     BTR_BLList_make
#define BLList_clone    BTR_BLList_clone
#define BLList_append   BTR_BLList_append
#define BLList_prepend  BTR_BLList_prepend
#define BLList_insert   BTR_BLList_insert
#define BLList_pop      BTR_BLList_pop
#define BLList_get      BTR_BLList_get
#define BLList_first    BTR_BLList_first
#define BLList_last     BTR_BLList_last
#define BLList_indexOf  BTR_BLList_indexOf
#define BLList_len      BTR_BLList_len
#define BLList_isEmpty  BTR_BLList_isEmpty
#define BLList_reverse  BTR_BLList_reverse
#define BLList_free     BTR_BLList_free
#define BLList_clear    BTR_BLList_clear

#define BLLIST_FOREACH   BTR_BLLIST_FOREACH
#define BLLIST_ENUMERATE BTR_BLLIST_ENUMERATE
#define BLLIST           BTR_BLLIST

#endif  // BTR_NO_PREFIX
