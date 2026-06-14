#pragma once

#include "stddef.h"
#include "stdbool.h"

#include "btrstd/allocator.h"
#include "btrstd/containers/generic_container_data.h"

// Bit Set

// Bit set is a data structure made to store booleans in a compact way.
// Normally one boolean variable consumes a whole byte and looks like this in memory:
//     00000000 → false
//     00000001 → true
// This is extremely inefficient. Bit set fixes this issue by using each bit of a byte to store values.
// For example, a bit set of size 16 will only consume 2 bytes, instead of 16 like a regular array, and looks like this in memory:
//     10110101 01001101
//     [0] → true
//     [1] → false
//     [2] → true
//     ...
//     [14] → false
//     [15] → true

// Allocator required capabilities:
//   allocate, deallocate

typedef struct {
    char *data;
    size_t bitCount;
    btr_allocator_t *allocator;
} btr_bit_set_t;

typedef BTR_Result(bool, btr_container_error_t) btr_bit_result_t;

// Creates a new bit set with a given capacity in bits.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_bit_set_t BTR_BitSet_make(size_t capacity, btr_allocator_t *allocator);
// Creates a new bit set from another bit set with the same data.
// The `allocator` parameter can be set as `NULL`, in this case it will use the global allocator.
btr_bit_set_t BTR_BitSet_clone(const btr_bit_set_t *bitSet, btr_allocator_t *allocator);
// Returns the data of the specified index of the bit set.
// Can accept negative indexes.
// Returns `BTR_ERR` with `BTR_CONTAINER_ERR_OUT_OF_BOUNDS` if the index is invalid.
btr_bit_result_t BTR_BitSet_get(const btr_bit_set_t *, long index);
// Sets the value at the given index to `true`.
// Can accept negative indexes.
void BTR_BitSet_set(btr_bit_set_t *, long index);
// Sets the value at the given index to `false`.
// Can accept negative indexes.
void BTR_BitSet_unset(btr_bit_set_t *, long index);
// Flips the value at the given index.
// Can accept negative indexes.
btr_bit_result_t BTR_BitSet_flip(btr_bit_set_t *, long index);
// Deallocates the bit set.
void BTR_BitSet_free(btr_bit_set_t *);

#ifdef BTR_NO_PREFIX

typedef btr_bit_set_t    bit_set_t;
typedef btr_bit_result_t bit_result_t;

#define BitSet_make  BTR_BitSet_make
#define BitSet_clone BTR_BitSet_clone
#define BitSet_get   BTR_BitSet_get
#define BitSet_set   BTR_BitSet_set
#define BitSet_unset BTR_BitSet_unset
#define BitSet_flip  BTR_BitSet_flip
#define BitSet_free  BTR_BitSet_free

#endif // BTR_NO_PREFIX
