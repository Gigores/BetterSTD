#include "btrstd/containers/bhtable.h"
#include "_util.h"

#include "btrstd/error.h"
#include "string.h"
#include <stdio.h>
#include "stdint.h"

const int DEFAULT_BUCKETS = 16;

static double calculateLoadFactor(btr_bhtable_t *this)
{
    size_t count = 0;
    for (size_t i = 0; i < this->capacity; i++)
        count += BTR_BLList_len(&this->data[i]);
    return (double)count / this->capacity;
}
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
static void freeBucketArray(btr_bllist_t *buckets, size_t bucketsCount, btr_allocator_t *allocator)
{
    for (size_t i = 0; i < bucketsCount; i++)
    {
        BTR_BLLIST_FOREACH(&buckets[i], j)
            BTR_Allocator_deallocate(allocator, j);
        BTR_BLList_free(&buckets[i]);
    }
    BTR_Allocator_deallocate(allocator, buckets);
}
static void rehashUp(btr_bhtable_t *this)
{
    size_t newCapacity = this->capacity * 2;
    btr_bllist_t *oldData = this->data;
    size_t oldCapacity = this->capacity;
    this->data = initData(newCapacity, this->allocator);
    this->capacity *= 2;
    this->count = 0;
    for (size_t i = 0; i < oldCapacity; i++) {
        BTR_BLLIST_FOREACH(&oldData[i], j)
            BTR_BHTable_put(this, ((btr_key_value_t *)j)->key, ((btr_key_value_t *)j)->value);
    }
    freeBucketArray(oldData, oldCapacity, this->allocator);
}
static size_t getBucketIndex(const btr_bhtable_t *this, const void *key)
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

btr_hash_t BTR_hashInt32(const void *ptr)
{
    uint64_t x = *(const uint32_t *)ptr;

    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;

    return (btr_hash_t)x;
}
bool BTR_compareInt32(const void *a, const void *b)
{
    return *(int *)a == *(int *)b;
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
    btr_bllist_t *bucket = &this->data[getBucketIndex(this, key)];
    BTR_BLLIST_FOREACH(bucket, i)
        if (this->compare(((btr_key_value_t *)i)->key, key)) {
            ((btr_key_value_t *)i)->value = (void *)value;
            return;
        }
    btr_key_value_t *pair = BTR_expect(
        BTR_Allocator_allocate(this->allocator, sizeof(btr_key_value_t)),
        "Allocation failed"
    );
    pair->key   = (void *)key;
    pair->value = (void *)value;
    BTR_BLList_append(bucket, (void *)pair);
    this->count++;
    if (calculateLoadFactor(this) > 1.5)
        rehashUp(this);
}
btr_container_ptr_result_t BTR_BHTable_get(const btr_bhtable_t *this, const void *key)
{
    BTR_BLLIST_FOREACH(&this->data[getBucketIndex(this, key)], i)
        if (this->compare(key, ((btr_key_value_t *)i)->key))
            BTR_Ok(btr_container_ptr_result_t, ((btr_key_value_t *)i)->value);
    BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_NOT_FOUND);
}
btr_container_ptr_result_t BTR_BHTable_pop(btr_bhtable_t *this, const void *key)
{
    btr_bllist_t *list = &this->data[getBucketIndex(this, key)];
    long index = -1;
    BTR_BLLIST_ENUMERATE(list, i, n)
        if (this->compare(key, ((btr_key_value_t *)i)->key)) {
            index = n;
            break;
        }
    if (index == -1)
        BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_NOT_FOUND);
    btr_key_value_t *popped = BTR_unwrap(BTR_BLList_pop(list, (size_t)index));
    void *result = popped->value;
    BTR_Allocator_deallocate(this->allocator, popped);
    this->count--;
    BTR_Ok(btr_container_ptr_result_t, result);
}
bool BTR_BHTable_contains(const btr_bhtable_t *this, const void *key)
{
    BTR_BLLIST_FOREACH(&this->data[getBucketIndex(this, key)], i)
        if (this->compare(key, ((btr_key_value_t *)i)->key))
            return true;
    return false;
}
size_t BTR_BHTable_len(const btr_bhtable_t *this)
{
    return this->count;
}
void BTR_BHTable_clear(btr_bhtable_t *this)
{
    BTR_BHTable_free(this);
    btr_bllist_t *newData = initData(DEFAULT_BUCKETS, this->allocator);
    this->data = newData;
    this->capacity = DEFAULT_BUCKETS;
    this->count = 0;
}
btr_bllist_t BTR_BHTable_keys(const btr_bhtable_t *this)
{
    btr_bllist_t keys = BTR_BLList_make(this->allocator);
    for (size_t i = 0; i < this->capacity; i++) {
        BTR_BLLIST_FOREACH(&this->data[i], j)
            BTR_BLList_append(&keys, ((btr_key_value_t *)j)->key);
    }
    return keys;
}
void BTR_BHTable_free(btr_bhtable_t *this)
{
    freeBucketArray(this->data, this->capacity, this->allocator);
}
