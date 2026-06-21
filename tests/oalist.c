#include "btrstd/containers/owning_array_list.h"
#include "assert.h"
#include "stdio.h"
#include "string.h"

#define ARR_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

// test make, free, and basic append
static void test1(void)
{
    printf("> test1\n");

    btr_oalist_s list = BTR_OALIST_OF(int);
    assert(list.capacity == 8);
    assert(list.count == 0);
    assert(list.itemSize == sizeof(int));

    int values[] = {10, 20, 30};
    for (size_t i = 0; i < ARR_COUNT(values); i++)
        *(int *)BTR_OAList_append(&list) = values[i];

    assert(list.count == 3);
    int *data = (int *)list.data;
    assert(data[0] == 10);
    assert(data[1] == 20);
    assert(data[2] == 30);

    BTR_OAList_free(&list);
}

// test append with capacity growth
static void test2(void)
{
    printf("> test2\n");

    btr_oalist_s list = BTR_OALIST_OF(int);

    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (size_t i = 0; i < ARR_COUNT(values); i++)
    {
        int *slot = BTR_OAList_append(&list);
        assert(slot != NULL);
        *slot = values[i];
    }

    assert(list.count == 10);
    assert(list.capacity >= 10);
    int *data = (int *)list.data;
    for (size_t i = 0; i < ARR_COUNT(values); i++)
        assert(data[i] == (int)i);

    BTR_OAList_free(&list);
}

// test prepend
static void test3(void)
{
    printf("> test3\n");

    btr_oalist_s list = BTR_OALIST_OF(int);

    int values[] = {10, 20, 30, 40};
    for (size_t i = 0; i < ARR_COUNT(values); i++)
        *(int *)BTR_OAList_prepend(&list) = values[i];

    assert(list.count == 4);
    int *data = (int *)list.data;
    assert(data[0] == 40);
    assert(data[1] == 30);
    assert(data[2] == 20);
    assert(data[3] == 10);

    BTR_OAList_free(&list);
}

// test insert at various positions
static void test4(void)
{
    printf("> test4\n");

    btr_oalist_s list = BTR_OALIST_OF(int);

    int a = 1, b = 2, c = 3, d = 4;

    // insert into empty list
    *(int *)BTR_OAList_insert(&list, 0) = a;
    assert(list.count == 1);
    assert(*(int *)list.data == 1);

    // insert at end (index == count)
    *(int *)BTR_OAList_insert(&list, 1) = b;
    assert(list.count == 2);
    {
        int *data = (int *)list.data;
        assert(data[0] == 1);
        assert(data[1] == 2);
    }

    // insert in middle
    *(int *)BTR_OAList_insert(&list, 1) = c;
    assert(list.count == 3);
    {
        int *data = (int *)list.data;
        assert(data[0] == 1);
        assert(data[1] == 3);
        assert(data[2] == 2);
    }

    // insert at head
    *(int *)BTR_OAList_insert(&list, 0) = d;
    assert(list.count == 4);
    {
        int *data = (int *)list.data;
        assert(data[0] == 4);
        assert(data[1] == 1);
        assert(data[2] == 3);
        assert(data[3] == 2);
    }

    BTR_OAList_free(&list);
}

