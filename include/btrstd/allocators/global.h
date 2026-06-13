#pragma once

#include "btrstd/allocator.h"

// A wrapper around the global stdlib allocator.

// Allocator capabilities:
// - allocate:   OK/OOM
// - reallocate: OK/OOM
// - deallocate: OK

btr_alloc_result_t BTR_allocate(size_t size);
btr_alloc_result_t BTR_reallocate(void *pointer, size_t newSize);
void BTR_deallocate(void *pointer);

const btr_allocator_t *BTR_getGlobalAllocator(void);
