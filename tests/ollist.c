#include "btrstd/containers/owning_linked_list.h"
#include "assert.h"
#include "stdio.h"

static int getInt(void *p)
{
    return *(int *)p;
}
static bool cmp_int(const void *a, const void *b)
{
    return *(const int *)a == *(const int *)b;
}

// test make, isEmpty, len, free
static void test1(void)
{
    printf("> test1\n");
    btr_ollist_s list = BTR_OLList_make(sizeof(int), NULL);
    assert(BTR_OLList_isEmpty(&list));
    assert(BTR_OLList_len(&list) == 0);
    BTR_OLList_free(&list);
}

// test makeFrom, get (positive and negative), first, last
static void test2(void)
{
    printf("> test2\n");

    const int VALUES[] = {10, 20, 30, 40, 50};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_ollist_s list = BTR_OLList_makeFrom((void *)VALUES, N, sizeof(int), NULL);
    assert(BTR_OLList_len(&list) == N);
    assert(!BTR_OLList_isEmpty(&list));

    // positive indexing
    for (size_t i = 0; i < N; i++)
        assert(getInt(BTR_unwrap(BTR_OLList_get(&list, i))) == VALUES[i]);

    // negative indexing
    for (long k = -1; k >= -(long)N; k--)
        assert(getInt(BTR_unwrap(BTR_OLList_get(&list, k))) == VALUES[N + k]);

    // first, last
    assert(getInt(BTR_unwrap(BTR_OLList_first(&list))) == VALUES[0]);
    assert(getInt(BTR_unwrap(BTR_OLList_last(&list))) == VALUES[N - 1]);

    BTR_OLList_free(&list);
}

// test append, FOREACH
static void test3(void)
{
    printf("> test3\n");

    const int VALUES[] = {100, 200, 300, 400};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_ollist_s list = BTR_OLList_make(sizeof(int), NULL);
    for (size_t i = 0; i < N; i++)
        *(int *)BTR_OLList_append(&list) = VALUES[i];

    assert(BTR_OLList_len(&list) == N);

    size_t i = 0;
    BTR_OLLIST_FOREACH(&list, node)
    {
        assert(getInt(node) == VALUES[i]);
        i++;
    }
    assert(i == N);

    BTR_OLList_free(&list);
}

// test prepend
static void test4(void)
{
    printf("> test4\n");

    const int VALUES[] = {1, 2, 3, 4, 5};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_ollist_s list = BTR_OLLIST_OF(int);
    for (size_t i = 0; i < N; i++)
        *(int *)BTR_OLList_prepend(&list) = VALUES[i];

    assert(BTR_OLList_len(&list) == N);

    BTR_OLLIST_ENUMERATE(&list, node, i)
        assert(getInt(node) == VALUES[N - 1 - i]);
    assert(i == N);

    BTR_OLList_free(&list);
}

// test insert at various positions
static void test5(void)
{
    printf("> test5\n");

    btr_ollist_s list = BTR_OLLIST_OF(int);

    // insert into empty (index 0)
    *(int *)BTR_OLList_insert(&list, 0) = 10;
    assert(BTR_OLList_len(&list) == 1);
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 0))) == 10);

    // insert middle (index 1 -> end since size=1)
    *(int *)BTR_OLList_insert(&list, 1) = 30;
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 1))) == 30);

    // insert at index 1 (between 10 and 30)
    *(int *)BTR_OLList_insert(&list, 1) = 20;
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 0))) == 10);
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 1))) == 20);
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 2))) == 30);

    // insert at head
    *(int *)BTR_OLList_insert(&list, 0) = 0;
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 0))) == 0);

    // insert at negative index
    *(int *)BTR_OLList_insert(&list, -1) = 99;
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, -2))) == 99);

    BTR_OLList_free(&list);
}

// test pop with buffer and with NULL buffer
static void test6(void)
{
    printf("> test6\n");

    const size_t N = 5;
    btr_ollist_s list = BTR_OLLIST(int, 10, 20, 30, 40, 50);
    assert(BTR_OLList_len(&list) == N);

    // pop middle with buffer
    int buf;
    BTR_OLList_pop(&list, 2, &buf);
    assert(buf == 30);
    assert(BTR_OLList_len(&list) == N - 1);
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 2))) == 40);

    // pop first
    BTR_OLList_pop(&list, 0, &buf);
    assert(buf == 10);
    assert(BTR_OLList_len(&list) == N - 2);
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 0))) == 20);

    // pop last
    BTR_OLList_pop(&list, -1, &buf);
    assert(buf == 50);
    assert(BTR_OLList_len(&list) == N - 3);

    // pop with NULL buffer (no copy)
    BTR_OLList_pop(&list, 0, NULL);
    assert(BTR_OLList_len(&list) == N - 4);

    // pop remaining
    BTR_OLList_pop(&list, 0, NULL);
    assert(BTR_OLList_len(&list) == N - 5);
    assert(BTR_OLList_isEmpty(&list));

    BTR_OLList_free(&list);
}

// test get/first/last on empty list (error)
static void test7(void)
{
    printf("> test7\n");

    btr_ollist_s list = BTR_OLLIST_OF(int);

    btr_container_ptr_r r = BTR_OLList_get(&list, 0);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    r = BTR_OLList_first(&list);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    r = BTR_OLList_last(&list);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    BTR_OLList_free(&list);
}

