#pragma once

#include "btrstd/containers/bllist.h"

// Borrowing Hash Table

// Borrowing hash table is a data structure that stores every item behind a specific key.
// It is pretty fast, because the time coplexity of most essential operations is O(1),
// unlike in just a list of key-value entries which will be O(n).

typedef unsigned long long btr_hash_t;

// Pre-made functions for creating borrowing hash tables with cstring keys.
btr_hash_t BTR_hashCString(const void *);
bool BTR_compareCString(const void *, const void *);

// Pre-made functions for creating borrowing hash tables with int32 keys.
btr_hash_t BTR_hashInt32(const void *);
bool BTR_compareInt32(const void *, const void *);

typedef struct BTR_TableEntry{
    void *key, *value;
    struct BTR_TableEntry *next;
} btr_table_entry_t;

typedef struct {
    btr_table_entry_t **data;
    size_t capacity;
    size_t count;

    btr_hash_t (*hash)(const void *);
    bool (*compare)(const void *, const void *);

    btr_allocator_t *allocator;
} btr_bhtable_t;

// Creates a new empty borrowing hash table. It requires to function pointers to properly
// work with needed keys.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_bhtable_t BTR_BHTable_make(
    btr_hash_t (*hash)(const void *),
    bool (*compare)(const void *, const void *),
    btr_allocator_t *allocator
);
// Sets a key to a value in the borrowing hash table.
void BTR_BHTable_put(btr_bhtable_t *, const void *key, const void *value);
// Returns a value of the key in the borrowing hash table.
btr_container_ptr_result_t BTR_BHTable_get(const btr_bhtable_t *, const void *key);
// Returns a value of the key and removes it from the borrowing hash table.
btr_container_ptr_result_t BTR_BHTable_pop(btr_bhtable_t *, const void *key);
// Tells if the key has been set in the borrowing hash table.
bool BTR_BHTable_contains(const btr_bhtable_t *, const void *key);
// Returns the amount of keys in the borrowing hash table.
size_t BTR_BHTable_len(const btr_bhtable_t *);
// Returns a borrowing linked list containing all the keys from the
// borrowing hash table.
// You will need to free this list yourself.
btr_bllist_t BTR_BHTable_keys(const btr_bhtable_t *);
// Deallocates the entire borrowing hash table, making it unusable afterwards.
void BTR_BHTable_free(btr_bhtable_t *);
// Deallocates the entire borrowing hash table, but reinitializes it to be empty.
void BTR_BHTable_clear(btr_bhtable_t *);

#ifdef BTR_NO_PREFIX

typedef btr_hash_t hash_t;

#define hashCString BTR_hashCString
#define compareCString BTR_compareCString

#define hashInt32 BTR_hashInt32
#define compareInt32 BTR_compareInt32

typedef btr_key_value_t key_value_t;
typedef btr_bhtable_t bhtable_t

#define BHTable_make BTR_BHTable_make
#define BHTable_put BTR_BHTable_put
#define BHTable_get BTR_BHTable_get
#define BHTable_pop BTR_BHTable_pop
#define BHTable_contains BTR_BHTable_contains
#define BHTable_len BTR_BHTable_len
#define BHTable_keys BTR_BHTable_keys
#define BHTable_free BTR_BHTable_free
#define BHTable_clear BTR_BHTable_clear

#endif  // BTR_NO_PREFIX
