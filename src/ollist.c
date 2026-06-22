#include "btrstd/containers/owning_linked_list.h"
#include "_util.c"
#include <string.h>

static void *allocateData(btr_ollist_s *this)
{
    return BTR_expect(
        BTR_Allocator_allocate(this->allocator, this->itemSize),
        "Allocation failed"
    );
}

btr_ollist_s BTR_OLList_makeFrom(
    void *items,
    size_t itemCount,
    size_t itemSize,
    btr_allocator_s *allocator
) {
    btr_ollist_s list = BTR_OLList_make(itemSize, allocator);
    for (size_t i = 0; i < itemCount; i++)
        memcpy(BTR_OLList_append(&list), items + i * itemSize, itemSize);
    return list;
}
btr_ollist_s BTR_OLList_make(
    size_t itemSize,
    btr_allocator_s *allocator
) {
    btr_allocator_s *theAllocator = getAllocator(allocator);
    btr_bllist_s underlying = BTR_BLList_make(allocator);
    return (btr_ollist_s)
    {
        .underlying = underlying,
        .allocator = theAllocator,
        .itemSize = itemSize,
    };
}
btr_ollist_s BTR_OLList_clone(const btr_ollist_s *list, btr_allocator_s *allocator)
{
    btr_ollist_s newList = BTR_OLList_make(list->itemSize, allocator);
    BTR_OLLIST_FOREACH(list, i)
        memcpy(BTR_OLList_append(&newList), i, list->itemSize);
    return newList;
}
void *BTR_OLList_append(btr_ollist_s *this)
{
    void *data = allocateData(this);
    BTR_BLList_append(&this->underlying, data);
    return data;
}
void *BTR_OLList_prepend(btr_ollist_s *this)
{
    void *data = allocateData(this);
    BTR_BLList_prepend(&this->underlying, data);
    return data;
}
void *BTR_OLList_insert(btr_ollist_s *this, long index)
{
    void *data = allocateData(this);
    BTR_BLList_insert(&this->underlying, data, index);
    return data;
}
void BTR_OLList_pop(btr_ollist_s *this, long index, void *buffer)
{
    void *popped = BTR_unwrap(BTR_BLList_pop(&this->underlying, index));
    if (buffer)
        memcpy(buffer, popped, this->itemSize);
    BTR_Allocator_deallocate(this->allocator, popped);
}
btr_container_ptr_r BTR_OLList_get(const btr_ollist_s *this, long index)
{
    return BTR_BLList_get(&this->underlying, index);
}
btr_container_ptr_r BTR_OLList_first(const btr_ollist_s *this)
{
    return BTR_BLList_first(&this->underlying);
}
btr_container_ptr_r BTR_OLList_last(const btr_ollist_s *this)
{
    return BTR_BLList_last(&this->underlying);
}
btr_container_idx_r BTR_OLList_indexOf
    (btr_ollist_s *list, void *value, bool (*cmp)(const void *, const void *))
{
    return BTR_BLList_indexOf(&list->underlying, value, cmp);
}
size_t BTR_OLList_len(const btr_ollist_s *this)
{
    return BTR_BLList_len(&this->underlying);
}
bool BTR_OLList_isEmpty(const btr_ollist_s *this)
{
    return BTR_BLList_isEmpty(&this->underlying);
}
void BTR_OLList_reverse(btr_ollist_s *this)
{
    BTR_BLList_reverse(&this->underlying);
}
void BTR_OLList_free(btr_ollist_s *this)
{
    BTR_BLLIST_FOREACH(&this->underlying, i)
        BTR_Allocator_deallocate(this->allocator, i);
    BTR_BLList_free(&this->underlying);
}
void BTR_OLList_clear(btr_ollist_s *this)
{
    BTR_BLLIST_FOREACH(&this->underlying, i)
        BTR_Allocator_deallocate(this->allocator, i);
    BTR_BLList_clear(&this->underlying);
}
