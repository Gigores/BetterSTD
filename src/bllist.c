#include "btrstd/bllist.h"
#include "stdio.h"

btr_bllist_t BTR_BLList_new(void *items[], size_t count)
{
}
btr_bllist_t BTR_BLList_clone(const btr_bllist_t *list);
void BTR_BLList_append(btr_bllist_t *this, void *data)
{
    btr_bllist_node_t **currNode = &this->head;
    while (*currNode)
        currNode = &(*currNode)->next;
    *currNode = malloc(sizeof(btr_bllist_node_t));
    if (!*currNode) return;
    (*currNode)->payload = data;
    (*currNode)->next = NULL;
    this->size++;
}
void BTR_BLList_prepend(btr_bllist_t *, void *data);
void BTR_BLList_insert(btr_bllist_t *, void *data, long index);
void *BTR_BLList_pop(btr_bllist_t *this, long index)
{
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
    btr_bllist_node_t *currNode = this->head;
    if (index < 0) index = this->size + index;
    for (long currIndex = 0; currIndex != index && currNode != NULL; currIndex++)
        currNode = currNode->next;
    if (!currNode) return NULL;
    return currNode->payload;
}
void *BTR_BLList_first(const btr_bllist_t *);
void *BTR_BLList_last(const btr_bllist_t *);
long BTR_BLList_indexOf(btr_bllist_t *list, void *value, int (*cmp)(const void *, const void *));
size_t BTR_BLList_len(const btr_bllist_t *);
bool BTR_BLList_isEmpty(const btr_bllist_t *);
void BTR_BLList_reverse(btr_bllist_t *);
void BTR_BLList_free(btr_bllist_t *this)
{
    while (this->head) {
        btr_bllist_node_t *next = this->head->next;
        free(this->head);
        this->head = next;
    }
    this->size = 0;
}
void BTR_BLList_clear(btr_bllist_t *);
