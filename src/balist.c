#include "btrstd/containers/balist.h"

#include "stdlib.h"
#include "string.h"

#include "stdio.h"

static void checkNull(btr_balist_t *this)
{
    if (!this) return;
    if (!this->data)
        *this = BTR_BAList_makeEmpty(8);
}
static void checkSizeToGrow(btr_balist_t *this)
{
    if (this->count >= this->capacity)
    {
        void **newData = realloc(this->data, this->capacity * 1.5 * sizeof(void *));
        if (!newData) return;
        this->data = newData;
        this->capacity *= 1.5;
    }
}

btr_balist_t BTR_BAList_make(void *items[], size_t itemCount)
{
    void **newData = malloc(itemCount * sizeof(void *));
    if (!newData) return (btr_balist_t) {0};
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
    if (!data) return (btr_balist_t) {0};
    return (btr_balist_t) {
        .data = data,
        .capacity = capacity,
        .count = 0,
    };
}
btr_balist_t BTR_BAList_clone(const btr_balist_t *list)
{
    void **data = malloc(list->capacity * sizeof(void *));
    memcpy(data, list->data, list->capacity * sizeof(void *));
    return (btr_balist_t) {
        .data = data,
        .capacity = list->capacity,
        .count = list->count,
    };
}
void BTR_BAList_append(btr_balist_t *this, void *data)
{
    if (!this) return;
    checkNull(this);
    checkSizeToGrow(this);
    this->data[this->count++] = data;
}
void BTR_BAList_prepend(btr_balist_t *this, void *data)
{
    BTR_BAList_insert(this, data, 0);
}
void BTR_BAList_insert(btr_balist_t *this, void *data, long index)
{
    if (!this) return;
    checkNull(this);
    checkSizeToGrow(this);
    if (index < 0) index = this->count + index;
    for (size_t i = this->count; i > (size_t)index; i--)
        this->data[i] = this->data[i - 1];
    this->count++;
    this->data[index] = data;
}
void *BTR_BAList_pop(btr_balist_t *this, long index)
{
    if (!this) return NULL;
    checkNull(this);
    if (!this->count) return NULL;
    if (index < 0) index = this->count + index;
    void *toReturn = this->data[index];
    for (size_t i = index; i < this->count - 1; i++)
        this->data[i] = this->data[i + 1];
    this->count--;
    return toReturn;
}
void *BTR_BAList_get(const btr_balist_t *this, long index)
{
    if (!this) return NULL;
    if (index < 0) index = this->count + index;
    if ((size_t)index > this->count) return NULL;
    if (index < 0) return NULL;
    return this->data[index];
}
void *BTR_BAList_first(const btr_balist_t *this)
{
    if (!this) return NULL;
    if (!this->count) return NULL;
    return this->data[0];
}
void *BTR_BAList_last(const btr_balist_t *this)
{
    if (!this) return NULL;
    if (!this->count) return NULL;
    return this->data[this->count - 1];
}
long BTR_BAList_indexOf(btr_balist_t *list, void *value, bool (*cmp)(const void *, const void *))
{
    if (!list) return -1;
    BTR_BALIST_ENUMERATE(list, i, n)
        if (cmp(i, value)) return n;
    return -1;
}
size_t BTR_BAList_len(const btr_balist_t *this)
{
    if (!this) return 0;
    return this->count;
}
bool BTR_BAList_isEmpty(const btr_balist_t *this)
{
    if (!this) return false;
    return this->count == 0;
}
void BTR_BAList_reverse(btr_balist_t *this)
{
    void **newData = malloc(this->capacity * sizeof(void *));
    if (!newData) return;
    BTR_BALIST_ENUMERATE(this, i, n)
        newData[this->count - n - 1] = i;
    free(this->data);
    this->data = newData;
}
void BTR_BAList_free(btr_balist_t *this)
{
    free(this->data);
    this->data = NULL;
    this->count = 0;
    this->capacity = 0;
}
void BTR_BAList_clear(btr_balist_t *this)
{
    BTR_BAList_free(this);
}
