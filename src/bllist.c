#include "btrstd/containers/bllist.h"

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

btr_bllist_t BTR_BLList_make(void *items[], size_t itemCount)
{
    btr_bllist_t list = {0};
    for (size_t i = 0; i < itemCount; i++)
        BTR_BLList_append(&list, items[i]);
    return list;
}
btr_bllist_t BTR_BLList_clone(const btr_bllist_t *list)
{
    if (!list) return (btr_bllist_t) {0};
    btr_bllist_t newList = {0};
    BTR_BLLIST_FOREACH(list, i) {
        BTR_BLList_append(&newList, i);
    }
    return newList;
}
void BTR_BLList_append(btr_bllist_t *this, void *data)
{
    if (!this || !data) return;
    btr_bllist_node_t **currNode = &this->head;
    while (*currNode)
        currNode = &(*currNode)->next;
    *currNode = malloc(sizeof(btr_bllist_node_t));
    if (!*currNode) return;
    (*currNode)->payload = data;
    (*currNode)->next = NULL;
    this->size++;
}
void BTR_BLList_prepend(btr_bllist_t *this, void *data)
{
    if (!this || !data) return;
    btr_bllist_node_t *oldFirst = this->head;
    btr_bllist_node_t *newFirst = malloc(sizeof(btr_bllist_node_t));
    if (!newFirst) return;
    newFirst->payload = data;
    newFirst->next = oldFirst;
    this->head = newFirst;
    this->size++;
}
void BTR_BLList_insert(btr_bllist_t *this, void *data, long index)
{
    if (!this || !data) return;
    if (index < 0) index = (long)this->size + index;
    if (index < 0 || (size_t)index > this->size) return;
    if (index == 0) {
        BTR_BLList_prepend(this, data);
        return;
    }
    btr_bllist_node_t *currNode = this->head;
    for (size_t i = 0; i < (size_t) index - 1; i++) {
        if (!currNode) return;
        currNode = currNode->next;
    }
    if (!currNode) return;
    btr_bllist_node_t *newNode = malloc(sizeof(btr_bllist_node_t));
    if (!newNode) return;
    newNode->next = currNode->next;
    newNode->payload = data;
    currNode->next = newNode;
    this->size++;
}
void *BTR_BLList_pop(btr_bllist_t *this, long index)
{
    if (!this) return NULL;
    if (!this->head)
        return NULL;
    if (index < 0) index = this->size + index;
    if (index == 0) {
        btr_bllist_node_t *connectTo = this->head->next;
        void *data = this->head->payload;
        free(this->head);
        this->head = connectTo;
        this->size--;
        return data;
    } else {
        btr_bllist_node_t **currNode = &this->head;
        for (long currIndex = 0; currIndex != index - 1 && *currNode != NULL; currIndex++)
            currNode = &(*currNode)->next;
        if (!*currNode) return NULL;
        if (!(*currNode)->next) return NULL;
        btr_bllist_node_t *connectTo = (*currNode)->next->next;
        void *data = (*currNode)->next->payload;
        free((*currNode)->next);
        (*currNode)->next = connectTo;
        this->size--;
        return data;
    }
}
void *BTR_BLList_get(const btr_bllist_t *this, long index)
{
    if (!this) return NULL;
    btr_bllist_node_t *currNode = this->head;
    if (index < 0) index = this->size + index;
    for (long currIndex = 0; currIndex != index && currNode != NULL; currIndex++)
        currNode = currNode->next;
    if (!currNode) return NULL;
    return currNode->payload;
}
void *BTR_BLList_first(const btr_bllist_t *this)
{
    if (!this) return NULL;
    if (this->size == 0) return NULL;
    return this->head->payload;
}
void *BTR_BLList_last(const btr_bllist_t *this)
{
    if (!this) return NULL;
    if (this->size == 0) return NULL;
    btr_bllist_node_t *currNode = this->head;
    while (currNode->next)
        currNode = currNode->next;
    return currNode->payload;
}
long BTR_BLList_indexOf(btr_bllist_t *this, void *value, bool (*cmp)(const void *, const void *))
{
    if (!this) return -1;
    BTR_BLLIST_ENUMERATE(this, i, n)
        if(cmp(i, value)) return n;
    return -1;
}
size_t BTR_BLList_len(const btr_bllist_t *this)
{
    if (!this) return 0;
    return this->size;
}
bool BTR_BLList_isEmpty(const btr_bllist_t *this)
{
    if (!this) return false;
    return this->size == 0;
}
void BTR_BLList_reverse(btr_bllist_t *this)
{
    if (!this) return;
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
    if (!this) return;
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
    if (!this) return;
    BTR_BLList_free(this);
}
