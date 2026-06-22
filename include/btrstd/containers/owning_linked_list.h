#pragma once

#include "btrstd/allocator.h"
#include "btrstd/containers/_generic_container_data.h"

typedef struct BTR_OLListNode {
    void *payload;
    struct BTR_OLListNode *next;
} btr_ollist_node_s;

typedef struct BTR_OLList {
    btr_ollist_node_s *head;
    btr_allocator_s *allocator;
    size_t size;
    size_t itemSize;
} btr_ollist_s;

btr_ollist_s BTR_OLList_makeFrom(
    void *items,
    size_t itemCount,
    size_t itemSize,
    btr_allocator_s *allocator
);
btr_ollist_s BTR_OLList_make(
    size_t capacity,
    size_t itemSize,
    btr_allocator_s *allocator
);
btr_ollist_s BTR_OLList_clone(const btr_ollist_s *list, btr_allocator_s *allocator);
void *BTR_OLList_append(btr_ollist_s *);
void *BTR_OLList_prepend(btr_ollist_s *);
void *BTR_OLList_insert(btr_ollist_s *, long index);
void BTR_OLList_pop(btr_ollist_s *, long index, void *buffer);
btr_container_ptr_r BTR_OLList_get(const btr_ollist_s *, long index);
btr_container_ptr_r BTR_OLList_first(const btr_ollist_s *);
btr_container_ptr_r BTR_OLList_last(const btr_ollist_s *);
btr_container_idx_r BTR_OLList_indexOf
    (btr_ollist_s *list, void *value, bool (*cmp)(const void *, const void *));
size_t BTR_OLList_len(const btr_ollist_s *);
bool BTR_OLList_isEmpty(const btr_ollist_s *);
void BTR_OLList_reverse(btr_ollist_s *);
void BTR_OLList_free(btr_ollist_s *);
void BTR_OLList_clear(btr_ollist_s *);
