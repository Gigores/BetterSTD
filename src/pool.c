#include "btrstd/allocators/pool.h"
#include "_util.c"

#include "string.h"


btr_pool_s BTR_Pool_make(size_t itemSize, size_t itemCount, btr_allocator_s *parentAllocator)
{
    btr_allocator_s *theAllocator = getAllocator(parentAllocator);
    uint8_t *data = BTR_expect(
        BTR_Allocator_allocate(theAllocator, itemSize * itemCount),
        "Allocation failed"
    );
    return (btr_pool_s) {
        .data = data,
        .allocator = theAllocator,
        .itemCount = itemCount,
        .itemSize = itemSize,
        .mask = BTR_BitSet_make(itemCount, theAllocator),
    };
}
btr_alloc_r BTR_Pool_allocate(btr_pool_s *this)
{
    // BTR_log(LOG_DEBUG, "Allocated");
    long index = -1;
    BTR_BITSET_ENUMERATE(&this->mask, i, n)
        if (!i)
        {
            index = n;
            break;
        }
    if (index < 0)
        BTR_ERR(btr_alloc_r, BTR_ALLOC_ERR_OUT_OF_MEMORY);
    BTR_BitSet_set(&this->mask, index);
    uint8_t *data = this->data + index * this->itemSize;
    memset(this->data + index * this->itemSize, 0, this->itemSize);
    BTR_OK(btr_alloc_r, data);
}
void BTR_Pool_deallocate(btr_pool_s *this, void *pointer)
{
    ptrdiff_t offset = (uint8_t *)pointer - this->data;
    size_t index = offset / this->itemSize;
    BTR_panicIf(
        offset < 0 ||
        offset % this->itemSize != 0 ||
        index >= this->itemCount,
        "Invalid pointer"
    );
    BTR_panicIf(
        !BTR_unwrap(BTR_BitSet_get(&this->mask, index)),
        "Double free detected"
    );
    BTR_BitSet_unset(&this->mask, index);
}
void BTR_Pool_destroy(btr_pool_s *this)
{
    BTR_Allocator_deallocate(this->allocator, this->data);
    BTR_BitSet_free(&this->mask);
}
void BTR_Pool_reset(btr_pool_s *this, size_t newItemCount)
{
    BTR_Pool_destroy(this);
    uint8_t *newData = BTR_expect(
        BTR_Allocator_allocate(this->allocator, this->itemSize * newItemCount),
        "Allocation failed"
    );
    this->data = newData;
    this->itemCount = newItemCount;
    this->mask = BTR_BitSet_make(newItemCount, this->allocator);
}
btr_alloc_r BTR_Pool_allocatec(void *context, size_t size)
{
    BTR_panicIf(!context, "NULL allocator context");
    if (size != ((btr_pool_s *)context)->itemSize)
        BTR_ERR(btr_alloc_r, BTR_ALLOC_ERR_INVALID_SIZE);
    return BTR_Pool_allocate(context);
}
btr_alloc_r BTR_Pool_reallocatec(
    __attribute__((unused)) void *context,
    __attribute__((unused)) void* pointer,
    __attribute__((unused)) size_t size
) {
    BTR_ERR(btr_alloc_r, BTR_ALLOC_ERR_UNSUPPORTED_OPERATION);
}
void BTR_Pool_deallocatec(void *context, void *pointer)
{
    BTR_Pool_deallocate(context, pointer);
}
btr_allocator_s BTR_Pool_getWrapper(btr_pool_s *this)
{
    return (btr_allocator_s) {
        .context = this,
        .allocate = BTR_Pool_allocatec,
        .reallocate = BTR_Pool_reallocatec,
        .deallocate = BTR_Pool_deallocatec,
    };
}
