#pragma once

#include "stddef.h"

#include "btrstd/containers/_generic_container_data.h"
#include "btrstd/allocator.h"

typedef struct {
    void *data;
    size_t capacity;
    size_t count;
    size_t itemSize;
} btr_oalist_t;

btr_oalist_t BTR_OAList_makeFrom(void *items[], size_t itemCount, btr_allocator_t *allocator);
btr_oalist_t BTR_OAList_make(size_t capacity, btr_allocator_t *allocator);
btr_oalist_t BTR_OAList_clone(const btr_oalist_t *list, btr_allocator_t *allocator);
void BTR_OAList_append(btr_oalist_t *, void *data);
void BTR_OAList_prepend(btr_oalist_t *, void *data);
void BTR_OAList_insert(btr_oalist_t *, void *data, long index);
btr_container_ptr_result_t BTR_OAList_pop(btr_oalist_t *, long index);
btr_container_ptr_result_t BTR_OAList_get(const btr_oalist_t *, long index);
void BTR_OAList_set(btr_oalist_t *, void *data, long index);
btr_container_ptr_result_t BTR_OAList_first(const btr_oalist_t *);
btr_container_ptr_result_t BTR_OAList_last(const btr_oalist_t *);
btr_container_idx_result_t BTR_OAList_indexOf
    (btr_oalist_t *list, void *value, bool (*cmp)(const void *, const void *));
size_t BTR_OAList_len(const btr_oalist_t *);
bool BTR_OAList_isEmpty(const btr_oalist_t *);
void BTR_OAList_reverse(btr_oalist_t *);
void BTR_OAList_free(btr_oalist_t *);
void BTR_OAList_clear(btr_oalist_t *);

// #define BTR_OALIST_FOREACH(LIST, i) \
//     void *i; \
//     for (size_t _i = 0; _i < (LIST)->count && ((i = (LIST)->data[_i]), 1); _i++)

// #define BTR_OALIST_ENUMERATE(LIST, i, n) \
//     void *i; \
//     for (size_t n = 0; n < (LIST)->count && ((i = (LIST)->data[n]), 1); n++)

// #define BTR_OALIST(...)                                     \
//     BTR_BAList_makeFrom(                                    \
//         (void *[]){ __VA_ARGS__ },                          \
//         sizeof((void *[]){ __VA_ARGS__ }) / sizeof(void *), \
//         NULL                                                \
//     )
