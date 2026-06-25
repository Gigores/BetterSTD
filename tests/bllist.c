#include "btrstd/containers/borrowing_linked_list.h"
#include "assert.h"
#include "stdio.h"

static int getInt(void *p)
{
    return *(int *)p;
}
static void checkList(btr_bllist_s *list, int expected[], size_t n)
{
    assert(list->size == n);
    btr_bllist_node_s *node = list->head;
    for (size_t i = 0; i < n; i++) {
        assert(node != NULL);
        assert(getInt(node->payload) == expected[i]);
        node = node->next;
    }
    assert(node == NULL);
}
static bool cmp_int(const void *a, const void *b)
{
    return *(const int *)a == *(const int *)b;
}

// test `new`, and `get`
static void test1(void)
{
    printf("> test1\n");

    const int VALUES[] = {
        0, 10, 20, 30, 40, 50, 60, 70, 80, 90
    };
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_bllist_s list = BTR_BLLIST(
        (void *)&VALUES[0],
        (void *)&VALUES[1],
        (void *)&VALUES[2],
        (void *)&VALUES[3],
        (void *)&VALUES[4],
        (void *)&VALUES[5],
        (void *)&VALUES[6],
        (void *)&VALUES[7],
        (void *)&VALUES[8],
        (void *)&VALUES[9]
    );
    // forward iteration
    size_t i = 0;
    BTR_BLLIST_FOREACH(&list, node)
    {
        assert(getInt(node) == VALUES[i]);
        i++;
    }
    assert(i == N);
    // negative indexing check
    for (long k = -1; k >= -(long)N; k--)
    {
        int v = getInt(BTR_unwrap(BTR_BLList_get(&list, k)));
        assert(v == VALUES[N + k]);
    }
    BTR_BLList_free(&list);
}
// test `append` and `pop`
static void test2(void)
{
    printf("> test2\n");

    const int INPUT[] = {
        654, 64357, 2475, 7465
    };
    const int EXPECT_AFTER_POP[] = {
        654, 7465
    };

    btr_bllist_s list = BTR_BLList_make(NULL);
    size_t n = sizeof(INPUT)/sizeof(INPUT[0]);
    for (size_t i = 0; i < n; i++)
        BTR_BLList_append(&list, (void *)&INPUT[i]);
    int a = getInt(BTR_unwrap(BTR_BLList_pop(&list, 1)));
    int b = getInt(BTR_unwrap(BTR_BLList_pop(&list, 1)));
    assert(a == INPUT[1]);
    assert(b == INPUT[2]);
    checkList(&list, (int *)EXPECT_AFTER_POP, 2);
    int c = getInt(BTR_unwrap(BTR_BLList_pop(&list, -1)));
    int d = getInt(BTR_unwrap(BTR_BLList_pop(&list, -1)));
    assert(c == INPUT[3]);
    assert(d == INPUT[0]);
    assert(list.size == 0);
    BTR_BLList_free(&list);
}
// test 'clone'
static void test3(void)
{
    printf("> test3\n");

    const int VALUES[] = {0,10,20,30,40,50,60,70,80,90};
    const size_t N = sizeof(VALUES)/sizeof(VALUES[0]);
    btr_bllist_s list = BTR_BLLIST(
        (void *)&VALUES[0],
        (void *)&VALUES[1],
        (void *)&VALUES[2],
        (void *)&VALUES[3],
        (void *)&VALUES[4],
        (void *)&VALUES[5],
        (void *)&VALUES[6],
        (void *)&VALUES[7],
        (void *)&VALUES[8],
        (void *)&VALUES[9]
    );
    btr_bllist_s clone = BTR_BLList_clone(&list, NULL);
    assert(clone.size == list.size);
    for (size_t i = 0; i < N; i++)
    {
        int a = getInt(BTR_unwrap(BTR_BLList_get(&list, i)));
        int b = getInt(BTR_unwrap(BTR_BLList_get(&clone, i)));
        assert(a == b);
    }
    BTR_BLList_free(&list);
    BTR_BLList_free(&clone);
}
// test `prepend`
static void test4(void)
{
    printf("> test4\n");

    const int VALUES[] = {
        0, 10, 20, 30, 40, 50, 60, 70, 80, 90
    };
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);
    btr_bllist_s list = BTR_BLList_make(NULL);
    for (size_t i = 0; i < N; i++)
        BTR_BLList_prepend(&list, (void *)&VALUES[i]);
    size_t i = 0;
    BTR_BLLIST_FOREACH(&list, node)
    {
        int v = getInt(node);
        int expected = VALUES[N - 1 - i];
        assert(v == expected);
        i++;
    }
    assert(i == N);
    BTR_BLList_free(&list);
}
// test `insert`
static void test5(void)
{
    printf("> test5\n");

    btr_bllist_s list = BTR_BLList_make(NULL);

    int a = 1, b = 2, c = 3, d = 4;

    // insert into empty list
    BTR_BLList_insert(&list, &a, 0);
    {
        int exp[] = {1};
        checkList(&list, exp, 1);
    }

    // append (index == size)
    BTR_BLList_insert(&list, &b, 1);
    {
        int exp[] = {1, 2};
        checkList(&list, exp, 2);
    }

    // insert in middle
    BTR_BLList_insert(&list, &c, 1);
    {
        int exp[] = {1, 3, 2};
        checkList(&list, exp, 3);
    }

    // insert at head
    BTR_BLList_insert(&list, &d, 0);
    {
        int exp[] = {4, 1, 3, 2};
        checkList(&list, exp, 4);
    }

    // negative index (-1 == before last, depending on your design)
    BTR_BLList_insert(&list, &a, -1);
    {
        int exp[] = {4, 1, 3, 1, 2};
        checkList(&list, exp, 5);
    }

    BTR_BLList_free(&list);
}
// test `first` & `last`
static void test6(void)
{
    printf("> test6\n");

    const int VALUES[] = {10, 20, 30, 40, 50};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_bllist_s list = BTR_BLList_make(NULL);

    // build list
    for (size_t i = 0; i < N; i++)
        BTR_BLList_append(&list, (void *)&VALUES[i]);

    // first element
    int first = *(int *)BTR_unwrap(BTR_BLList_first(&list));
    assert(first == VALUES[0]);

    // last element
    int last = *(int *)BTR_unwrap(BTR_BLList_last(&list));
    assert(last == VALUES[N - 1]);

    // structural consistency check
    checkList(&list, (int *)VALUES, N);

    // pop last and re-check
    int popped_last = *(int *)BTR_unwrap(BTR_BLList_pop(&list, -1));
    assert(popped_last == VALUES[N - 1]);

    const int VALUES_AFTER[] = {10, 20, 30, 40};
    checkList(&list, (int *)VALUES_AFTER, N - 1);

    // pop first and re-check
    int popped_first = *(int *)BTR_unwrap(BTR_BLList_pop(&list, 0));
    assert(popped_first == VALUES_AFTER[0]);

    const int VALUES_AFTER2[] = {20, 30, 40};
    checkList(&list, (int *)VALUES_AFTER2, N - 2);

    BTR_BLList_free(&list);
}
// test `indexOf`
static void test7(void)
{
    printf("> test7\n");

    const int VALUES[] = {
        10, 20, 30, 40, 50
    };
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_bllist_s list = BTR_BLList_make(NULL);

    // build list
    for (size_t i = 0; i < N; i++)
        BTR_BLList_append(&list, (void *)&VALUES[i]);

    // check all valid indices
    for (size_t i = 0; i < N; i++)
    {
        long idx = BTR_unwrap(BTR_BLList_indexOf(&list, (void *)&VALUES[i], cmp_int));
        assert(idx == (long)i);
    }

    // non-existing value
    int missing = 999;
    btr_container_idx_r not_found = BTR_BLList_indexOf(&list, &missing, cmp_int);

    assert(not_found.status == BTR_STATUS_ERR);
    assert(not_found.error == BTR_CONTAINER_ERR_NOT_FOUND);

    // duplicate case
    int dup = 30;
    BTR_BLList_append(&list, &dup);

    long first_occurrence = BTR_unwrap(BTR_BLList_indexOf(&list, &dup, cmp_int));

    assert(first_occurrence == 2); // first 30

    BTR_BLList_free(&list);
}
// test `reverse`
static void test8(void)
{
    printf("> test8\n");

    const int VALUES[] = {
        10, 20, 30, 40, 50
    };
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);
    btr_bllist_s list = BTR_BLList_make(NULL);

    for (size_t i = 0; i < N; i++)
        BTR_BLList_append(&list, (void *)&VALUES[i]);
    assert(list.size == N);
    BTR_BLList_reverse(&list);
    for (size_t i = 0; i < N; i++)
    {
        int v = *(int *)BTR_unwrap(BTR_BLList_get(&list, i));
        int expected = VALUES[N - 1 - i];
        assert(v == expected);
    }
    assert(list.size == N);
    BTR_BLList_reverse(&list);
    for (size_t i = 0; i < N; i++)
    {
        int v = *(int *)BTR_unwrap(BTR_BLList_get(&list, i));
        assert(v == VALUES[i]);
    }
    assert(list.size == N);
    BTR_BLList_free(&list);
}

