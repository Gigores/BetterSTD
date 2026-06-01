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

    BTR_BLLIST_ENUMERATE(testList, node, i) {
        printf("%zu: %d\n", i, *(int *)node->payload);
        assert(*(int *)node->payload == VALUES[i]);
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
    BTR_BLLIST_ENUMERATE(testList, node, i) {
        printf("%zu: %d\n", i, *(int *)node->payload);
        assert(*(int *)node->payload == OUTPUT[i]);
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

int main(void) {
    test1();
    test2();
    printf("SUCCESS\n");
    return 0;
}
