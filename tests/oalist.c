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

int main(void)
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    printf("SUCCESS\n");
    return 0;
}