// test `set`
static void test9(void)
{
    printf("> test9\n");

    int values[] = {10, 20, 30, 40, 50};
    size_t n = sizeof(values) / sizeof(values[0]);

    btr_bllist_s list = BTR_BLList_make(NULL);
    for (size_t i = 0; i < n; i++)
        BTR_BLList_append(&list, (void *)&values[i]);

    // set at positive index
    int newVal = 99;
    BTR_BLList_set(&list, &newVal, 2);
    assert(getInt(BTR_unwrap(BTR_BLList_get(&list, 2))) == 99);
    assert(getInt(BTR_unwrap(BTR_BLList_get(&list, 0))) == 10);
    assert(getInt(BTR_unwrap(BTR_BLList_get(&list, 4))) == 50);

    // set at negative index
    int first = 1, last = 100;
    BTR_BLList_set(&list, &last, -1);
    BTR_BLList_set(&list, &first, -5);
    assert(getInt(BTR_unwrap(BTR_BLList_get(&list, -1))) == 100);
    assert(getInt(BTR_unwrap(BTR_BLList_get(&list, -5))) == 1);

    // set at index 0
    int zero = 0;
    BTR_BLList_set(&list, &zero, 0);
    assert(getInt(BTR_unwrap(BTR_BLList_get(&list, 0))) == 0);

    BTR_BLList_free(&list);
}

// test `toArray`
static void test10(void)
{
    printf("> test10\n");

    const int VALUES[] = {10, 20, 30, 40, 50};
    const size_t N = sizeof(VALUES) / sizeof(VALUES[0]);

    btr_bllist_s list = BTR_BLList_make(NULL);
    for (size_t i = 0; i < N; i++)
        BTR_BLList_append(&list, (void *)&VALUES[i]);

    void **arr = BTR_BLList_toArray(&list, NULL);
    assert(arr != NULL);
    for (size_t i = 0; i < N; i++)
        assert(*(int *)arr[i] == VALUES[i]);

    BTR_Allocator_deallocate(list.allocator, arr);
    BTR_BLList_free(&list);
}

// test `toArray` on empty list
static void test11(void)
{
    printf("> test11\n");

    btr_bllist_s list = BTR_BLList_make(NULL);
    assert(list.size == 0);

    void **arr = BTR_BLList_toArray(&list, NULL);
    assert(arr != NULL);

    BTR_Allocator_deallocate(list.allocator, arr);
    BTR_BLList_free(&list);
}

int main(void) {
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
    printf("SUCCESS\n");

    return 0;
}
