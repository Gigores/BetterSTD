#pragma once

#include "stddef.h"
#include "stdbool.h"

// The idea of the Borrowing Linked List is that it does not own the data,
// it only points to data that you own.
// So it is your responsibility to allocate and free the data.

typedef struct BLListNode {
    void *payload;
    struct BLListNode *next;
} bllist_node_t;

typedef struct {
    bllist_node_t *head;
    size_t size;
} bllist_t;

// Creates a new borrowing linked list from an array of given values.
// Example:
// ```c
// int a = 20;
// int b = 26;
// int c = 43;
// bllist_t list = BLList_make({&a, &b, &c}, 3);
// ```
bllist_t BLList_make(void *items[], size_t itemCount);
// Creates a new borrowing linked list from another borrowing linked list with the same data.
bllist_t BLList_clone(const bllist_t *list);
// Appends the data to the end of the borrowing linked list.
void BLList_append(bllist_t *, void *data);
// Prepends the data to the beginning of the borrowing linked list.
void BLList_prepend(bllist_t *, void *data);
// Inserts an item so that it's index is the specified one.
void BLList_insert(bllist_t *, void *data, long index);
// Pops the data of the specified index from a borrowing linked list and returns it.
// Can accept negative indexes.
void *BLList_pop(bllist_t *, long index);
// Returns the data of the specified index of the borrowing linked list.
// Returns `NULL` if the index is invalid.
// Returns `NULL` if the pointer to the borrowing linked list is invalid.
// Can accept negative indexes.
void *BLList_get(const bllist_t *, long index);
// Returns the first item of the borrowing linked list.
// Returns `NULL` if the list is empty.
// Returns `NULL` if the pointer to the borrowing linked list is invalid.
void *BLList_first(const bllist_t *);
// Returns the last item of the borrowing linked list.
// Returns `NULL` if the list is empty.
// Returns `NULL` if the pointer to the borrowing linked list is invalid.
void *BLList_last(const bllist_t *);
// Returns the index of the first occurance of the item.
// Returns -1 if the item wasn't found.
// Returns -1 if the pointer to the borrowing linked list is invalid.
long BLList_indexOf(bllist_t *list, void *value, bool (*cmp)(const void *, const void *));
// Returns the amount of items in the borrowing linked list.
// Returns -1 if the pointer to the borrowing linked list is invalid.
size_t BLList_len(const bllist_t *);
// Tells if the borrowing linked list is empty.
// Returns -1 if the pointer to the borrowing linked list is invalid.
bool BLList_isEmpty(const bllist_t *);
// Reverses the order of items in the borrowing linked list.
// Returns `false` if the pointer to the borrowing linked list is invalid.
void BLList_reverse(bllist_t *);
// Deallocates the borrowing linked list.
void BLList_free(bllist_t *);
// The exact same as `BLList_free`
void BLList_clear(bllist_t *);
// Example usage:
// ```c
// bllist_t list = {...};  // a list of strings
// BLLIST_FOREACH(list, item)
//     printf("%s\n", (char *)item);
// ```
#define BLLIST_FOREACH(LIST, i)           \
    void *i;                                  \
    for (                                     \
        bllist_node_t *_n = (LIST)->head; \
        _n != NULL && ((i = _n->payload), 1); \
        _n = _n->next                         \
    )
// Example usage:
// ```c
// bllist_t list = {...};  // a list of strings
// BLLIST_ENUMERATE(list, item, index)
//     printf("%zu: %s\n", index, (char *)item);
// ```
#define BLLIST_ENUMERATE(LIST, i, n)      \
    size_t n = 0;                             \
    void *i;                                  \
    for (                                     \
        bllist_node_t *_i = (LIST)->head; \
        _i != NULL && ((i = _i->payload), 1); \
        _i = _i->next, n++                    \
    )

// Example usage:
// ```c
// int a = 14, b = 43, c = 34;
// bre_bllist_t list = BLLIST(&a, &b, &c);
// ```
#define BLLIST(...)                                    \
    BLList_make(                                        \
        (void *[]){ __VA_ARGS__ },                         \
        sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *) \
    )
