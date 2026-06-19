#include "btrstd/allocators/pool.h"
#include "btrstd/containers/borrowing_linked_list.h"

#include "stdio.h"
#include "assert.h"
#include "string.h"


static void test1(void)
{
    printf("> test1\n");
    btr_pool_s pool = BTR_Pool_make(sizeof(int), 256, NULL);
    BTR_Pool_destroy(&pool);
}

static void test2(void)
{
    printf("> test2\n");

    btr_pool_s pool = BTR_Pool_make(sizeof(int), 8, NULL);

    int *p = BTR_unwrap(BTR_Pool_allocate(&pool));
    assert(p != NULL);
    *p = 42;
    assert(*p == 42);

    BTR_Pool_destroy(&pool);
}

static void test3(void)
{
    printf("> test3\n");

    btr_pool_s pool = BTR_Pool_make(sizeof(int), 8, NULL);

    void *ptrs[8];
    for (int i = 0; i < 8; i++)
    {
        ptrs[i] = BTR_unwrap(BTR_Pool_allocate(&pool));
        assert(ptrs[i] != NULL);
    }

    // all pointers should be unique
    for (int i = 0; i < 8; i++)
        for (int j = i + 1; j < 8; j++)
            assert(ptrs[i] != ptrs[j]);

    BTR_Pool_destroy(&pool);
}

static void test4(void)
{
    printf("> test4\n");

    btr_pool_s pool = BTR_Pool_make(sizeof(int), 4, NULL);

    for (int i = 0; i < 4; i++)
    {
        btr_alloc_r r = BTR_Pool_allocate(&pool);
        assert(r.status == BTR_OK);
        assert(r.value != NULL);
    }

    // next allocation should fail
    btr_alloc_r r = BTR_Pool_allocate(&pool);
    assert(r.status == BTR_ERR);
    assert(r.error == BTR_ALLOC_ERR_OUT_OF_MEMORY);

    BTR_Pool_destroy(&pool);
}

static void test5(void)
{
    printf("> test5\n");

    btr_pool_s pool = BTR_Pool_make(sizeof(int), 4, NULL);

    int *a = BTR_unwrap(BTR_Pool_allocate(&pool));
    int *b = BTR_unwrap(BTR_Pool_allocate(&pool));
    *a = 10;
    *b = 20;

    BTR_Pool_deallocate(&pool, a);

    // reallocate should reuse the freed slot
    int *c = BTR_unwrap(BTR_Pool_allocate(&pool));
    assert(c == a);

    BTR_Pool_destroy(&pool);
}

static void test6(void)
{
    printf("> test6\n");

    btr_pool_s pool = BTR_Pool_make(sizeof(int), 4, NULL);

    int *p = BTR_unwrap(BTR_Pool_allocate(&pool));
    assert(*p == 0);

    BTR_Pool_destroy(&pool);
}

static void test7(void)
{
    printf("> test7\n");

    btr_pool_s pool = BTR_Pool_make(sizeof(int), 4, NULL);

    int *a = BTR_unwrap(BTR_Pool_allocate(&pool));
    int *b = BTR_unwrap(BTR_Pool_allocate(&pool));
    *a = 100;
    *b = 200;
    assert(*a == 100);
    assert(*b == 200);

    // deallocate and reallocate slot 0
    BTR_Pool_deallocate(&pool, a);
    int *c = BTR_unwrap(BTR_Pool_allocate(&pool));
    assert(c == a);
    *c = 300;

    // slot 1 should still have its value
    assert(*b == 200);

    BTR_Pool_destroy(&pool);
}

static void test8(void)
{
    printf("> test8\n");

    btr_pool_s pool = BTR_Pool_make(sizeof(int), 4, NULL);

    // exhaust the pool
    for (int i = 0; i < 4; i++)
        BTR_unwrap(BTR_Pool_allocate(&pool));

    btr_alloc_r r = BTR_Pool_allocate(&pool);
    assert(r.status == BTR_ERR);

    // reset with larger count
    BTR_Pool_reset(&pool, 8);

    for (int i = 0; i < 8; i++)
    {
        r = BTR_Pool_allocate(&pool);
        assert(r.status == BTR_OK);
    }

    // exhausted again
    r = BTR_Pool_allocate(&pool);
    assert(r.status == BTR_ERR);

    BTR_Pool_destroy(&pool);
}

static void test9(void)
{
    printf("> test9\n");

    btr_pool_s pool = BTR_Pool_make(sizeof(double) * 4, 3, NULL);

    double (*a)[4] = BTR_unwrap(BTR_Pool_allocate(&pool));
    double (*b)[4] = BTR_unwrap(BTR_Pool_allocate(&pool));
    assert(a != NULL);
    assert(b != NULL);
    assert(a != b);

    (*a)[0] = 1.0;
    (*a)[1] = 2.0;
    (*a)[2] = 3.0;
    (*a)[3] = 4.0;

    assert((*a)[0] == 1.0);
    assert((*a)[3] == 4.0);

    BTR_Pool_destroy(&pool);
}

static void test10(void)
{
    printf("> test10\n");

    const int VALUES[] = {
        0, 10, 20, 30, 40, 50, 60, 70, 80, 90
    };

    btr_pool_s pool = BTR_Pool_make(sizeof(btr_bllist_node_s), 64, NULL);
    btr_allocator_s allocator = BTR_Pool_getWrapper(&pool);
    btr_bllist_s list = BTR_BLList_make(&allocator);

    for (size_t i = 0; i < 64; i++)
        BTR_BLList_append(&list, (void *)&VALUES[i % 10]);

    assert(BTR_isErr(BTR_Pool_allocate(&pool)));

    for (size_t i = 0; i < 64; i++)
        BTR_BLList_pop(&list, -1);

    assert(BTR_isOk(BTR_Pool_allocate(&pool)));

    BTR_BLList_free(&list);
    BTR_Pool_destroy(&pool);
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
    test10();
    printf("SUCCESS\n");
    return 0;
}
