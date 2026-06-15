#include "btrstd/allocators/global.h"

#include "stdlib.h"


btr_alloc_result_t BTR_allocate(size_t size)
{
    void *pointer = calloc(size, 1);
    if (!pointer) BTR_Err(btr_alloc_result_t, BTR_ALLOC_ERR_OUT_OF_MEMORY);
    BTR_Ok(btr_alloc_result_t, pointer);
}
btr_alloc_result_t BTR_reallocate(void *pointer, size_t newSize)
{
    void *newPointer = realloc(pointer, newSize);
    if (!newPointer) BTR_Err(btr_alloc_result_t, BTR_ALLOC_ERR_OUT_OF_MEMORY);
    BTR_Ok(btr_alloc_result_t, newPointer);
}
void BTR_deallocate(void *pointer)
{
    free(pointer);
}
btr_alloc_result_t BTR_allocatec(__attribute__((unused)) void *context, size_t size)
{
    return BTR_allocate(size);
}
btr_alloc_result_t BTR_reallocatec(__attribute__((unused)) void *context, void *pointer, size_t newSize)
{
    return BTR_reallocate(pointer, newSize);
}
void BTR_deallocatec(__attribute__((unused)) void *context, void *pointer)
{
    BTR_deallocate(pointer);
}

static const btr_allocator_t GLOBAL_ALLOCATOR = {
    .context = NULL,
    .allocate = BTR_allocatec,
    .reallocate = BTR_reallocatec,
    .deallocate = BTR_deallocatec,
};
const btr_allocator_t *BTR_getGlobalAllocator(void)
{
    return &GLOBAL_ALLOCATOR;
}
