#include "btrstd/containers/balist.h"

#include "stdlib.h"
#include "string.h"

#include "stdio.h"

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
void *BTR_BAList_get(const btr_balist_t *this, long index)
{
    if (!this) return NULL;
    if (index < 0) index = this->count + index;
    return this->data[index];
}
void BTR_BAList_free(btr_balist_t *this)
{
    free(this->data);
}
