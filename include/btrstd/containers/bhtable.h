#pragma once

#include "btrstd/containers/bllist.h"

// Borrowing Hash Table

typedef unsigned long long btr_hash_t;

btr_hash_t BTR_hashCString(const void *);
bool BTR_compareCString(const void *, const void *);

typedef struct {
    void *key, *value;
} btr_key_value_t;

typedef struct {
    btr_bllist_t *data;
    size_t capacity;

    btr_hash_t (*hash)(const void *);
    bool (*compare)(const void *, const void *);

    btr_allocator_t *allocator;
} btr_bhtable_t;

btr_bhtable_t BTR_BHTable_make(
    btr_hash_t (*hash)(const void *),
    bool (*compare)(const void *, const void *),
    btr_allocator_t *allocator
);
void BTR_BHTable_put(btr_bhtable_t *, const void *key, const void *value);
btr_container_ptr_result_t BTR_BHTable_get(btr_bhtable_t *, const void *key);
void BTR_BHTable_free(btr_bhtable_t *);
