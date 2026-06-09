#include "btrstd/containers/balist.h"

#include "stdlib.h"
#include "string.h"

static void checkNull(btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (!this->data)
        *this = BTR_BAList_makeEmpty(8);
}
static void checkSizeToGrow(btr_balist_t *this)
{
    if (this->count >= this->capacity)
    {
        size_t newCapacity = this->capacity ? this->capacity * 3 / 2 : 8;
        void **newData = realloc(this->data, newCapacity * sizeof(void *));
        BTR_panicIf(!newData, "reallocation failed");
        this->data = newData;
        this->capacity = newCapacity;
    }
}
static void checkSizeToShrink(btr_balist_t *this)
{
    if (this->count <= this->capacity / 4)
    {
        size_t newCapacity = this->capacity / 4;
        void **newData = realloc(this->data, newCapacity * sizeof(void *));
        BTR_panicIf(!newData, "reallocation failed");
        this->data = newData;
        this->capacity = newCapacity;
    }
}

btr_balist_t BTR_BAList_make(void *items[], size_t itemCount)
{
    void **newData = malloc(itemCount * sizeof(void *));
    BTR_panicIf(!newData, "allocation failed");
    memcpy(newData, items, itemCount * sizeof(void *));
    return (btr_balist_t) {
        .data = newData,
        .capacity = itemCount,
        .count = itemCount,
    };
}
btr_balist_t BTR_BAList_makeEmpty(size_t capacity)
{
    void **data = calloc(capacity, sizeof(void *));
    BTR_panicIf(!data, "allocation failed");
    return (btr_balist_t) {
        .data = data,
        .capacity = capacity,
        .count = 0,
    };
}
btr_balist_t BTR_BAList_clone(const btr_balist_t *list)
{
    BTR_panicIf(!list, "`list` is invalid");
    void **data = malloc(list->capacity * sizeof(void *));
    BTR_panicIf(!data, "allocation failed");
    memcpy(data, list->data, list->capacity * sizeof(void *));
    return (btr_balist_t) {
        .data = data,
        .capacity = list->capacity,
        .count = list->count,
    };
}
void BTR_BAList_append(btr_balist_t *this, void *data)
{
    BTR_panicIf(!this, "`this` is invalid");
    checkNull(this);
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
    checkNull(this);
    if (index < 0) index = this->count + index;
    BTR_panicIf(index < 0 || (size_t)index > this->count, "index out of bounds");
    checkSizeToGrow(this);
    for (size_t i = this->count; i > (size_t)index; i--)
        this->data[i] = this->data[i - 1];
    this->count++;
    this->data[index] = data;
}
btr_balist_ptr_result_t BTR_BAList_pop(btr_balist_t *this, long index)
{
    BTR_panicIf(!this, "`this` is invalid");
    checkNull(this);
    if (index < 0) index = this->count + index;
    if (index < 0 || (size_t)index >= this->count)
        BTR_Err(btr_balist_ptr_result_t, BTR_BALIST_ERR_OUT_OF_BOUNDS);
    void *toReturn = this->data[index];
    for (size_t i = index; i < this->count - 1; i++)
        this->data[i] = this->data[i + 1];
    this->count--;
    checkSizeToShrink(this);
    BTR_Ok(btr_balist_ptr_result_t, toReturn);
}
btr_balist_ptr_result_t BTR_BAList_get(const btr_balist_t *this, long index)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (index < 0) index = this->count + index;
    if (index < 0 || (size_t)index >= this->count)
        BTR_Err(btr_balist_ptr_result_t, BTR_BALIST_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_balist_ptr_result_t, this->data[index]);
}
btr_balist_ptr_result_t BTR_BAList_first(const btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (!this->count)
        BTR_Err(btr_balist_ptr_result_t, BTR_BALIST_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_balist_ptr_result_t, this->data[0]);
}
btr_balist_ptr_result_t BTR_BAList_last(const btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    if (!this->count)
        BTR_Err(btr_balist_ptr_result_t, BTR_BALIST_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_balist_ptr_result_t, this->data[this->count - 1]);
}
btr_balist_idx_result_t BTR_BAList_indexOf(btr_balist_t *list, void *value, bool (*cmp)(const void *, const void *))
{
    BTR_panicIf(!list, "`list` is invalid");
    BTR_BALIST_ENUMERATE(list, i, n)
        if (cmp(i, value))
            BTR_Ok(btr_balist_idx_result_t, n);
    BTR_Err(btr_balist_idx_result_t, BTR_BALIST_ERR_NOT_FOUND);
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
    void **newData = malloc(this->capacity * sizeof(void *));
    BTR_panicIf(!newData, "allocation failed");
    BTR_BALIST_ENUMERATE(this, i, n)
        newData[this->count - n - 1] = i;
    free(this->data);
    this->data = newData;
}
void BTR_BAList_free(btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    free(this->data);
    this->data = NULL;
    this->count = 0;
    this->capacity = 0;
}
void BTR_BAList_clear(btr_balist_t *this)
{
    BTR_panicIf(!this, "`this` is invalid");
    BTR_BAList_free(this);
}
