#include "btrstd/allocators/pool.h"
#include "_util.h"

#include "string.h"


btr_pool_t BTR_Pool_make(size_t itemSize, size_t itemCount, btr_allocator_t *parentAllocator)
{
    btr_allocator_t *theAllocator = getAllocator(parentAllocator);
    uint8_t *data = BTR_expect(
        BTR_Allocator_allocate(theAllocator, itemSize * itemCount),
        "Allocation failed"
    );
    return (btr_pool_t) {
        .data = data,
        .allocator = theAllocator,
        .itemCount = itemCount,
        .itemSize = itemSize,
        .mask = BTR_BitSet_make(itemCount, theAllocator),
    };
}
btr_alloc_result_t BTR_Pool_allocate(btr_pool_t *this)
{
    long index = -1;
    BTR_BITSET_ENUMERATE(&this->mask, i, n)
        if (!i)
        {
            index = n;
            break;
        }
    if (index < 0)
        BTR_Err(btr_alloc_result_t, BTR_ALLOC_ERR_OUT_OF_MEMORY);
    BTR_BitSet_set(&this->mask, index);
    uint8_t *data = this->data + index * this->itemSize;
    memset(this->data + index * this->itemSize, 0, this->itemSize);
    BTR_Ok(btr_alloc_result_t, data);
}
void BTR_Pool_deallocate(btr_pool_t *this, void *pointer)
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
void BTR_Pool_destroy(btr_pool_t *this)
{
    BTR_Allocator_deallocate(this->allocator, this->data);
    BTR_BitSet_free(&this->mask);
}
void BTR_Pool_reset(btr_pool_t *this, size_t newItemCount)
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
