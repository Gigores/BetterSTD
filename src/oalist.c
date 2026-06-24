#include "btrstd/containers/owning_array_list.h"
#include "_util.c"

#include "string.h"


static void checkSizeToGrow(btr_oalist_s *this)
{
    if (this->count >= this->capacity)
    {
        size_t newCapacity = this->capacity ? this->capacity * 3 / 2 : 8;

        void *newData = BTR_expect(
            BTR_Allocator_allocate(this->allocator, newCapacity * this->itemSize),
            "Reallocation failed"
        );
        memcpy(newData, this->data, this->count * this->itemSize);
        BTR_Allocator_deallocate(this->allocator, this->data);
        this->data = newData;
        this->capacity = newCapacity;
    }
}
static void checkSizeToShrink(btr_oalist_s *this)
{
    if (this->count <= this->capacity / 4)
    {
        size_t newCapacity = this->capacity / 2;
        if (newCapacity == 0) {
            BTR_Allocator_deallocate(this->allocator, this->data);
            this->data = NULL;
            this->capacity = 0;
        } else {
            void *newData = BTR_expect(
                BTR_Allocator_allocate(this->allocator, newCapacity * this->itemSize),
                "Reallocation failed"
            );
            memcpy(newData, this->data, this->count * this->itemSize);
            BTR_Allocator_deallocate(this->allocator, this->data);
            this->data = newData;
            this->capacity = newCapacity;
        }
    }
}

btr_oalist_s BTR_OAList_makeFrom(
    void *items,
    size_t itemCount,
    size_t itemSize,
    btr_allocator_s *allocator
) {
    btr_allocator_s *theAllocator = getAllocator(allocator);
    void *data = BTR_expect(
        BTR_Allocator_allocate(theAllocator, itemCount * itemSize),
        "Allocation failed"
    );
    memcpy(data, items, itemCount * itemSize);
    return (btr_oalist_s)
    {
        .data = data,
        .capacity = itemCount,
        .count = itemCount,
        .itemSize = itemSize,
        .allocator = theAllocator,
    };
}
btr_oalist_s BTR_OAList_make(
    size_t capacity,
    size_t itemSize,
    btr_allocator_s *allocator
) {
    btr_allocator_s *theAllocator = getAllocator(allocator);
    void *data = BTR_expect(
        BTR_Allocator_allocate(theAllocator, capacity * itemSize),
        "Allocation failed"
    );
    return (btr_oalist_s)
    {
        .data = data,
        .capacity = capacity,
        .count = 0,
        .itemSize = itemSize,
        .allocator = theAllocator,
    };
}
btr_oalist_s BTR_OAList_clone(const btr_oalist_s *list, btr_allocator_s *allocator)
{
    btr_allocator_s *theAllocator = getAllocator(allocator);
    void *data = BTR_expect(
        BTR_Allocator_allocate(theAllocator, list->capacity * list->itemSize),
        "Allocation failed"
    );
    memcpy(data, list->data, list->capacity * list->itemSize);
    return (btr_oalist_s)
    {
        .data = data,
        .capacity = list->capacity,
        .count = list->count,
        .itemSize = list->itemSize,
        .allocator = theAllocator,
    };
}
void *BTR_OAList_append(btr_oalist_s *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    checkSizeToGrow(this);
    return this->data + this->count++ * this->itemSize;
}
void *BTR_OAList_prepend(btr_oalist_s *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    return BTR_OAList_insert(this, 0);
}
void *BTR_OAList_insert(btr_oalist_s *this, long index)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (index < 0) index = this->count + index;
    BTR_panicIf(index < 0 || (size_t)index > this->count, "Index out of bounds");
    checkSizeToGrow(this);
    memmove(
        this->data + (index + 1) * this->itemSize,
        this->data + index * this->itemSize,
        (this->count - index) * this->itemSize
    );
    this->count++;
    return this->data + index * this->itemSize;
}
void BTR_OAList_pop(btr_oalist_s *this, long index, void *buffer)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (index < 0) index = this->count + index;
    BTR_panicIf(index < 0 || (size_t)index >= this->count, "Index out of bounds");
    if (buffer)
        memcpy(buffer, this->data + index * this->itemSize, this->itemSize);
    memmove(
        this->data + index * this->itemSize,
        this->data + (index + 1) * this->itemSize,
        (this->count - index - 1) * this->itemSize
    );
    this->count--;
    checkSizeToShrink(this);
}
btr_container_ptr_r BTR_OAList_get(const btr_oalist_s *this, long index)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (index < 0) index = this->count + index;
    if (index < 0 || (size_t)index >= this->count)
        BTR_ERR(btr_container_ptr_r, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    BTR_OK(btr_container_ptr_r, this->data + index * this->itemSize);
}
btr_container_ptr_r BTR_OAList_first(const btr_oalist_s *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (!this->count)
        BTR_ERR(btr_container_ptr_r, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    BTR_OK(btr_container_ptr_r, this->data);
}
btr_container_ptr_r BTR_OAList_last(const btr_oalist_s *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (!this->count)
        BTR_ERR(btr_container_ptr_r, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    BTR_OK(btr_container_ptr_r, this->data + (this->count - 1) * this->itemSize);
}
btr_container_idx_r BTR_OAList_indexOf
    (btr_oalist_s *list, void *value, bool (*cmp)(const void *, const void *))
{
    BTR_panicIf(!list, "`list` is invalid");
    BTR_OALIST_ENUMERATE(list, i, n)
        if (cmp(i, value))
            BTR_OK(btr_container_idx_r, n);
    BTR_ERR(btr_container_idx_r, BTR_CONTAINER_ERR_NOT_FOUND);
}
size_t BTR_OAList_len(const btr_oalist_s *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    return this->count;
}
bool BTR_OAList_isEmpty(const btr_oalist_s *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    return !BTR_OAList_len(this);
}
void BTR_OAList_reverse(btr_oalist_s *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    void *newData = BTR_expect(
        BTR_Allocator_allocate(this->allocator, this->capacity * this->itemSize),
        "Allocation failed"
    );
    BTR_OALIST_ENUMERATE(this, i, n)
        memcpy(newData + (this->count - n - 1) * this->itemSize, i, this->itemSize);
    BTR_Allocator_deallocate(this->allocator, this->data);
    this->data = newData;
}
void BTR_OAList_free(btr_oalist_s *this)
{
    BTR_Allocator_deallocate(this->allocator, this->data);
}
void BTR_OAList_clear(btr_oalist_s *this)
{
    BTR_OAList_free(this);
    void *data = BTR_expect(
        BTR_Allocator_allocate(this->allocator, this->itemSize * 8),
        "Allocation failed"
    );
    this->data = data;
    this->capacity = 8;
    this->count = 0;
}
