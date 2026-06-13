#pragma once

#include "btrstd/allocator.h"

// __Arena__ is a linear allocator. That means, that it is __fast__, but cannot free or deallocate individual pointers.

// Allocator capabilities:
// - allocate:   OK/OOM
// - reallocate: BTR_ALLOC_ERR_UNSUPPORTED_OPERATION
// - deallocate: OK (no-op)

// Parent allocator required capabilities:
//   allocate, deallocate

typedef struct {
    void *data;
    size_t capacity;
    size_t next;
    btr_allocator_t *allocator;
} btr_arena_t;

// Creates an empty Arena.
// The `parentAllocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_arena_t BTR_Arena_make(size_t capacity, btr_allocator_t *parentAllocator);
// Allocates a given amount of bytes in the Arena.
// Can return BTR_ALLOC_ERR_OUT_OF_MEMORY.
btr_alloc_result_t BTR_Arena_allocate(btr_arena_t *, size_t size);
// Deallocates the entire Arena.
void BTR_Arena_destroy(btr_arena_t *);
// Deallocates the entire Arena and allocates again with the new capacity.
void BTR_Arena_reset(btr_arena_t *, size_t newCapacity);
// Returns a `btr_allocator_t` wrapper for the specified Arena.
btr_allocator_t BTR_Arena_getWrapper(btr_arena_t *);
