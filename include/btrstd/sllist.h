#pragma once

#include "stdlib.h"

#include "bool.h"

typedef struct BTR_SLListNode {
    void *payload;
    struct BTR_SLListNode *next;
} btr_sllist_node_t;

typedef struct {
    btr_sllist_node_t *head;
    size_t size;
} btr_sllist_t;

btr_sllist_t BTR_SLList_new(void *items[], size_t count);
btr_sllist_t BTR_SLList_clone(const btr_sllist_t *list);

void BTR_SLList_append(btr_sllist_t *, void *data);
void BTR_SLList_prepend(btr_sllist_t *, void *data);
void BTR_SLList_insert(btr_sllist_t *, void *data, long index);

void *BTR_SLList_pop(btr_sllist_t *, long index);

void *BTR_SLList_get(const btr_sllist_t *, long index);
void *BTR_SLList_first(const btr_sllist_t *);
void *BTR_SLList_last(const btr_sllist_t *);

long BTR_SLList_indexOf(btr_sllist_t *list, void *value, int (*cmp)(const void *, const void *));

size_t BTR_SLList_len(const btr_sllist_t *);
bool BTR_SLList_isEmpty(const btr_sllist_t *);
void BTR_SLList_reverse(btr_sllist_t *);

void BTR_SLList_free(btr_sllist_t *);
void BTR_SLList_clear(btr_sllist_t *);

#define BTR_SLLIST_FOREACH(LIST, VAR) for (btr_sllist_node_t *VAR = (LIST)->head; VAR != NULL; VAR = VAR->next)
#define BTR_SLLIST_ENUMERATE(LIST, NODE, i) size_t i = 0; for(btr_sllist_node_t *NODE = (LIST).head; NODE != NULL; NODE = NODE->next, i++)

#define BTR_SLLIST(...) \
    BTR_SLList_new( \
        (void *[]){ __VA_ARGS__ }, \
        sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *) \
    )