// test indexOf
static void test8(void)
{
    printf("> test8\n");

    const int VALUES[] = {10, 20, 30, 40, 50};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_ollist_s list = BTR_OLList_makeFrom((void *)VALUES, N, sizeof(int), NULL);

    // found
    for (size_t i = 0; i < N; i++)
    {
        long idx = BTR_unwrap(BTR_OLList_indexOf(&list, (void *)&VALUES[i], cmp_int));
        assert(idx == (long)i);
    }

    // not found
    int missing = 999;
    btr_container_idx_r nr = BTR_OLList_indexOf(&list, &missing, cmp_int);
    assert(nr.status == BTR_STATUS_ERR);
    assert(nr.error == BTR_CONTAINER_ERR_NOT_FOUND);

    // duplicates (first occurrence)
    BTR_OLList_pop(&list, -1, NULL);
    int val30 = 30;
    *(int *)BTR_OLList_append(&list) = val30;

    long first = BTR_unwrap(BTR_OLList_indexOf(&list, &val30, cmp_int));
    assert(first == 2);

    BTR_OLList_free(&list);
}

// test reverse
static void test9(void)
{
    printf("> test9\n");

    const int VALUES[] = {10, 20, 30, 40, 50};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_ollist_s list = BTR_OLList_makeFrom((void *)VALUES, N, sizeof(int), NULL);

    BTR_OLList_reverse(&list);
    for (size_t i = 0; i < N; i++)
        assert(getInt(BTR_unwrap(BTR_OLList_get(&list, i))) == VALUES[N - 1 - i]);

    // reverse back
    BTR_OLList_reverse(&list);
    for (size_t i = 0; i < N; i++)
        assert(getInt(BTR_unwrap(BTR_OLList_get(&list, i))) == VALUES[i]);

    // reverse single element list
    btr_ollist_s single = BTR_OLList_makeFrom((void *)&VALUES[0], 1, sizeof(int), NULL);
    BTR_OLList_reverse(&single);
    assert(getInt(BTR_unwrap(BTR_OLList_get(&single, 0))) == VALUES[0]);
    BTR_OLList_free(&single);

    BTR_OLList_free(&list);
}

// test clone
static void test10(void)
{
    printf("> test10\n");

    const int VALUES[] = {5, 10, 15, 20};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_ollist_s list = BTR_OLList_makeFrom((void *)VALUES, N, sizeof(int), NULL);

    btr_ollist_s clone = BTR_OLList_clone(&list, NULL);
    assert(BTR_OLList_len(&clone) == N);

    for (size_t i = 0; i < N; i++)
    {
        int a = getInt(BTR_unwrap(BTR_OLList_get(&list, i)));
        int b = getInt(BTR_unwrap(BTR_OLList_get(&clone, i)));
        assert(a == b);
    }

    // modifying clone does not affect original
    *(int *)BTR_OLList_append(&clone) = 99;
    assert(BTR_OLList_len(&list) == N);
    assert(BTR_OLList_len(&clone) == N + 1);

    BTR_OLList_free(&list);
    BTR_OLList_free(&clone);
}

// test clear (reuse list after clear)
static void test11(void)
{
    printf("> test11\n");

    btr_ollist_s list = BTR_OLLIST(int, 1, 2, 3);
    assert(!BTR_OLList_isEmpty(&list));

    BTR_OLList_clear(&list);
    assert(BTR_OLList_isEmpty(&list));
    assert(BTR_OLList_len(&list) == 0);

    // reuse after clear
    *(int *)BTR_OLList_append(&list) = 42;
    assert(BTR_OLList_len(&list) == 1);
    assert(getInt(BTR_unwrap(BTR_OLList_get(&list, 0))) == 42);

    BTR_OLList_free(&list);
}

// test ENUMERATE macro
static void test12(void)
{
    printf("> test12\n");

    const int VALUES[] = {7, 14, 21, 28};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_ollist_s list = BTR_OLList_makeFrom((void *)VALUES, N, sizeof(int), NULL);

    size_t count = 0;
    BTR_OLLIST_ENUMERATE(&list, item, index)
    {
        assert(getInt(item) == VALUES[index]);
        count++;
    }
    assert(count == N);

    BTR_OLList_free(&list);
}

// test `toArray`
static void test13(void)
{
    printf("> test13\n");

    const int VALUES[] = {10, 20, 30, 40, 50};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_ollist_s list = BTR_OLLIST(int, 10, 20, 30, 40, 50);
    assert(BTR_OLList_len(&list) == N);

    int *arr = (int *)BTR_OLList_toArray(&list, NULL);
    assert(arr != NULL);
    for (size_t i = 0; i < N; i++)
        assert(arr[i] == VALUES[i]);

    BTR_Allocator_deallocate(list.allocator, arr);
    BTR_OLList_free(&list);
}

// test `toArray` on empty list
static void test14(void)
{
    printf("> test14\n");

    btr_ollist_s list = BTR_OLList_make(sizeof(int), NULL);
    assert(BTR_OLList_len(&list) == 0);

    int *arr = (int *)BTR_OLList_toArray(&list, NULL);
    assert(arr != NULL);

    BTR_Allocator_deallocate(list.allocator, arr);
    BTR_OLList_free(&list);
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
    printf("SUCCESS\n");
    return 0;
}
