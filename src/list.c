#include "btrstd/list.h"

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
void *BTR_SLList_pop(btr_sllist_t *this, size_t index)
{
    if (index == 0) {
        btr_sllist_node_t *connectTo = this->head->next;
        void *data = this->head->payload;
        free(this->head);
        this->head = connectTo;
        return data;
    } else {
        btr_sllist_node_t **currNode = &this->head;
        for (size_t currIndex = 0; currIndex != index - 1; currIndex++)
            currNode = &(*currNode)->next;
        btr_sllist_node_t *connectTo = (*currNode)->next->next;
        void *data = (*currNode)->next->payload;
        free((*currNode)->next);
        (*currNode)->next = connectTo;
        return data;
    }
}
void *BTR_SLList_get(btr_sllist_t *this, size_t index)
{
    btr_sllist_node_t *currNode = this->head;
    for (size_t currIndex = 0; currIndex != index; currIndex++)
        currNode = currNode->next;
    return currNode->payload;
}
