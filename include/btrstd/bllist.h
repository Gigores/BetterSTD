#pragma once

#include "stdlib.h"

#include "bool.h"

typedef struct BTR_BLListNode {
    void *payload;
    struct BTR_BLListNode *next;
} btr_bllist_node_t;

typedef struct {
    btr_bllist_node_t *head;
    size_t size;
} btr_bllist_t;

/**
 * Creates a new borrowing linked list from an array of given values.
 * Example:
 * ```c
 * int a = 20;
 * int b = 26;
 * int c = 43;
 * btr_bllist_t list = BTR_BLList_new({&a, &b, &c}, 3);
 * ```
 */
btr_bllist_t BTR_BLList_new(void *items[], size_t itemCount);
/**
 * Creates a new borrowing linked list from another borrowing linked list with the same data.
 */
btr_bllist_t BTR_BLList_clone(const btr_bllist_t *list);
/**
 * Appends the data to the end of the borrowing linked list.
 */
void BTR_BLList_append(btr_bllist_t *, void *data);
/**
 * Prepends the data to the beginning of the borrowing linked list.
 */
void BTR_BLList_prepend(btr_bllist_t *, void *data);
/**
 * Inserts an item so that it's index is the specified one.
 */
void BTR_BLList_insert(btr_bllist_t *, void *data, long index);

/**
 * Pops the data of the specified index from a borrowing linked list and returns it.
 * Returns `NULL` if the index is invalid.
 * Can accept negative indexes.
 */
void *BTR_BLList_pop(btr_bllist_t *, long index);

/**
 * Returns the data of the specified index.
 * Returns `NULL` if the index is invalid.
 * Can accept negative indexes.
 */
void *BTR_BLList_get(const btr_bllist_t *, long index);
void *BTR_BLList_first(const btr_bllist_t *);
void *BTR_BLList_last(const btr_bllist_t *);

long BTR_BLList_indexOf(btr_bllist_t *list, void *value, int (*cmp)(const void *, const void *));

/**
 * Returns the amount of items in the borrowing linked list.
 */
size_t BTR_BLList_len(const btr_bllist_t *);
bool BTR_BLList_isEmpty(const btr_bllist_t *);
void BTR_BLList_reverse(btr_bllist_t *);

/**
 * Deallocates the borrowing linked list.
 */
void BTR_BLList_free(btr_bllist_t *);
void BTR_BLList_clear(btr_bllist_t *);

#define BTR_BLLIST_FOREACH(LIST, i)           \
    void *i;                                  \
    for (                                     \
        btr_bllist_node_t *_n = (LIST)->head; \
        _n != NULL && ((i = _n->payload), 1); \
        _n = _n->next                         \
    )
#define BTR_BLLIST_ENUMERATE(LIST, i, n)      \
    size_t n = 0;                             \
    void *i;                                  \
    for (                                     \
        btr_bllist_node_t *_i = (LIST)->head; \
        _i != NULL && ((i = _i->payload), 1); \
        _i = _i->next, n++                     \
    )

#define BTR_BLLIST(...) \
    BTR_BLList_new( \
        (void *[]){ __VA_ARGS__ }, \
        sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *) \
    )
