#pragma once

#include "btrstd/error.h"

#include "stddef.h"

typedef enum BTR_AllocErr {
    BTR_ALLOC_ERR_OUT_OF_MEMORY,
    BTR_ALLOC_ERR_UNSUPPORTED_OPERATION,
    BTR_ALLOC_ERR_INVALID_POINTER,
    BTR_ALLOC_ERR_INVALID_SIZE,
} btr_alloc_err_e;

typedef BTR_RESULT(void *, btr_alloc_err_e) btr_alloc_r;

// Generic allocator wrapper
typedef struct BTR_Allocator {
    void *context;
    btr_alloc_r (*allocate)(
        void *context,
        size_t size
    );
    btr_alloc_r (*reallocate)(
        void *context,
        void *pointer,
        size_t newSize
    );
    void (*deallocate)(
        void *context,
        void *pointer
    );
} btr_allocator_s;

btr_alloc_r BTR_Allocator_allocate(btr_allocator_s *, size_t size);
btr_alloc_r BTR_Allocator_reallocate(btr_allocator_s *, void *pointer, size_t newSize);
void BTR_Allocator_deallocate(btr_allocator_s *, void *pointer);

#ifdef BTR_NO_PREFIX

#define AllocErr BTR_AllocErr
#define Allocator BTR_Allocator

typedef btr_alloc_err_e alloc_err_e;
typedef btr_alloc_r     alloc_r;
typedef btr_allocator_s allocator_s;

#define ALLOC_ERR_OUT_OF_MEMORY         BTR_ALLOC_ERR_OUT_OF_MEMORY
#define ALLOC_ERR_UNSUPPORTED_OPERATION BTR_ALLOC_ERR_UNSUPPORTED_OPERATION
#define ALLOC_ERR_INVALID_POINTER       BTR_ALLOC_ERR_INVALID_POINTER
#define ALLOC_ERR_INVALID_SIZE          BTR_ALLOC_ERR_INVALID_SIZE

#define Allocator_allocate   BTR_Allocator_allocate
#define Allocator_reallocate BTR_Allocator_reallocate
#define Allocator_deallocate BTR_Allocator_deallocate

#endif  // BTR_NO_PREFIX
