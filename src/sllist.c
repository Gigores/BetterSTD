#include "btrstd/sllist.h"
#include "stdio.h"

btr_sllist_t BTR_SLList_new(void *items[], size_t count)
{
}
btr_sllist_t BTR_SLList_clone(const btr_sllist_t *list);
void BTR_SLList_append(btr_sllist_t *this, void *data)
{
    btr_sllist_node_t **currNode = &this->head;
    while (*currNode)
        currNode = &(*currNode)->next;
    *currNode = malloc(sizeof(btr_sllist_node_t));
    if (!*currNode) return;
    (*currNode)->payload = data;
    (*currNode)->next = NULL;
    this->size++;
}
void BTR_SLList_prepend(btr_sllist_t *, void *data);
void BTR_SLList_insert(btr_sllist_t *, void *data, long index);
void *BTR_SLList_pop(btr_sllist_t *this, long index)
{
    if (!this->head)
        return NULL;
    if (index < 0) index = this->size + index;
    if (index == 0) {
        btr_sllist_node_t *connectTo = this->head->next;
        void *data = this->head->payload;
        free(this->head);
        this->head = connectTo;
        this->size--;
        return data;
    } else {
        btr_sllist_node_t **currNode = &this->head;
        for (long currIndex = 0; currIndex != index - 1 && *currNode != NULL; currIndex++)
            currNode = &(*currNode)->next;
        if (!*currNode) return NULL;
        if (!(*currNode)->next) return NULL;
        btr_sllist_node_t *connectTo = (*currNode)->next->next;
        void *data = (*currNode)->next->payload;
        free((*currNode)->next);
        (*currNode)->next = connectTo;
        this->size--;
        return data;
    }
}
void *BTR_SLList_get(const btr_sllist_t *this, long index)
{
    btr_sllist_node_t *currNode = this->head;
    if (index < 0) index = this->size + index;
    for (long currIndex = 0; currIndex != index && currNode != NULL; currIndex++)
        currNode = currNode->next;
    if (!currNode) return NULL;
    return currNode->payload;
}
void *BTR_SLList_first(const btr_sllist_t *);
void *BTR_SLList_last(const btr_sllist_t *);
long BTR_SLList_indexOf(btr_sllist_t *list, void *value, int (*cmp)(const void *, const void *));
size_t BTR_SLList_len(const btr_sllist_t *);
bool BTR_SLList_isEmpty(const btr_sllist_t *);
void BTR_SLList_reverse(btr_sllist_t *);
void BTR_SLList_free(btr_sllist_t *this)
{
    while (this->head) {
        btr_sllist_node_t *next = this->head->next;
        free(this->head);
        this->head = next;
    }
    this->size = 0;
}
void BTR_SLList_clear(btr_sllist_t *);
