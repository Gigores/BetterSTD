#include "btrstd/containers/borrowing_linked_list.h"
#include "_util.h"

#include "stdlib.h"


btr_bllist_s BTR_BLList_makeFrom(void *items[], size_t itemCount, btr_allocator_s *allocator)
{
    btr_bllist_s list = {
        .allocator = getAllocator(allocator)
    };
    for (size_t i = 0; i < itemCount; i++)
        BTR_BLList_append(&list, items[i]);
    return list;
}
btr_bllist_s BTR_BLList_make(btr_allocator_s *allocator)
{
    return (btr_bllist_s) {
        .allocator = getAllocator(allocator)
    };
}
btr_bllist_s BTR_BLList_clone(const btr_bllist_s *list, btr_allocator_s *allocator)
{
    BTR_panicIf(!list, "`list` is null");
    btr_bllist_s newList = {
        .allocator = getAllocator(allocator)
    };
    BTR_BLLIST_FOREACH(list, i) {
        BTR_BLList_append(&newList, i);
    }
    return newList;
}
void BTR_BLList_append(btr_bllist_s *this, void *data)
{
    BTR_panicIf(!this, "`this` is null");
    btr_bllist_node_s **currNode = &this->head;
    while (*currNode)
        currNode = &(*currNode)->next;
    *currNode = BTR_expect(
        BTR_Allocator_allocate(this->allocator, sizeof(btr_bllist_node_s)),
        "Allocation failed"
    );
    (*currNode)->payload = data;
    (*currNode)->next = NULL;
    this->size++;
}
void BTR_BLList_prepend(btr_bllist_s *this, void *data)
{
    BTR_panicIf(!this, "`this` is null");
    btr_bllist_node_s *oldFirst = this->head;
    btr_bllist_node_s *newFirst = BTR_expect(
        BTR_Allocator_allocate(this->allocator, sizeof(btr_bllist_node_s)),
        "Allocation failed"
    );
    newFirst->payload = data;
    newFirst->next = oldFirst;
    this->head = newFirst;
    this->size++;
}
void BTR_BLList_insert(btr_bllist_s *this, void *data, long index)
{
    BTR_panicIf(!this, "`this` is null");
    if (index < 0) index = (long)this->size + index;
    BTR_panicIf(index < 0 || (size_t)index > this->size, "index out of bounds");
    if (index == 0) {
        BTR_BLList_prepend(this, data);
        return;
    }
    btr_bllist_node_s *currNode = this->head;
    for (size_t i = 0; i < (size_t) index - 1; i++)
        currNode = currNode->next;
    BTR_panicIf(!currNode, "unexpected null node");
    btr_bllist_node_s *newNode = BTR_expect(
        BTR_Allocator_allocate(this->allocator, sizeof(btr_bllist_node_s)),
        "Allocation failed"
    );
    newNode->next = currNode->next;
    newNode->payload = data;
    currNode->next = newNode;
    this->size++;
}
btr_container_ptr_r BTR_BLList_pop(btr_bllist_s *this, long index)
{
    BTR_panicIf(!this, "`this` is null");
    BTR_panicIf(!this->head, "`this` is invalid");
    if (index < 0) index = this->size + index;
    if (index == 0) {
        btr_bllist_node_s *connectTo = this->head->next;
        void *data = this->head->payload;
        BTR_Allocator_deallocate(this->allocator, this->head);
        this->head = connectTo;
        this->size--;
        BTR_Ok(btr_container_ptr_r, data);
    } else {
        btr_bllist_node_s **currNode = &this->head;
        for (long currIndex = 0; currIndex != index - 1 && *currNode != NULL; currIndex++)
            currNode = &(*currNode)->next;
        if (!*currNode)
            BTR_Err(btr_container_ptr_r, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
        if (!(*currNode)->next)
            BTR_Err(btr_container_ptr_r, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
        btr_bllist_node_s *connectTo = (*currNode)->next->next;
        void *data = (*currNode)->next->payload;
        BTR_Allocator_deallocate(this->allocator, (*currNode)->next);
        (*currNode)->next = connectTo;
        this->size--;
        BTR_Ok(btr_container_ptr_r, data);
    }
}
btr_container_ptr_r BTR_BLList_get(const btr_bllist_s *this, long index)
{
    BTR_panicIf(!this, "`this` is null");
    btr_bllist_node_s *currNode = this->head;
    if (index < 0) index = this->size + index;
    for (long currIndex = 0; currIndex != index && currNode != NULL; currIndex++)
        currNode = currNode->next;
    if (!currNode)
        BTR_Err(btr_container_ptr_r, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_container_ptr_r, currNode->payload);
}
void BTR_BLList_set(btr_bllist_s *this, void *data, long index)
{
    BTR_panicIf(!this, "`this` is null");
    btr_bllist_node_s *currNode = this->head;
    if (index < 0) index = this->size + index;
    for (long currIndex = 0; currIndex != index && currNode != NULL; currIndex++)
        currNode = currNode->next;
    BTR_panicIf(!currNode, "index out of bounds");
    currNode->payload = data;
}
btr_container_ptr_r BTR_BLList_first(const btr_bllist_s *this)
{
    BTR_panicIf(!this, "`this` is null");
    if (this->size == 0)
        BTR_Err(btr_container_ptr_r, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_container_ptr_r, this->head->payload);
}
btr_container_ptr_r BTR_BLList_last(const btr_bllist_s *this)
{
    BTR_panicIf(!this, "`this` is null");
    if (this->size == 0)
        BTR_Err(btr_container_ptr_r, BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    btr_bllist_node_s *currNode = this->head;
    while (currNode->next)
        currNode = currNode->next;
    BTR_Ok(btr_container_ptr_r, currNode->payload);
}
btr_container_idx_r BTR_BLList_indexOf(
    btr_bllist_s *this,
    void *value,
    bool (*cmp)(const void *, const void *)
) {
    BTR_panicIf(!this, "`this` is null");
    BTR_BLLIST_ENUMERATE(this, i, n)
        if(cmp(i, value))
            BTR_Ok(btr_container_idx_r, n);
    BTR_Err(btr_container_idx_r, BTR_CONTAINER_ERR_NOT_FOUND);
}
size_t BTR_BLList_len(const btr_bllist_s *this)
{
    BTR_panicIf(!this, "`this` is null");
    return this->size;
}
bool BTR_BLList_isEmpty(const btr_bllist_s *this)
{
    BTR_panicIf(!this, "`this` is null");
    return this->size == 0;
}
void BTR_BLList_reverse(btr_bllist_s *this)
{
    BTR_panicIf(!this, "`this` is null");
    btr_bllist_node_s *prev = NULL;
    btr_bllist_node_s *curr = this->head;
    btr_bllist_node_s *next = NULL;
    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    this->head = prev;
}
void BTR_BLList_free(btr_bllist_s *this)
{
    BTR_panicIf(!this, "`this` is null");
    while (this->head)
    {
        btr_bllist_node_s *next = this->head->next;
        BTR_Allocator_deallocate(this->allocator, this->head);
        this->head = next;
    }
    this->size = 0;
}
void BTR_BLList_clear(btr_bllist_s *this)
{
    BTR_panicIf(!this, "`this` is null");
    BTR_BLList_free(this);
}
