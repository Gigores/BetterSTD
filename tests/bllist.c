#include "btrstd/bllist.h"
#include "assert.h"
#include "stdio.h"

// test `new`, and `get`
void test1(void)
{
    printf("> test1\n");
    const int VALUES[] = {
        0, 10, 20, 30, 40, 50, 60, 70, 80, 90
    };
    btr_bllist_t testList = BTR_BLLIST(
        (void *)&VALUES[0],
        (void *)&VALUES[1],
        (void *)&VALUES[2],
        (void *)&VALUES[3],
        (void *)&VALUES[4],
        (void *)&VALUES[5],
        (void *)&VALUES[6],
        (void *)&VALUES[7],
        (void *)&VALUES[8],
        (void *)&VALUES[9],
    );

    BTR_BLLIST_ENUMERATE(&testList, node, i) {
        printf("%zu: %d\n", i, *(int *)node);
        assert(*(int *)node == VALUES[i]);
    }
    for (long i = -1; i > -(long)testList.size - 1; i--) {
        printf("%ld: %d\n", i, *(int *)BTR_BLList_get(&testList, i));
    }
    BTR_BLList_free(&testList);
}
// test `append` and `pop`
void test2(void)
{
    printf("> test2\n");
    const int INPUT[] = {
        654, 64357, 2475, 7465
    };
    const int OUTPUT[] = {
        654, 7465
    };
    btr_bllist_t testList = {0};

    for (size_t i = 0; i < sizeof(INPUT) / sizeof(INPUT[0]); i++)
        BTR_BLList_append(&testList, (void *)&INPUT[i]);

    int a = *(int *)BTR_BLList_pop(&testList, 1);
    int b = *(int *)BTR_BLList_pop(&testList, 1);

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    assert(a == INPUT[1]);
    assert(b == INPUT[2]);
    BTR_BLLIST_ENUMERATE(&testList, node, i) {
        printf("%zu: %d\n", i, *(int *)node);
        assert(*(int *)node == OUTPUT[i]);
    }
    int c = *(int *)BTR_BLList_pop(&testList, -1);
    int d = *(int *)BTR_BLList_pop(&testList, -1);
    printf("c = %d\n", c);
    printf("d = %d\n", d);
    assert(c == INPUT[3]);
    assert(d == INPUT[0]);
    assert(testList.size == 0);
    BTR_BLList_free(&testList);
}
// test 'clone'
void test3(void)
{
    printf("> test3\n");
    const int VALUES[] = {
        0, 10, 20, 30, 40, 50, 60, 70, 80, 90
    };
    btr_bllist_t testList = BTR_BLLIST(
        (void *)&VALUES[0],
        (void *)&VALUES[1],
        (void *)&VALUES[2],
        (void *)&VALUES[3],
        (void *)&VALUES[4],
        (void *)&VALUES[5],
        (void *)&VALUES[6],
        (void *)&VALUES[7],
        (void *)&VALUES[8],
        (void *)&VALUES[9],
    );
    btr_bllist_t testListCloned = BTR_BLList_clone(&testList);
    size_t listSize = BTR_BLList_len(&testList);
    for (size_t i = 0; i < listSize; i++)
    {
        int a = *(int *)BTR_BLList_get(&testList, i);
        int b = *(int *)BTR_BLList_get(&testListCloned, i);
        printf("%zu: %d / %d\n", i, a, b);
        assert(a == b);
    }
    BTR_BLList_free(&testList);
    BTR_BLList_free(&testListCloned);
}
// test `prepend`
void test4(void)
{
    printf("> test4\n");
    const int VALUES[] = {
        0, 10, 20, 30, 40, 50, 60, 70, 80, 90
    };
    const size_t VALUES_SIZE = sizeof(VALUES) / sizeof(VALUES[0]);
    btr_bllist_t testList = {0};
    for (size_t i = 0; i < VALUES_SIZE; i++)
        BTR_BLList_prepend(&testList, (void *)&VALUES[i]);
    BTR_BLLIST_ENUMERATE(&testList, i, n)
    {
        int a = *(int *)i;
        int b = VALUES[VALUES_SIZE - (n + 1)];
        printf("%zu: %d / %d\n", n, a, b);
        assert(a == b);
    }
    BTR_BLList_free(&testList);
}

int main(void) {
    test1();
    test2();
    test3();
    test4();
    printf("SUCCESS\n");
    return 0;
}
