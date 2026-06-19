#include "btrstd/allocators/arena.h"

#include "_util.h"


btr_arena_s BTR_Arena_make(size_t capacity, btr_allocator_s *parentAllocator)
{
    btr_allocator_s *theAllocator = getAllocator(parentAllocator);
    void *data = BTR_expect(
        BTR_Allocator_allocate(theAllocator, capacity),
        "Allocation failed"
    );
    return (btr_arena_s) {
        .data = data,
        .capacity = capacity,
        .next = 0,
        .allocator = theAllocator,
    };
}
btr_alloc_r BTR_Arena_allocate(btr_arena_s *this, size_t size)
{
    BTR_panicIf(!this, "`this` is NULL");
    if (this->next + size > this->capacity)
        BTR_Err(btr_alloc_r, BTR_ALLOC_ERR_OUT_OF_MEMORY);
    void *result = this->data + this->next;
    this->next += size;
    BTR_Ok(btr_alloc_r, result);
}
void BTR_Arena_destroy(btr_arena_s *this)
{
    BTR_panicIf(!this, "`this` is NULL");
    BTR_Allocator_deallocate(this->allocator, this->data);
}
void BTR_Arena_reset(btr_arena_s *this, size_t newCapacity)
{
    BTR_panicIf(!this, "`this` is NULL");
    BTR_Arena_destroy(this);
    void *newData = BTR_expect(
        BTR_Allocator_allocate(this->allocator, newCapacity),
        "Allocation failed"
    );
    this->data = newData;
    this->capacity = newCapacity;
    this->next = 0;
}
btr_alloc_r BTR_Arena_allocatec(void *context, size_t size)
{
    return BTR_Arena_allocate((btr_arena_s *)context, size);
}
btr_alloc_r BTR_Arena_reallocatec(
    __attribute__((unused)) void *context,
    __attribute__((unused)) void *pointer,
    __attribute__((unused)) size_t size
) {
    BTR_Err(btr_alloc_r, BTR_ALLOC_ERR_UNSUPPORTED_OPERATION);
}
void BTR_Arena_deallocatec(
    __attribute__((unused)) void *context,
    __attribute__((unused)) void *pointer
) {}

btr_allocator_s BTR_Arena_getWrapper(btr_arena_s *this)
{
    BTR_panicIf(!this, "`this` is NULL");
    return (btr_allocator_s) {
        .context = this,
        .allocate = BTR_Arena_allocatec,
        .reallocate = BTR_Arena_reallocatec,
        .deallocate = BTR_Arena_deallocatec,
    };
}
