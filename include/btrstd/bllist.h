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

btr_bllist_t BTR_BLList_new(void *items[], size_t count);
btr_bllist_t BTR_BLList_clone(const btr_bllist_t *list);

void BTR_BLList_append(btr_bllist_t *, void *data);
void BTR_BLList_prepend(btr_bllist_t *, void *data);
void BTR_BLList_insert(btr_bllist_t *, void *data, long index);

void *BTR_BLList_pop(btr_bllist_t *, long index);

void *BTR_BLList_get(const btr_bllist_t *, long index);
void *BTR_BLList_first(const btr_bllist_t *);
void *BTR_BLList_last(const btr_bllist_t *);

long BTR_BLList_indexOf(btr_bllist_t *list, void *value, int (*cmp)(const void *, const void *));

size_t BTR_BLList_len(const btr_bllist_t *);
bool BTR_BLList_isEmpty(const btr_bllist_t *);
void BTR_BLList_reverse(btr_bllist_t *);

void BTR_BLList_free(btr_bllist_t *);
void BTR_BLList_clear(btr_bllist_t *);

#define BTR_BLLIST_FOREACH(LIST, VAR) for (btr_bllist_node_t *VAR = (LIST)->head; VAR != NULL; VAR = VAR->next)
#define BTR_BLLIST_ENUMERATE(LIST, NODE, i) size_t i = 0; for(btr_bllist_node_t *NODE = (LIST).head; NODE != NULL; NODE = NODE->next, i++)

#define BTR_BLLIST(...) \
    BTR_BLList_new( \
        (void *[]){ __VA_ARGS__ }, \
        sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *) \
    )
