#include "btrstd/containers/bllist.h"

#include "stdlib.h"

btr_bllist_t BTR_BLList_make(void *items[], size_t itemCount)
{
    btr_bllist_t list = {0};
    for (size_t i = 0; i < itemCount; i++)
        BTR_BLList_append(&list, items[i]);
    return list;
}
btr_bllist_t BTR_BLList_clone(const btr_bllist_t *list)
{
    BTR_panicIf(!list, "`list` is null");
    btr_bllist_t newList = {0};
    BTR_BLLIST_FOREACH(list, i) {
        BTR_BLList_append(&newList, i);
    }
    return newList;
}
void BTR_BLList_append(btr_bllist_t *this, void *data)
{
    BTR_panicIf(!this || !data, "`this` is null");
    btr_bllist_node_t **currNode = &this->head;
    while (*currNode)
        currNode = &(*currNode)->next;
    *currNode = malloc(sizeof(btr_bllist_node_t));
    BTR_panicIf(!*currNode, "allocation failed");
    (*currNode)->payload = data;
    (*currNode)->next = NULL;
    this->size++;
}
void BTR_BLList_prepend(btr_bllist_t *this, void *data)
{
    BTR_panicIf(!this || !data, "`this` is null");
    btr_bllist_node_t *oldFirst = this->head;
    btr_bllist_node_t *newFirst = malloc(sizeof(btr_bllist_node_t));
    BTR_panicIf(!newFirst, "allocation failed");
    newFirst->payload = data;
    newFirst->next = oldFirst;
    this->head = newFirst;
    this->size++;
}
void BTR_BLList_insert(btr_bllist_t *this, void *data, long index)
{
    BTR_panicIf(!this || !data, "`this` is null");
    if (index < 0) index = (long)this->size + index;
    BTR_panicIf(index < 0 || (size_t)index > this->size, "index out of bounds");
    if (index == 0) {
        BTR_BLList_prepend(this, data);
        return;
    }
    btr_bllist_node_t *currNode = this->head;
    for (size_t i = 0; i < (size_t) index - 1; i++)
        currNode = currNode->next;
    BTR_panicIf(!currNode, "unexpected null node");
    btr_bllist_node_t *newNode = malloc(sizeof(btr_bllist_node_t));
    BTR_panicIf(!newNode, "allocation failed");
    newNode->next = currNode->next;
    newNode->payload = data;
    currNode->next = newNode;
    this->size++;
}
btr_bllist_ptr_result_t BTR_BLList_pop(btr_bllist_t *this, long index)
{
    BTR_panicIf(!this, "`this` is null");
    BTR_panicIf(!this->head, "`this` is invalid");
    if (index < 0) index = this->size + index;
    if (index == 0) {
        btr_bllist_node_t *connectTo = this->head->next;
        void *data = this->head->payload;
        free(this->head);
        this->head = connectTo;
        this->size--;
        BTR_Ok(btr_bllist_ptr_result_t, data);
    } else {
        btr_bllist_node_t **currNode = &this->head;
        for (long currIndex = 0; currIndex != index - 1 && *currNode != NULL; currIndex++)
            currNode = &(*currNode)->next;
        if (!*currNode)
            BTR_Err(btr_bllist_ptr_result_t, BTR_BLLIST_ERR_OUT_OF_BOUNDS);
        if (!(*currNode)->next)
            BTR_Err(btr_bllist_ptr_result_t, BTR_BLLIST_ERR_OUT_OF_BOUNDS);
        btr_bllist_node_t *connectTo = (*currNode)->next->next;
        void *data = (*currNode)->next->payload;
        free((*currNode)->next);
        (*currNode)->next = connectTo;
        this->size--;
        BTR_Ok(btr_bllist_ptr_result_t, data);
    }
}
btr_bllist_ptr_result_t BTR_BLList_get(const btr_bllist_t *this, long index)
{
    BTR_panicIf(!this, "`this` is null");
    btr_bllist_node_t *currNode = this->head;
    if (index < 0) index = this->size + index;
    for (long currIndex = 0; currIndex != index && currNode != NULL; currIndex++)
        currNode = currNode->next;
    if (!currNode)
        BTR_Err(btr_bllist_ptr_result_t, BTR_BLLIST_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_bllist_ptr_result_t, currNode->payload);
}
btr_bllist_ptr_result_t BTR_BLList_first(const btr_bllist_t *this)
{
    BTR_panicIf(!this, "`this` is null");
    if (this->size == 0)
        BTR_Err(btr_bllist_ptr_result_t, BTR_BLLIST_ERR_OUT_OF_BOUNDS);
    BTR_Ok(btr_bllist_ptr_result_t, this->head->payload);
}
btr_bllist_ptr_result_t BTR_BLList_last(const btr_bllist_t *this)
{
    BTR_panicIf(!this, "`this` is null");
    if (this->size == 0)
        BTR_Err(btr_bllist_ptr_result_t, BTR_BLLIST_ERR_OUT_OF_BOUNDS);
    btr_bllist_node_t *currNode = this->head;
    while (currNode->next)
        currNode = currNode->next;
    BTR_Ok(btr_bllist_ptr_result_t, currNode->payload);
}
btr_bllist_idx_result_t BTR_BLList_indexOf(btr_bllist_t *this, void *value, bool (*cmp)(const void *, const void *))
{
    BTR_panicIf(!this, "`this` is null");
    BTR_BLLIST_ENUMERATE(this, i, n)
        if(cmp(i, value))
            BTR_Ok(btr_bllist_idx_result_t, n);
    BTR_Err(btr_bllist_idx_result_t, BTR_BLLIST_ERR_NOT_FOUND);
}
size_t BTR_BLList_len(const btr_bllist_t *this)
{
    BTR_panicIf(!this, "`this` is null");
    return this->size;
}
bool BTR_BLList_isEmpty(const btr_bllist_t *this)
{
    BTR_panicIf(!this, "`this` is null");
    return this->size == 0;
}
void BTR_BLList_reverse(btr_bllist_t *this)
{
    BTR_panicIf(!this, "`this` is null");
    btr_bllist_node_t *prev = NULL;
    btr_bllist_node_t *curr = this->head;
    btr_bllist_node_t *next = NULL;
    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    this->head = prev;
}
void BTR_BLList_free(btr_bllist_t *this)
{
    BTR_panicIf(!this, "`this` is null");
    while (this->head)
    {
        btr_bllist_node_t *next = this->head->next;
        free(this->head);
        this->head = next;
    }
    this->size = 0;
}
void BTR_BLList_clear(btr_bllist_t *this)
{
    BTR_panicIf(!this, "`this` is null");
    BTR_BLList_free(this);
}
