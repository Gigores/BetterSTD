#pragma once

#include "btrstd/allocator.h"

// A wrapper around the global stdlib allocator.

// Allocator capabilities:
// - allocate:   OK/BTR_ALLOC_ERR_OUT_OF_MEMORY
// - reallocate: OK/BTR_ALLOC_ERR_OUT_OF_MEMORY
// - deallocate: OK

btr_alloc_result_t BTR_allocate(size_t size);
btr_alloc_result_t BTR_reallocate(void *pointer, size_t newSize);
void BTR_deallocate(void *pointer);

const btr_allocator_t *BTR_getGlobalAllocator(void);

#ifdef BTR_NO_PREFIX

#define allocate   BTR_allocate
#define reallocate BTR_reallocate
#define deallocate BTR_deallocate

#define getGlobalAllocator BTR_getGlobalAllocator

#endif  // BTR_NO_PREFIX
