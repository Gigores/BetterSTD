#include "btrstd/containers/balist.h"
#include "_util.h"

#include "stdlib.h"
#include "string.h"


static void checkSizeToGrow(btr_balist_t *this)
{
    if (this->count >= this->capacity)
    {
        size_t newCapacity = this->capacity ? this->capacity * 3 / 2 : 8;

        void **newData = BTR_expect(BTR_Allocator_allocate(this->allocator, newCapacity * sizeof(void *)), "Reallocation failed");
        memcpy(newData, this->data, this->count * sizeof(void *));
        BTR_Allocator_deallocate(this->allocator, this->data);
        this->data = newData;
        this->capacity = newCapacity;
    }
}
static void checkSizeToShrink(btr_balist_t *this)
{
    if (this->count <= this->capacity / 4)
    {
        size_t newCapacity = this->capacity / 2;
        if (newCapacity == 0) {
            BTR_Allocator_deallocate(this->allocator, this->data);
            this->data = NULL;
            this->capacity = 0;
        } else {
            void **newData = BTR_expect(BTR_Allocator_allocate(this->allocator, newCapacity * sizeof(void *)), "Reallocation failed");
            memcpy(newData, this->data, this->count * sizeof(void *));
            BTR_Allocator_deallocate(this->allocator, this->data);
            this->data = newData;
            this->capacity = newCapacity;
        }
    }
}

btr_balist_t BTR_BAList_makeFrom(void *items[], size_t itemCount, btr_allocator_t *allocator)
{
    BTR_panicIf(!items && itemCount > 0, "`items` is NULL with non-zero itemCount");
    void **newData = BTR_expect(BTR_Allocator_allocate(getAllocator(allocator), itemCount * sizeof(void *)), "Allocation failed");
    memcpy(newData, items, itemCount * sizeof(void *));
    return (btr_balist_t) {
        .data = newData,
        .capacity = itemCount,
        .count = itemCount,
        .allocator = getAllocator(allocator),
    };
}
btr_balist_t BTR_BAList_make(size_t capacity, btr_allocator_t *allocator)
{
    void **data = BTR_expect(BTR_Allocator_allocate(getAllocator(allocator), ( capacity ? capacity : 8 ) * sizeof(void *)), "Allocation failed");
    return (btr_balist_t) {
        .data = data,
        .capacity = capacity ? capacity : 8,
        .count = 0,
        .allocator = getAllocator(allocator),
    };
}
btr_balist_t BTR_BAList_clone(const btr_balist_t *list, btr_allocator_t *allocator)
{
    BTR_panicIf(!list, "`list` is invalid");
    void **data = BTR_expect(BTR_Allocator_allocate(getAllocator(allocator), list->capacity * sizeof(void *)), "Allocation failed");
    memcpy(data, list->data, list->capacity * sizeof(void *));
    return (btr_balist_t) {
        .data = data,
        .capacity = list->capacity,
        .count = list->count,
        .allocator = getAllocator(allocator),
    };
}
void BTR_BAList_append(btr_balist_t *this, void *data)
{
    BTR_panicIf(!this, "`this` is invalid");
    checkSizeToGrow(this);
    this->data[this->count++] = data;
}
void BTR_BAList_prepend(btr_balist_t *this, void *data)
{
    BTR_panicIf(!this, "`this` is invalid");
    BTR_BAList_insert(this, data, 0);
}
void BTR_BAList_insert(btr_balist_t *this, void *data, long index)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (index < 0) index = this->count + index;
    BTR_panicIf(index < 0 || (size_t)index > this->count, "index out of bounds");
    checkSizeToGrow(this);
    memmove(this->data + index + 1, this->data + index, (this->count - index) * sizeof(void *));
    this->count++;
    this->data[index] = data;
}
btr_container_ptr_result_t BTR_BAList_pop(btr_balist_t *this, long index)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (index < 0) index = this->count + index;
    if (index < 0 || (size_t)index >= this->count)
        BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    void *toReturn = this->data[index];
    memmove(this->data + index, this->data + index + 1, (this->count - index) * sizeof(void *));
    this->count--;
    checkSizeToShrink(this);
    BTR_Ok(btr_container_ptr_result_t, toReturn);
}
btr_container_ptr_result_t BTR_BAList_get(const btr_balist_t *this, long index)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (index < 0) index = this->count + index;
    if (index < 0 || (size_t)index >= this->count)
        BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_container_ptr_result_t, this->data[index]);
}
void BTR_BAList_set(btr_balist_t *this, void *data, long index)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (index < 0) index = this->count + index;
    BTR_panicIf(index < 0 || (size_t)index >= this->count, "index out of bounds");
    this->data[index] = data;
}
btr_container_ptr_result_t BTR_BAList_first(const btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (!this->count)
        BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_container_ptr_result_t, this->data[0]);
}
btr_container_ptr_result_t BTR_BAList_last(const btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (!this->count)
        BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_container_ptr_result_t, this->data[this->count - 1]);
}
btr_container_idx_result_t BTR_BAList_indexOf(btr_balist_t *list, void *value, bool (*cmp)(const void *, const void *))
{
    BTR_panicIf(!list, "`list` is invalid");
    BTR_BALIST_ENUMERATE(list, i, n)
        if (cmp(i, value))
            BTR_Ok(btr_container_idx_result_t, n);
    BTR_Err(btr_container_idx_result_t, BTR_CONTAINER_ERR_NOT_FOUND);
}
size_t BTR_BAList_len(const btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    return this->count;
}
bool BTR_BAList_isEmpty(const btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    return this->count == 0;
}
void BTR_BAList_reverse(btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    void **newData = BTR_expect(BTR_Allocator_allocate(this->allocator, this->capacity * sizeof(void *)), "Allocation failed");
    BTR_BALIST_ENUMERATE(this, i, n)
        newData[this->count - n - 1] = i;
    BTR_Allocator_deallocate(this->allocator, this->data);
    this->data = newData;
}
void BTR_BAList_free(btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    BTR_Allocator_deallocate(this->allocator, this->data);
    this->data = NULL;
    this->count = 0;
    this->capacity = 0;
}
void BTR_BAList_clear(btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    BTR_BAList_free(this);
}
