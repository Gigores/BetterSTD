#include "btrstd/containers/bit_set.h"
#include "_util.h"

#include "string.h"


btr_bit_set_t BTR_BitSet_make(size_t bitCapacity, btr_allocator_t *allocator)
{
    size_t capacity = bitCapacity / 8;
    capacity += (bitCapacity % 8) ? 1 : 0;
    btr_allocator_t *theAllocator = getAllocator(allocator);
    char *data = BTR_expect(BTR_Allocator_allocate(theAllocator, capacity), "Allocation failed");
    return (btr_bit_set_t) {
        .data      = data,
        .bitCount  = bitCapacity,
        .allocator = theAllocator,
    };
}
btr_bit_set_t BTR_BitSet_clone(const btr_bit_set_t *bitSet, btr_allocator_t *allocator)
{
    BTR_panicIf(!bitSet, "`bitSet` is null");
    size_t capacity = bitSet->bitCount / 8;
    capacity += (bitSet->bitCount % 8) ? 1 : 0;
    btr_allocator_t *theAllocator = getAllocator(allocator);
    char *data = BTR_expect(BTR_Allocator_allocate(theAllocator, capacity), "Allocation failed");
    memcpy(data, bitSet->data, capacity);
    return (btr_bit_set_t) {
        .data      = data,
        .bitCount  = bitSet->bitCount,
        .allocator = theAllocator,
    };
}
btr_bit_result_t BTR_BitSet_get(const btr_bit_set_t *this, long index)
{
    BTR_panicIf(!this, "`this` is null");
    if (index < 0) index = this->bitCount + index;
    if (index < 0 || (size_t)index >= this->bitCount)
        BTR_Err(btr_bit_result_t, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    size_t byteIndex = (size_t) index / 8;
    size_t bitIndex = (size_t) index % 8;
    BTR_Ok(btr_bit_result_t, (this->data[byteIndex] >> bitIndex) & 1);
}
void BTR_BitSet_set(btr_bit_set_t *this, long index)
{
    BTR_panicIf(!this, "`this` is null");
    if (index < 0) index = this->bitCount + index;
    BTR_panicIf(index < 0 || (size_t)index >= this->bitCount, "Index out of bounds");
    size_t byteIndex = (size_t) index / 8;
    size_t bitIndex = (size_t) index % 8;
    this->data[byteIndex] |= (1u << bitIndex);
}
void BTR_BitSet_unset(btr_bit_set_t *this, long index)
{
    BTR_panicIf(!this, "`this` is null");
    if (index < 0) index = this->bitCount + index;
    BTR_panicIf(index < 0 || (size_t)index >= this->bitCount, "Index out of bounds");
    size_t byteIndex = (size_t) index / 8;
    size_t bitIndex = (size_t) index % 8;
    this->data[byteIndex] &= ~(1u << bitIndex);
}
btr_bit_result_t BTR_BitSet_flip(btr_bit_set_t *this, long index)
{
    BTR_panicIf(!this, "`this` is null");
    if (index < 0) index = this->bitCount + index;
    if (index < 0 || (size_t)index >= this->bitCount)
        BTR_Err(btr_bit_result_t, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    size_t byteIndex = (size_t) index / 8;
    size_t bitIndex = (size_t) index % 8;
    this->data[byteIndex] ^= (1u << bitIndex);
    BTR_Ok(btr_bit_result_t, (this->data[byteIndex] >> bitIndex) & 1);
}
void BTR_BitSet_free(btr_bit_set_t *this)
{
    BTR_Allocator_deallocate(this->allocator, this->data);
}
