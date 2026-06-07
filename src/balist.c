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
    if (this->count >= this->capacity)
    {
        void **newData = realloc(this->data, this->capacity * 1.5 * sizeof(void *));
        if (!newData) return;
        this->data = newData;
        this->capacity *= 1.5;
    }
    this->data[this->count++] = data;
}
void *BTR_BAList_pop(btr_balist_t *this, long index)
{
    if (!this) return NULL;
    checkNull(this);
    if (!this->count) return NULL;
    if (index < 0) index = this->count + index;
    void *toReturn = this->data[index];
    for (size_t i = index; i < this->count - 1; i++)
    {
        this->data[i] = this->data[i + 1];
    }
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
void BTR_BAList_free(btr_balist_t *this)
{
    free(this->data);
}
