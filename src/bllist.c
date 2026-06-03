#include "btrstd/bllist.h"

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

bllist_t BLList_make(void *items[], size_t itemCount)
{
    bllist_t list = {0};
    for (size_t i = 0; i < itemCount; i++)
        BLList_append(&list, items[i]);
    return list;
}
bllist_t BLList_clone(const bllist_t *list)
{
    if (!list) return (bllist_t) {0};
    bllist_t newList = {0};
    BLLIST_FOREACH(list, i) {
        BLList_append(&newList, i);
    }
    return newList;
}
void BLList_append(bllist_t *this, void *data)
{
    if (!this || !data) return;
    bllist_node_t **currNode = &this->head;
    while (*currNode)
        currNode = &(*currNode)->next;
    *currNode = malloc(sizeof(bllist_node_t));
    if (!*currNode) return;
    (*currNode)->payload = data;
    (*currNode)->next = NULL;
    this->size++;
}
void BLList_prepend(bllist_t *this, void *data)
{
    if (!this || !data) return;
    bllist_node_t *oldFirst = this->head;
    bllist_node_t *newFirst = malloc(sizeof(bllist_node_t));
    if (!newFirst) return;
    newFirst->payload = data;
    newFirst->next = oldFirst;
    this->head = newFirst;
    this->size++;
}
void BLList_insert(bllist_t *this, void *data, long index)
{
    if (!this || !data) return;
    if (index < 0) index = (long)this->size + index;
    if (index < 0 || (size_t)index > this->size) return;
    if (index == 0) {
        BLList_prepend(this, data);
        return;
    }
    bllist_node_t *currNode = this->head;
    for (size_t i = 0; i < (size_t) index - 1; i++) {
        if (!currNode) return;
        currNode = currNode->next;
    }
    if (!currNode) return;
    bllist_node_t *newNode = malloc(sizeof(bllist_node_t));
    if (!newNode) return;
    newNode->next = currNode->next;
    newNode->payload = data;
    currNode->next = newNode;
    this->size++;
}
void *BLList_pop(bllist_t *this, long index)
{
    if (!this) return NULL;
    if (!this->head)
        return NULL;
    if (index < 0) index = this->size + index;
    if (index == 0) {
        bllist_node_t *connectTo = this->head->next;
        void *data = this->head->payload;
        free(this->head);
        this->head = connectTo;
        this->size--;
        return data;
    } else {
        bllist_node_t **currNode = &this->head;
        for (long currIndex = 0; currIndex != index - 1 && *currNode != NULL; currIndex++)
            currNode = &(*currNode)->next;
        if (!*currNode) return NULL;
        if (!(*currNode)->next) return NULL;
        bllist_node_t *connectTo = (*currNode)->next->next;
        void *data = (*currNode)->next->payload;
        free((*currNode)->next);
        (*currNode)->next = connectTo;
        this->size--;
        return data;
    }
}
void *BLList_get(const bllist_t *this, long index)
{
    if (!this) return NULL;
    bllist_node_t *currNode = this->head;
    if (index < 0) index = this->size + index;
    for (long currIndex = 0; currIndex != index && currNode != NULL; currIndex++)
        currNode = currNode->next;
    if (!currNode) return NULL;
    return currNode->payload;
}
void *BLList_first(const bllist_t *this)
{
    if (!this) return NULL;
    if (this->size == 0) return NULL;
    return this->head->payload;
}
void *BLList_last(const bllist_t *this)
{
    if (!this) return NULL;
    if (this->size == 0) return NULL;
    bllist_node_t *currNode = this->head;
    while (currNode->next)
        currNode = currNode->next;
    return currNode->payload;
}
long BLList_indexOf(bllist_t *this, void *value, bool (*cmp)(const void *, const void *))
{
    if (!this) return -1;
    BLLIST_ENUMERATE(this, i, n)
        if(cmp(i, value)) return n;
    return -1;
}
size_t BLList_len(const bllist_t *this)
{
    if (!this) return -1;
    return this->size;
}
bool BLList_isEmpty(const bllist_t *this)
{
    if (!this) return false;
    return this->size == 0;
}
void BLList_reverse(bllist_t *this)
{
    if (!this) return;
    bllist_node_t *prev = NULL;
    bllist_node_t *curr = this->head;
    bllist_node_t *next = NULL;
    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    this->head = prev;
}
void BLList_free(bllist_t *this)
{
    if (!this) return;
    while (this->head)
    {
        bllist_node_t *next = this->head->next;
        free(this->head);
        this->head = next;
    }
    this->size = 0;
}
void BLList_clear(bllist_t *this)
{
    if (!this) return;
    BLList_free(this);
}
