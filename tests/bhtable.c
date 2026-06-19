#include "btrstd/containers/borrowing_hash_table.h"
#include "btrstd/allocators/arena.h"
#include "assert.h"
#include "stdio.h"

// test make and free
static void test1(void)
{
    printf("> test1\n");
    btr_bhtable_s table = BTR_BHTable_make(NULL, NULL, NULL);
    assert(BTR_BHTable_len(&table) == 0);
    BTR_BHTable_free(&table);
}
// test put and get
static void test2(void)
{
    printf("> test2\n");
    const int VALUES[] = {
        0, 10, 20, 30, 40, 50, 60, 70, 80, 90
    };
    btr_bhtable_s table = BTR_BHTable_make(BTR_hashCString, BTR_compareCString, NULL);
    BTR_BHTable_put(&table, "key1", &VALUES[1]);
    BTR_BHTable_put(&table, "key2", &VALUES[2]);
    BTR_BHTable_put(&table, "key3", &VALUES[3]);
    assert(BTR_BHTable_len(&table) == 3);
    assert(*(int *)BTR_unwrap(BTR_BHTable_get(&table, "key1")) == VALUES[1]);
    assert(*(int *)BTR_unwrap(BTR_BHTable_get(&table, "key2")) == VALUES[2]);
    assert(*(int *)BTR_unwrap(BTR_BHTable_get(&table, "key3")) == VALUES[3]);
    BTR_BHTable_free(&table);
}
// test rehashing
static void test3(void)
{
    printf("> test3\n");
    btr_arena_s arena = BTR_Arena_make(1024 * 1024, NULL);
    btr_bhtable_s table = BTR_BHTable_make(BTR_hashInt32, BTR_compareInt32, NULL);
    btr_bllist_s keys = BTR_BLList_make(NULL);
    for (size_t i = 0; i < 4096; i++)
    {
        int *key = BTR_unwrap(BTR_Arena_allocate(&arena, sizeof(int)));
        *key = i;
        BTR_BLList_append(&keys, key);
        BTR_BHTable_put(&table, key, key);
    }
    assert(BTR_BHTable_len(&table) == 4096);
    BTR_BLList_free(&keys);
    BTR_Arena_destroy(&arena);
    BTR_BHTable_free(&table);
}
// test pop
static void test4(void)
{
    printf("> test4\n");
    const int VALUES[] = {100, 200, 300};
    btr_bhtable_s table = BTR_BHTable_make(BTR_hashCString, BTR_compareCString, NULL);

    BTR_BHTable_put(&table, "key1", &VALUES[0]);
    BTR_BHTable_put(&table, "key2", &VALUES[1]);
    BTR_BHTable_put(&table, "key3", &VALUES[2]);
    assert(BTR_BHTable_len(&table) == 3);

    btr_container_ptr_r result = BTR_BHTable_pop(&table, "key2");
    assert(result.status == BTR_STATUS_OK);
    assert(*(int *)result.value == VALUES[1]);

    result = BTR_BHTable_get(&table, "key2");
    assert(result.status == BTR_STATUS_ERR);

    assert(*(int *)BTR_unwrap(BTR_BHTable_get(&table, "key1")) == VALUES[0]);
    assert(*(int *)BTR_unwrap(BTR_BHTable_get(&table, "key3")) == VALUES[2]);
    assert(BTR_BHTable_len(&table) == 2);

    result = BTR_BHTable_pop(&table, "nonexistent");
    assert(result.status == BTR_STATUS_ERR);
    assert(BTR_BHTable_len(&table) == 2);

    BTR_BHTable_free(&table);
}

// test pop with rehashing
static void test5(void)
{
    printf("> test5\n");
    btr_arena_s arena = BTR_Arena_make(1024 * 1024, NULL);
    btr_bhtable_s table = BTR_BHTable_make(BTR_hashInt32, BTR_compareInt32, NULL);

    for (size_t i = 0; i < 1024; i++)
    {
        int *key = BTR_unwrap(BTR_Arena_allocate(&arena, sizeof(int)));
        int *val = BTR_unwrap(BTR_Arena_allocate(&arena, sizeof(int)));
        *key = (int)i;
        *val = (int)i * 10;
        BTR_BHTable_put(&table, key, val);
    }

    assert(BTR_BHTable_len(&table) == 1024);
    for (size_t i = 128; i < 256; i++)
    {
        btr_container_ptr_r result = BTR_BHTable_pop(&table, &i);
        assert(result.status == BTR_STATUS_OK);
        assert(*(int *)result.value == (int)i * 10);
    }
    assert(BTR_BHTable_len(&table) == 896);

    for (size_t i = 0; i < 128; i++)
    {
        btr_container_ptr_r result = BTR_BHTable_get(&table, &i);
        assert(result.status == BTR_STATUS_OK);
        assert(*(int *)result.value == (int)i * 10);
    }

    for (size_t i = 128; i < 256; i++)
    {
        btr_container_ptr_r result = BTR_BHTable_get(&table, &i);
        assert(result.status == BTR_STATUS_ERR);
    }

    BTR_BHTable_free(&table);
    BTR_Arena_destroy(&arena);
}

