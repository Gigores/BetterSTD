#pragma once

#include "btrstd/allocator.h"
#include "btrstd/containers/bit_set.h"

#include "stdint.h"

typedef struct {
    btr_bit_set_t mask;
    btr_allocator_t *allocator;
    uint8_t *data;
    size_t itemSize;
    size_t itemCount;
} btr_pool_t;

btr_pool_t BTR_Pool_make(size_t itemSize, size_t itemCount, btr_allocator_t *parentAllocator);
btr_alloc_result_t BTR_Pool_allocate(btr_pool_t *);
void BTR_Pool_deallocate(btr_pool_t *, void *pointer);
void BTR_Pool_destroy(btr_pool_t *);
void BTR_Pool_reset(btr_pool_t *, size_t newItemCount);
