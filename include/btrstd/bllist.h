#pragma once

#include "stddef.h"
#include "stdbool.h"

// The idea of the Borrowing Linked List is that it does not own the data,
// it only points to data that you own.
// So it is your responsibility to allocate and free the data.

typedef struct BLListNode {
    void *payload;
    struct BLListNode *next;
} btr_bllist_node_t;

typedef struct {
    btr_bllist_node_t *head;
    size_t size;
} btr_bllist_t;

// Creates a new borrowing linked list from an array of given values.
// Example:
// ```c
// int a = 20;
// int b = 26;
// int c = 43;
// bllist_t list = BLList_make({&a, &b, &c}, 3);
// ```
btr_bllist_t BTR_BLList_make(void *items[], size_t itemCount);
// Creates a new borrowing linked list from another borrowing linked list with the same data.
btr_bllist_t BTR_BLList_clone(const btr_bllist_t *list);
// Appends the data to the end of the borrowing linked list.
void BTR_BLList_append(btr_bllist_t *, void *data);
// Prepends the data to the beginning of the borrowing linked list.
void BTR_BLList_prepend(btr_bllist_t *, void *data);
// Inserts an item so that it's index is the specified one.
void BTR_BLList_insert(btr_bllist_t *, void *data, long index);
// Pops the data of the specified index from a borrowing linked list and returns it.
// Can accept negative indexes.
void *BTR_BLList_pop(btr_bllist_t *, long index);
// Returns the data of the specified index of the borrowing linked list.
// Returns `NULL` if the index is invalid.
// Returns `NULL` if the pointer to the borrowing linked list is invalid.
// Can accept negative indexes.
void *BTR_BLList_get(const btr_bllist_t *, long index);
// Returns the first item of the borrowing linked list.
// Returns `NULL` if the list is empty.
// Returns `NULL` if the pointer to the borrowing linked list is invalid.
void *BTR_BLList_first(const btr_bllist_t *);
// Returns the last item of the borrowing linked list.
// Returns `NULL` if the list is empty.
// Returns `NULL` if the pointer to the borrowing linked list is invalid.
void *BTR_BLList_last(const btr_bllist_t *);
// Returns the index of the first occurance of the item.
// Returns -1 if the item wasn't found.
// Returns -1 if the pointer to the borrowing linked list is invalid.
long BTR_BLList_indexOf(btr_bllist_t *list, void *value, bool (*cmp)(const void *, const void *));
// Returns the amount of items in the borrowing linked list.
// Returns -1 if the pointer to the borrowing linked list is invalid.
size_t BTR_BLList_len(const btr_bllist_t *);
// Tells if the borrowing linked list is empty.
// Returns -1 if the pointer to the borrowing linked list is invalid.
bool BTR_BLList_isEmpty(const btr_bllist_t *);
// Reverses the order of items in the borrowing linked list.
// Returns `false` if the pointer to the borrowing linked list is invalid.
void BTR_BLList_reverse(btr_bllist_t *);
// Deallocates the borrowing linked list.
void BTR_BLList_free(btr_bllist_t *);
// The exact same as `BLList_free`
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
#define BTR_BLLIST(...)                                    \
    BTR_BLList_make(                                       \
        (void *[]){ __VA_ARGS__ },                         \
        sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *) \
    )

#ifdef BTR_NO_PREFIX

typedef bllist_node_t btr_bllist_node_t
typedef bllist_t btr_bllist_t

#define BLList_make    BTR_BLList_make
#define BLList_clone   BTR_BLList_clone
#define BLList_append  BTR_BLList_append
#define BLList_prepend BTR_BLList_prepend
#define BLList_insert  BTR_BLList_insert
#define BLList_pop     BTR_BLList_pop
#define BLList_get     BTR_BLList_get
#define BLList_first   BTR_BLList_first
#define BLList_last    BTR_BLList_last
#define BLList_indexOf BTR_BLList_indexOf
#define BLList_len     BTR_BLList_len
#define BLList_isEmpty BTR_BLList_isEmpty
#define BLList_reverse BTR_BLList_reverse
#define BLList_free    BTR_BLList_free
#define BLList_clear   BTR_BLList_clear

#define BLLIST_FOREACH   BTR_BLLIST_FOREACH
#define BLLIST_ENUMERATE BTR_BLLIST_ENUMERATE
#define BLLIST           BTR_BLLIST

#endif  // BTR_NO_PREFIX
