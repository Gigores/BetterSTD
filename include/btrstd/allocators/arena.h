#pragma once

#include "btrstd/allocator.h"


typedef struct {
    void *data;
    size_t capacity;
    size_t next;
    btr_allocator_t *allocator;
} btr_arena_t;

btr_arena_t BTR_Arena_make(size_t capacity, btr_allocator_t *parentAllocator);
btr_alloc_result_t BTR_Arena_allocate(btr_arena_t *, size_t size);
void BTR_Arena_destroy(btr_arena_t *);
void BTR_Arena_reset(btr_arena_t *this, size_t newCapacity);
btr_allocator_t BTR_Arena_getWrapper(btr_arena_t *);
