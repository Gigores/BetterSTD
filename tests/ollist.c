#include "btrstd/containers/owning_linked_list.h"
#include "assert.h"
#include "stdio.h"

// test make and free
static void test1(void)
{
    printf("> test1\n");
    btr_ollist_s list = BTR_OLList_make(sizeof(int), NULL);
    BTR_OLList_free(&list);
}

int main(void)
{
    test1();
    printf("SUCCESS\n");
    return 0;
}
