#include "btrstd/containers/bhtable.h"
#include "assert.h"
#include "stdio.h"

// test make and free
static void test1(void)
{
    printf("> test1\n");
    btr_bhtable_t table = BTR_BHTable_make(NULL, NULL, NULL);
    BTR_BHTable_free(&table);
}
// test put and get
static void test2(void)
{
    printf("> test2\n");
    const int VALUES[] = {
        0, 10, 20, 30, 40, 50, 60, 70, 80, 90
    };
    btr_bhtable_t table = BTR_BHTable_make(BTR_hashCString, BTR_compareCString, NULL);
    BTR_BHTable_put(&table, "key1", &VALUES[1]);
    BTR_BHTable_put(&table, "key2", &VALUES[2]);
    BTR_BHTable_put(&table, "key3", &VALUES[3]);
    assert(*(int *)BTR_unwrap(BTR_BHTable_get(&table, "key1")) == VALUES[1]);
    assert(*(int *)BTR_unwrap(BTR_BHTable_get(&table, "key2")) == VALUES[2]);
    assert(*(int *)BTR_unwrap(BTR_BHTable_get(&table, "key3")) == VALUES[3]);
    BTR_BHTable_free(&table);
}

int main(void)
{
    test1();
    test2();
    printf("SUCCESS\n");
}
