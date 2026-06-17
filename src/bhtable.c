#include "btrstd/containers/bhtable.h"
#include "_util.h"

#include "btrstd/error.h"
#include "string.h"
#include <stdio.h>
#include "stdint.h"

const int DEFAULT_BUCKETS = 16;

static double calculateLoadFactor(btr_bhtable_t *this)
{
    return (double)this->count / this->capacity;
}
static btr_table_entry_t **initData(size_t capacity, btr_allocator_t *allocator)
{
    btr_table_entry_t **data = BTR_expect(
        BTR_Allocator_allocate(allocator, sizeof(btr_table_entry_t) * capacity),
        "Allocation failed"
    );
    return data;
}
static void freeBucketArray(btr_table_entry_t **buckets, size_t bucketsCount, btr_allocator_t *allocator)
{
    for (size_t i = 0; i < bucketsCount; i++)
    {
        btr_table_entry_t *current = buckets[i];
        while (current)
        {
            btr_table_entry_t *next = current->next;
            BTR_Allocator_deallocate(allocator, current);
            current = next;
        }
    }
    BTR_Allocator_deallocate(allocator, buckets);
}
static void rehashUp(btr_bhtable_t *this)
{
    size_t newCapacity = this->capacity * 2;
    btr_table_entry_t **oldData = this->data;
    size_t oldCapacity = this->capacity;
    this->data = initData(newCapacity, this->allocator);
    this->capacity *= 2;
    this->count = 0;
    for (size_t i = 0; i < oldCapacity; i++)
        for (btr_table_entry_t *current = oldData[i]; current; current = current->next)
            BTR_BHTable_put(this, current->key, current->value);
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
    btr_table_entry_t **data = initData(DEFAULT_BUCKETS, theAllocator);
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
    btr_table_entry_t **first = &this->data[getBucketIndex(this, key)];
    for (btr_table_entry_t *current = *first; current; current = current->next)
        if (this->compare(current->key, key)) {
            current->key = (void *)key;
            return;
        }
    btr_table_entry_t *pair = BTR_expect(
        BTR_Allocator_allocate(this->allocator, sizeof(btr_table_entry_t)),
        "Allocation failed"
    );
    pair->key   = (void *)key;
    pair->value = (void *)value;
    pair->next  = *first;
    *first = pair;
    this->count++;
    if (calculateLoadFactor(this) > 1.5)
        rehashUp(this);
}
btr_container_ptr_result_t BTR_BHTable_get(const btr_bhtable_t *this, const void *key)
{
    for (
        btr_table_entry_t *current = this->data[getBucketIndex(this, key)];
        current;
        current = current->next
    ) if (this->compare(current->key, key))
        BTR_Ok(btr_container_ptr_result_t, current->value);
    BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_NOT_FOUND);
}
btr_container_ptr_result_t BTR_BHTable_pop(btr_bhtable_t *this, const void *key)
{
    size_t bucket = getBucketIndex(this, key);
    btr_table_entry_t *current = this->data[bucket];
    btr_table_entry_t *prev = NULL;
    while (current)
    {
        if (this->compare(current->key, key))
        {
            if (prev)
                prev->next = current->next;
            else
                this->data[bucket] = current->next;
            void *result = current->value;
            BTR_Allocator_deallocate(this->allocator, current);
            this->count--;
            BTR_Ok(btr_container_ptr_result_t, result);
        }
        prev = current;
        current = current->next;
    }
    BTR_Err(btr_container_ptr_result_t, BTR_CONTAINER_ERR_NOT_FOUND);
}
bool BTR_BHTable_contains(const btr_bhtable_t *this, const void *key)
{
    for (
        btr_table_entry_t *current = this->data[getBucketIndex(this, key)];
        current;
        current = current->next
    ) if (this->compare(current->key, key))
        if (this->compare(current->key, key))
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
    btr_table_entry_t **newData = initData(DEFAULT_BUCKETS, this->allocator);
    this->data = newData;
    this->capacity = DEFAULT_BUCKETS;
    this->count = 0;
}
btr_bllist_t BTR_BHTable_keys(const btr_bhtable_t *this)
{
    btr_bllist_t keys = BTR_BLList_make(this->allocator);
    for (size_t i = 0; i < this->capacity; i++)
        for (
            btr_table_entry_t *current = this->data[i];
            current;
            current = current->next
        ) BTR_BLList_append(&keys, current->key);
    return keys;
}
void BTR_BHTable_free(btr_bhtable_t *this)
{
    freeBucketArray(this->data, this->capacity, this->allocator);
}
