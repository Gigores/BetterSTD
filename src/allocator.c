#include "btrstd/allocator.h"


btr_alloc_r BTR_Allocator_allocate(btr_allocator_s *this, size_t size)
{
    BTR_panicIf(!this, "`this` allocator is NULL");
    return this->allocate(this->context, size);
}
btr_alloc_r BTR_Allocator_reallocate(btr_allocator_s *this, void *pointer, size_t newSize)
{
    BTR_panicIf(!this, "`this` allocator is NULL");
    return this->reallocate(this->context, pointer, newSize);
}
void BTR_Allocator_deallocate(btr_allocator_s *this, void *pointer)
{
    BTR_panicIf(!this, "`this` allocator is NULL");
    this->deallocate(this->context, pointer);
}