// test insert with negative index
static void test5(void)
{
    printf("> test5\n");

    btr_oalist_s list = BTR_OALIST(int, 10, 20, 30, 40);

    // negative index: -1 means before last element
    int v = 99;
    *(int *)BTR_OAList_insert(&list, -1) = v;
    {
        int expected[] = {10, 20, 30, 99, 40};
        assert(list.count == 5);
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    // negative index: -5 means before first element
    int w = 0;
    *(int *)BTR_OAList_insert(&list, -5) = w;
    {
        int expected[] = {0, 10, 20, 30, 99, 40};
        assert(list.count == 6);
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    BTR_OAList_free(&list);
}

// test mixing append, prepend, and insert
static void test6(void)
{
    printf("> test6\n");

    btr_oalist_s list = BTR_OALIST_OF(int);

    int a = 2, b = 3;
    *(int *)BTR_OAList_append(&list) = a;
    *(int *)BTR_OAList_append(&list) = b;
    {
        int *data = (int *)list.data;
        assert(data[0] == 2);
        assert(data[1] == 3);
    }

    int c = 1;
    *(int *)BTR_OAList_prepend(&list) = c;
    {
        int expected[] = {1, 2, 3};
        assert(list.count == 3);
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    int d = 4;
    *(int *)BTR_OAList_append(&list) = d;
    {
        int expected[] = {1, 2, 3, 4};
        assert(list.count == 4);
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    int e = 0;
    *(int *)BTR_OAList_insert(&list, 0) = e;
    {
        int expected[] = {0, 1, 2, 3, 4};
        assert(list.count == 5);
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    BTR_OAList_free(&list);
}

// test get at various positions
static void test7(void)
{
    printf("> test7\n");

    btr_oalist_s list = BTR_OALIST(int, 10, 20, 30, 40, 50);

    // positive indices
    assert(list.count == 5);
    {
        int expected[] = {10, 20, 30, 40, 50};
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
        {
            btr_container_ptr_r r = BTR_OAList_get(&list, i);
            assert(r.status == BTR_STATUS_OK);
            assert(*(int *)r.value == expected[i]);
        }
    }

    // negative indices
    assert(*(int *)BTR_unwrap(BTR_OAList_get(&list, -1)) == 50);
    assert(*(int *)BTR_unwrap(BTR_OAList_get(&list, -2)) == 40);
    assert(*(int *)BTR_unwrap(BTR_OAList_get(&list, -5)) == 10);

    // out of bounds
    {
        btr_container_ptr_r r = BTR_OAList_get(&list, 5);
        assert(r.status == BTR_STATUS_ERR);
        assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    }
    {
        btr_container_ptr_r r = BTR_OAList_get(&list, -6);
        assert(r.status == BTR_STATUS_ERR);
        assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);
    }

    BTR_OAList_free(&list);
}

// test get on empty list
static void test8(void)
{
    printf("> test8\n");

    btr_oalist_s list = BTR_OALIST_OF(int);

    btr_container_ptr_r r = BTR_OAList_get(&list, 0);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    BTR_OAList_free(&list);
}

// test first
static void test9(void)
{
    printf("> test9\n");

    // non-empty list
    btr_oalist_s list = BTR_OALIST(int, 100, 200, 300);
    btr_container_ptr_r r = BTR_OAList_first(&list);
    assert(r.status == BTR_STATUS_OK);
    assert(*(int *)r.value == 100);

    // empty list
    btr_oalist_s empty = BTR_OALIST_OF(int);
    r = BTR_OAList_first(&empty);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    BTR_OAList_free(&list);
    BTR_OAList_free(&empty);
}

// test last
static void test10(void)
{
    printf("> test10\n");

    // non-empty list
    btr_oalist_s list = BTR_OALIST(int, 7, 8, 9);
    btr_container_ptr_r r = BTR_OAList_last(&list);
    assert(r.status == BTR_STATUS_OK);
    assert(*(int *)r.value == 9);

    // empty list
    btr_oalist_s empty = BTR_OALIST_OF(int);
    r = BTR_OAList_last(&empty);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    BTR_OAList_free(&list);
    BTR_OAList_free(&empty);
}

// test pop from end
static void test11(void)
{
    printf("> test11\n");

    btr_oalist_s list = BTR_OALIST(int, 10, 20, 30, 40, 50);

    int popped;
    BTR_OAList_pop(&list, 4, &popped);
    assert(popped == 50);
    assert(list.count == 4);
    {
        int expected[] = {10, 20, 30, 40};
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    BTR_OAList_free(&list);
}

// test pop from front
static void test12(void)
{
    printf("> test12\n");

    btr_oalist_s list = BTR_OALIST(int, 100, 200, 300);

    int popped;
    BTR_OAList_pop(&list, 0, &popped);
    assert(popped == 100);
    assert(list.count == 2);
    {
        int expected[] = {200, 300};
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    BTR_OAList_free(&list);
}

// test pop from middle
static void test13(void)
{
    printf("> test13\n");

    btr_oalist_s list = BTR_OALIST(int, 1, 2, 3, 4, 5);

    int popped;
    BTR_OAList_pop(&list, 2, &popped);
    assert(popped == 3);
    assert(list.count == 4);
    {
        int expected[] = {1, 2, 4, 5};
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    BTR_OAList_free(&list);
}

// test pop with negative index
static void test14(void)
{
    printf("> test14\n");

    btr_oalist_s list = BTR_OALIST(int, 10, 20, 30, 40);

    int popped;
    BTR_OAList_pop(&list, -1, &popped);
    assert(popped == 40);
    assert(list.count == 3);

    BTR_OAList_pop(&list, 0, &popped);
    assert(popped == 10);
    assert(list.count == 2);

    BTR_OAList_free(&list);
}

// test pop with NULL buffer (discard value)
static void test15(void)
{
    printf("> test15\n");

    btr_oalist_s list = BTR_OALIST(int, 7, 8, 9);

    BTR_OAList_pop(&list, 0, NULL);
    assert(list.count == 2);
    {
        int expected[] = {8, 9};
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    BTR_OAList_free(&list);
}

// test pop until empty
static void test16(void)
{
    printf("> test16\n");

    btr_oalist_s list = BTR_OALIST(int, 1, 2, 3);

    int popped;
    BTR_OAList_pop(&list, -1, &popped);
    assert(popped == 3);
    assert(list.count == 2);

    BTR_OAList_pop(&list, -1, &popped);
    assert(popped == 2);
    assert(list.count == 1);

    BTR_OAList_pop(&list, -1, &popped);
    assert(popped == 1);
    assert(list.count == 0);

    BTR_OAList_free(&list);
}

static bool cmp_int(const void *a, const void *b)
{
    return *(const int *)a == *(const int *)b;
}

// test indexOf
static void test17(void)
{
    printf("> test17\n");

    btr_oalist_s list = BTR_OALIST(int, 10, 20, 30, 40, 50);

    for (size_t i = 0; i < 5; i++)
    {
        int val = (int)(i + 1) * 10;
        long idx = BTR_unwrap(BTR_OAList_indexOf(&list, &val, cmp_int));
        assert(idx == (long)i);
    }

    BTR_OAList_free(&list);
}

// test indexOf - not found
static void test18(void)
{
    printf("> test18\n");

    btr_oalist_s list = BTR_OALIST(int, 10, 20, 30);

    int missing = 999;
    btr_container_idx_r r = BTR_OAList_indexOf(&list, &missing, cmp_int);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_NOT_FOUND);

    BTR_OAList_free(&list);
}

// test indexOf - duplicate (finds first)
static void test19(void)
{
    printf("> test19\n");

    btr_oalist_s list = BTR_OALIST(int, 10, 20, 30, 20, 40);

    int needle = 20;
    long idx = BTR_unwrap(BTR_OAList_indexOf(&list, &needle, cmp_int));
    assert(idx == 1);

    BTR_OAList_free(&list);
}

// test len
static void test20(void)
{
    printf("> test20\n");

    btr_oalist_s list = BTR_OALIST_OF(int);
    assert(BTR_OAList_len(&list) == 0);

    *(int *)BTR_OAList_append(&list) = 1;
    assert(BTR_OAList_len(&list) == 1);

    *(int *)BTR_OAList_append(&list) = 2;
    assert(BTR_OAList_len(&list) == 2);

    BTR_OAList_free(&list);
}

// test isEmpty
static void test21(void)
{
    printf("> test21\n");

    btr_oalist_s empty = BTR_OALIST_OF(int);
    assert(BTR_OAList_isEmpty(&empty));

    *(int *)BTR_OAList_append(&empty) = 1;
    assert(!BTR_OAList_isEmpty(&empty));

    BTR_OAList_free(&empty);
}

// test reverse
static void test22(void)
{
    printf("> test22\n");

    btr_oalist_s list = BTR_OALIST(int, 10, 20, 30, 40, 50);

    assert(BTR_OAList_len(&list) == 5);

    BTR_OAList_reverse(&list);
    assert(BTR_OAList_len(&list) == 5);
    {
        int expected[] = {50, 40, 30, 20, 10};
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    BTR_OAList_reverse(&list);
    assert(BTR_OAList_len(&list) == 5);
    {
        int expected[] = {10, 20, 30, 40, 50};
        int *data = (int *)list.data;
        for (size_t i = 0; i < ARR_COUNT(expected); i++)
            assert(data[i] == expected[i]);
    }

    BTR_OAList_free(&list);
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
    test11();
    test12();
    test13();
    test14();
    test15();
    test16();
    test17();
    test18();
    test19();
    test20();
    test21();
    test22();
    printf("SUCCESS\n");
    return 0;
}
