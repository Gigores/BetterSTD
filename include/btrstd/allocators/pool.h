#pragma once

#include "btrstd/allocator.h"
#include "btrstd/containers/bit_set.h"

#include "stdint.h"

// Pool Allocator

// Pool allocator is an allocator, that allocates an entire array of "blocks"
// and gives them away one-by-one. Unlike linear allocators, pool can actually
// free memory. To track free blocks, it uses a bit set.
//     BTR_Pool_make(sizeof(short), 8)
//       → 00000000000000000000000000000000
//         0   0   0   0   0   0   0   0
//     short *a = BTR_Pool_allocate(&pool)
//       → 00000000000000000000000000000000
//         1   0   0   0   0   0   0   0
//     short *b = BTR_Pool_allocate(&pool)
//       → 00000000000000000000000000000000
//         1   1   0   0   0   0   0   0
//     short *c = BTR_Pool_allocate(&pool)
//     short *d = BTR_Pool_allocate(&pool)
//     *a = 18
//     *b = 76
//     *c = 456
//     *d = 846
//       → 0012004C01C8034E0000000000000000
//         1   1   1   1   0   0   0   0
//     BTR_Pool_deallocate(&pool, a);
//       → 0012004C01C8034E0000000000000000
//         0   1   1   1   0   0   0   0
//     short *e = BTR_Pool_allocate(&pool)
//       → 0000004C01C8034E0000000000000000
//         1   1   1   1   0   0   0   0

// Pros: Fast and can free memory.
// Cons: Only can allocate a fixed amount of bytes.

// Allocator capabilities:
// - allocate:   OK/BTR_ALLOC_ERR_OUT_OF_MEMORY/BTR_ALLOC_ERR_INVALID_SIZE
// - reallocate: BTR_ALLOC_ERR_UNSUPPORTED_OPERATION
// - deallocate: OK/panic(invalid pointer)

// Parent allocator required capabilities:
//   allocate variadic_size, deallocate

typedef struct {
    btr_bit_set_t mask;
    btr_allocator_t *allocator;
    uint8_t *data;
    size_t itemSize;
    size_t itemCount;
} btr_pool_t;

// Creates an empty Pool.
// The `parentAllocator` parameter can be set as `NULL`, in this case it will use
// the global allocator.
btr_pool_t BTR_Pool_make(size_t itemSize, size_t itemCount, btr_allocator_t *parentAllocator);
// Allocates a block in the Pool.
// Can return BTR_ALLOC_ERR_OUT_OF_MEMORY.
btr_alloc_result_t BTR_Pool_allocate(btr_pool_t *);
// Deallocates a block in the Pool.
void BTR_Pool_deallocate(btr_pool_t *, void *pointer);
// Deallocates the entire Pool.
void BTR_Pool_destroy(btr_pool_t *);
// Deallocates the entire Pool and allocates again with the new capacity.
void BTR_Pool_reset(btr_pool_t *, size_t newItemCount);
// Returns a `btr_allocator_t` interface wrapper for the specified Pool.
btr_allocator_t BTR_Pool_getWrapper(btr_pool_t *);

#ifdef BTR_NO_PREFIX

typedef btr_pool_t pool_t;

#define Pool_make           BTR_Pool_make
#define Pool_allocate       BTR_Pool_allocate
#define Pool_deallocate     BTR_Pool_deallocate
#define Pool_destroy        BTR_Pool_destroy
#define Pool_reset          BTR_Pool_reset
#define BTR_Pool_getWrapper Pool_getWrapper

#endif // BTR_NO_PREFIX
