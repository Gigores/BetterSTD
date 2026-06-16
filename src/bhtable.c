#include "btrstd/containers/bhtable.h"
#include "_util.h"

#include "btrstd/error.h"
#include "btrstd/logger.h"
#include "string.h"
#include <stdio.h>

const int DEFAULT_BUCKETS = 16;

static btr_bllist_t *initData(size_t capacity, btr_allocator_t *allocator)
{
    btr_bllist_t *data = BTR_expect(
        BTR_Allocator_allocate(allocator, sizeof(btr_bllist_t) * capacity),
        "Allocation failed"
    );
    for (size_t i = 0; i < capacity; i++)
        data[i] = BTR_BLList_make(allocator);
    return data;
}
static size_t getBucketIndex(btr_bhtable_t *this, const void *key)
{
    btr_hash_t hash = this->hash(key);
    return hash % this->capacity;
}

btr_hash_t BTR_hashCString(const void *string)
{
    const char *str = string;
    btr_hash_t hash = 14695981039346656037ull;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 1099511628211ull;
    }
    return hash;
}
bool BTR_compareCString(const void *a, const void *b)
{
    printf("%s - %s\n", (const char *)a, (const char *)b);
    return !strcmp((const char *)a, (const char *)b);
}

btr_bhtable_t BTR_BHTable_make(
    btr_hash_t (*hash)(const void *),
    bool (*compare)(const void *, const void *),
    btr_allocator_t *allocator
) {
    btr_allocator_t *theAllocator = getAllocator(allocator);
    btr_bllist_t *data = initData(DEFAULT_BUCKETS, theAllocator);
    return (btr_bhtable_t) {
        .data = data,
        .capacity = DEFAULT_BUCKETS,
        .hash = hash,
        .compare = compare,
        .allocator = theAllocator,
    };
}
void BTR_BHTable_put(btr_bhtable_t *this, const void *key, const void *value)
{
    btr_key_value_t *pair = BTR_expect(
        BTR_Allocator_allocate(this->allocator, sizeof(btr_key_value_t)),
        "Allocation failed"
    );
    pair->key   = (void *)key;
    pair->value = (void *)value;
    BTR_BLList_append(&this->data[getBucketIndex(this, key)], (void *)pair);
}
btr_container_ptr_result_t BTR_BHTable_get(btr_bhtable_t *this, const void *key)
{
    BTR_BLLIST_FOREACH(&this->data[getBucketIndex(this, key)], i)
        if (this->compare(key, ((btr_key_value_t *)i)->key))
            BTR_Ok(btr_container_ptr_result_t, ((btr_key_value_t *)i)->value);
    BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_NOT_FOUND);
}
void BTR_BHTable_free(btr_bhtable_t *this)
{
    for (size_t i = 0; i < this->capacity; i++)
    {
        BTR_BLLIST_FOREACH(&this->data[i], j)
            BTR_Allocator_deallocate(this->allocator, j);
        BTR_BLList_free(&this->data[i]);
    }
    BTR_Allocator_deallocate(this->allocator, this->data);
}
