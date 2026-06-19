#include "btrstd/allocators/global.h"

#include "stdlib.h"


btr_alloc_r BTR_allocate(size_t size)
{
    void *pointer = calloc(size, 1);
    if (!pointer) BTR_Err(btr_alloc_r, BTR_ALLOC_ERR_OUT_OF_MEMORY);
    BTR_Ok(btr_alloc_r, pointer);
}
btr_alloc_r BTR_reallocate(void *pointer, size_t newSize)
{
    void *newPointer = realloc(pointer, newSize);
    if (!newPointer) BTR_Err(btr_alloc_r, BTR_ALLOC_ERR_OUT_OF_MEMORY);
    BTR_Ok(btr_alloc_r, newPointer);
}
void BTR_deallocate(void *pointer)
{
    free(pointer);
}
btr_alloc_r BTR_allocatec(
    __attribute__((unused)) void *context,
    size_t size
) {
    return BTR_allocate(size);
}
btr_alloc_r BTR_reallocatec(
    __attribute__((unused)) void *context,
    void *pointer,
    size_t newSize
) {
    return BTR_reallocate(pointer, newSize);
}
void BTR_deallocatec(
    __attribute__((unused)) void *context,
    void *pointer
){
    BTR_deallocate(pointer);
}

static const btr_allocator_s GLOBAL_ALLOCATOR = {
    .context = NULL,
    .allocate = BTR_allocatec,
    .reallocate = BTR_reallocatec,
    .deallocate = BTR_deallocatec,
};
const btr_allocator_s *BTR_getGlobalAllocator(void)
{
    return &GLOBAL_ALLOCATOR;
}
