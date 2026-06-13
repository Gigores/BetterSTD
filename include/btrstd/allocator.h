#pragma once

#include "btrstd/error.h"

#include "stddef.h"

typedef enum {
    BTR_ALLOC_ERR_OUT_OF_MEMORY,
    BTR_ALLOC_ERR_UNSUPPORTED_OPERATION,
    BTR_ALLOC_ERR_INVALID_POINTER,
    BTR_ALLOC_ERR_INVALID_SIZE,
} btr_alloc_err_t;

typedef BTR_Result(void *, btr_alloc_err_t) btr_alloc_result_t;

// Generic allocator wrapper
typedef struct {
    void *context;
    btr_alloc_result_t (*allocate)(
        void *context,
        size_t size
    );
    btr_alloc_result_t (*reallocate)(
        void *context,
        void *pointer,
        size_t newSize
    );
    void (*deallocate)(
        void *context,
        void *pointer
    );
} btr_allocator_t;

btr_alloc_result_t BTR_Allocator_allocate(btr_allocator_t *, size_t size);
btr_alloc_result_t BTR_Allocator_reallocate(btr_allocator_t *, void *pointer, size_t newSize);
void BTR_Allocator_deallocate(btr_allocator_t *, void *pointer);

#ifdef BTR_NO_PREFIX

typedef btr_alloc_err_t    alloc_err_t;
typedef btr_alloc_result_t alloc_result_t;
typedef btr_allocator_t    allocator_t;

#define ALLOC_ERR_OUT_OF_MEMORY         BTR_ALLOC_ERR_OUT_OF_MEMORY
#define ALLOC_ERR_UNSUPPORTED_OPERATION BTR_ALLOC_ERR_UNSUPPORTED_OPERATION
#define ALLOC_ERR_INVALID_POINTER       BTR_ALLOC_ERR_INVALID_POINTER
#define ALLOC_ERR_INVALID_SIZE          BTR_ALLOC_ERR_INVALID_SIZE

#define Allocator_allocate   BTR_Allocator_allocate
#define Allocator_reallocate BTR_Allocator_reallocate
#define Allocator_deallocate BTR_Allocator_deallocate

#endif  // BTR_NO_PREFIX
