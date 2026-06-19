#pragma once

#include "btrstd/allocator.h"

// Arena Allocator

// Arena is a linear allocator. That means, that it is fast, but
// cannot free or deallocate individual pointers.

// Pros: Extremely fast and can allocate differently sized memory chunks.
// Cons: Can't deallocate.

// Allocator capabilities:
// - allocate:   OK/BTR_ALLOC_ERR_OUT_OF_MEMORY
// - reallocate: BTR_ALLOC_ERR_UNSUPPORTED_OPERATION
// - deallocate: OK (no-op)

// Parent allocator required capabilities:
//   allocate variadic_size, deallocate

typedef struct {
    void *data;
    size_t capacity;
    size_t next;
    btr_allocator_s *allocator;
} btr_arena_s;

// Creates an empty Arena.
// The `parentAllocator` parameter can be set as `NULL`, in this case it will use
// the global allocator.
btr_arena_s BTR_Arena_make(size_t capacity, btr_allocator_s *parentAllocator);
// Allocates a given amount of bytes in the Arena.
// Can return BTR_ALLOC_ERR_OUT_OF_MEMORY.
btr_alloc_r BTR_Arena_allocate(btr_arena_s *, size_t size);
// Deallocates the entire Arena.
void BTR_Arena_destroy(btr_arena_s *);
// Deallocates the entire Arena and allocates again with the new capacity.
void BTR_Arena_reset(btr_arena_s *, size_t newCapacity);
// Returns a `btr_allocator_t` interface wrapper for the specified Arena.
btr_allocator_s BTR_Arena_getWrapper(btr_arena_s *);

#ifdef BTR_NO_PREFIX

typedef btr_arena_s arena_s;

#define Arena_make       BTR_Arena_make
#define Arena_allocate   BTR_Arena_allocate
#define Arena_destroy    BTR_Arena_destroy
#define Arena_reset      BTR_Arena_reset
#define Arena_getWrapper BTR_Arena_getWrapper

#endif  // BTR_NO_PREFIX