// test contains
static void test6(void)
{
    printf("> test6\n");
    btr_bhtable_s table = BTR_BHTable_make(BTR_hashCString, BTR_compareCString, NULL);
    const int VALUES[] = {0, 10, 20, 30};

    assert(!BTR_BHTable_contains(&table, "key1"));
    assert(!BTR_BHTable_contains(&table, "nonexistent"));

    BTR_BHTable_put(&table, "key1", &VALUES[1]);
    BTR_BHTable_put(&table, "key2", &VALUES[2]);
    BTR_BHTable_put(&table, "key3", &VALUES[3]);
    assert(BTR_BHTable_len(&table) == 3);

    assert(BTR_BHTable_contains(&table, "key1"));
    assert(BTR_BHTable_contains(&table, "key2"));
    assert(BTR_BHTable_contains(&table, "key3"));
    assert(!BTR_BHTable_contains(&table, "key4"));
    assert(!BTR_BHTable_contains(&table, ""));

    BTR_BHTable_pop(&table, "key2");
    assert(BTR_BHTable_len(&table) == 2);
    assert(!BTR_BHTable_contains(&table, "key2"));
    assert(BTR_BHTable_contains(&table, "key1"));
    assert(BTR_BHTable_contains(&table, "key3"));

    BTR_BHTable_free(&table);
}

// test contains with int keys
static void test7(void)
{
    printf("> test7\n");
    btr_bhtable_s table = BTR_BHTable_make(BTR_hashInt32, BTR_compareInt32, NULL);
    btr_arena_s arena = BTR_Arena_make(1024 * 1024, NULL);

    for (int i = 0; i < 256; i++)
    {
        int *key = BTR_unwrap(BTR_Arena_allocate(&arena, sizeof(int)));
        *key = i;
        BTR_BHTable_put(&table, key, key);
    }
    assert(BTR_BHTable_len(&table) == 256);

    for (int i = 0; i < 256; i++)
        assert(BTR_BHTable_contains(&table, &i));

    for (int i = 256; i < 512; i++)
        assert(!BTR_BHTable_contains(&table, &i));

    BTR_BHTable_free(&table);
    BTR_Arena_destroy(&arena);
}

// test clear
static void test8(void)
{
    printf("> test8\n");
    const int VALUES[] = {10, 20, 30};
    btr_bhtable_s table = BTR_BHTable_make(BTR_hashCString, BTR_compareCString, NULL);

    BTR_BHTable_put(&table, "a", &VALUES[0]);
    BTR_BHTable_put(&table, "b", &VALUES[1]);
    BTR_BHTable_put(&table, "c", &VALUES[2]);
    assert(BTR_BHTable_len(&table) == 3);
    assert(BTR_BHTable_contains(&table, "a"));

    BTR_BHTable_clear(&table);
    assert(BTR_BHTable_len(&table) == 0);
    assert(!BTR_BHTable_contains(&table, "a"));
    assert(!BTR_BHTable_contains(&table, "b"));
    assert(!BTR_BHTable_contains(&table, "c"));

    BTR_BHTable_put(&table, "x", &VALUES[0]);
    BTR_BHTable_put(&table, "y", &VALUES[1]);
    assert(BTR_BHTable_len(&table) == 2);
    assert(BTR_BHTable_contains(&table, "x"));
    assert(BTR_BHTable_contains(&table, "y"));

    BTR_BHTable_free(&table);
}

// test keys
static void test9(void)
{
    printf("> test9\n");
    btr_bhtable_s table = BTR_BHTable_make(BTR_hashCString, BTR_compareCString, NULL);
    const int VALUES[] = {100, 200, 300};

    btr_bllist_s keys = BTR_BHTable_keys(&table);
    assert(BTR_BLList_len(&keys) == 0);
    BTR_BLList_free(&keys);

    BTR_BHTable_put(&table, "k1", &VALUES[0]);
    BTR_BHTable_put(&table, "k2", &VALUES[1]);
    BTR_BHTable_put(&table, "k3", &VALUES[2]);
    assert(BTR_BHTable_len(&table) == 3);

    keys = BTR_BHTable_keys(&table);
    assert(BTR_BLList_len(&keys) == 3);
    assert(BTR_BHTable_contains(&table, BTR_unwrap(BTR_BLList_get(&keys, 0))));
    assert(BTR_BHTable_contains(&table, BTR_unwrap(BTR_BLList_get(&keys, 1))));
    assert(BTR_BHTable_contains(&table, BTR_unwrap(BTR_BLList_get(&keys, 2))));
    BTR_BLList_free(&keys);

    BTR_BHTable_clear(&table);
    keys = BTR_BHTable_keys(&table);
    assert(BTR_BLList_len(&keys) == 0);
    BTR_BLList_free(&keys);

    BTR_BHTable_free(&table);
}

int main(void)
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    printf("SUCCESS\n");
}
