#include "btrstd/allocator.h"

btr_alloc_result_t BTR_Allocator_allocate(btr_allocator_t *this, size_t size)
{
    return this->allocate(this->context, size);
}
btr_alloc_result_t BTR_Allocator_reallocate(btr_allocator_t *this, void *pointer, size_t newSize)
{
    return this->reallocate(this->context, pointer, newSize);
}
void BTR_Allocator_deallocate(btr_allocator_t *this, void *pointer)
{
    this->deallocate(this->context, pointer);
}
