#pragma once

#include "stddef.h"
#include "stdbool.h"

#include "btrstd/allocator.h"
#include "btrstd/containers/generic_container_data.h"

typedef struct {
    char *data;
} btr_bit_set_t;

typedef BTR_Result(bool, btr_container_error_t) btr_bit_result_t;

btr_bit_set_t BTR_BitSet_make(size_t capacity, btr_allocator_t *allocator);
btr_bit_set_t BTR_BitSet_clone(const btr_bit_set_t *bitSet, btr_allocator_t *allocator);
btr_bit_result_t BTR_BitSet_get(const btr_bit_set_t *, long index);
btr_bit_result_t BTR_BitSet_set(btr_bit_set_t *, long index, bool value);
btr_bit_result_t BTR_BitSet_flip(btr_bit_set_t *, long index);
void BTR_BitSet_free(btr_bit_set_t *);
