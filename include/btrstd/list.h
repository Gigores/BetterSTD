#include "stdlib.h"

typedef struct BTR_SLListNode {
    void *payload;
    struct BTR_SLListNode *next;
} btr_sllist_node_t;

typedef struct {
    btr_sllist_node_t *head;
    size_t size;
} btr_sllist_t;

void BTR_SLList_append(btr_sllist_t *, void *data);
void *BTR_SLList_pop(btr_sllist_t *, size_t index);
void *BTR_SLList_get(btr_sllist_t *, size_t index);

#define BTR_SLLIST_FOREACH(LUST, VAR) for (btr_sllist_node_t *VAR = (LIST).head; VAR != NULL; VAR = VAR->next)
#define BTR_SLLIST_ENUMERATE(LIST, NODE, i) size_t i = 0; for(btr_sllist_node_t *NODE = (LIST).head; NODE != NULL; NODE = NODE->next, i++)
