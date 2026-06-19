#include "btrstd/allocators/arena.h"
#include "btrstd/error.h"
#include "assert.h"
#include "stdio.h"
#include "string.h"

static void test1(void)
{
    printf("> test1\n");

    btr_arena_s arena = BTR_Arena_make(1024, NULL);
    assert(arena.data != NULL);
    assert(arena.capacity == 1024);
    assert(arena.next == 0);

    void *p1 = BTR_unwrap(BTR_Arena_allocate(&arena, 64));
    assert(p1 != NULL);
    assert(p1 == arena.data);
    assert(arena.next == 64);

    void *p2 = BTR_unwrap(BTR_Arena_allocate(&arena, 128));
    assert(p2 != NULL);
    assert(p2 == (char *)arena.data + 64);
    assert(arena.next == 192);

    memset(p1, 0xAB, 64);
    memset(p2, 0xCD, 128);

    for (size_t i = 0; i < 64; i++)
        assert(((unsigned char *)p1)[i] == 0xAB);
    for (size_t i = 0; i < 128; i++)
        assert(((unsigned char *)p2)[i] == 0xCD);

    BTR_Arena_destroy(&arena);
}

static void test2(void)
{
    printf("> test2\n");

    btr_arena_s arena = BTR_Arena_make(100, NULL);

    void *p = BTR_unwrap(BTR_Arena_allocate(&arena, 100));
    assert(p != NULL);
    assert(arena.next == 100);

    btr_alloc_r r = BTR_Arena_allocate(&arena, 1);
    assert(r.status == BTR_ERR);
    assert(r.error == BTR_ALLOC_ERR_OUT_OF_MEMORY);

    BTR_Arena_destroy(&arena);
}

static void test3(void)
{
    printf("> test3\n");

    btr_arena_s arena = BTR_Arena_make(64, NULL);

    void *p1 = BTR_unwrap(BTR_Arena_allocate(&arena, 64));
    assert(p1 != NULL);
    assert(arena.next == 64);

    btr_alloc_r r = BTR_Arena_allocate(&arena, 1);
    assert(r.status == BTR_ERR);

    BTR_Arena_reset(&arena, 256);
    assert(arena.capacity == 256);
    assert(arena.next == 0);
    assert(arena.data != NULL);

    void *p2 = BTR_unwrap(BTR_Arena_allocate(&arena, 256));
    assert(p2 != NULL);
    assert(arena.next == 256);

    BTR_Arena_destroy(&arena);
}

static void test4(void)
{
    printf("> test4\n");

    btr_arena_s arena = BTR_Arena_make(512, NULL);
    btr_allocator_s wrapper = BTR_Arena_getWrapper(&arena);

    void *p1 = BTR_unwrap(BTR_Allocator_allocate(&wrapper, 128));
    assert(p1 == arena.data);
    assert(arena.next == 128);

    void *p2 = BTR_unwrap(BTR_Allocator_allocate(&wrapper, 256));
    assert(p2 == (char *)arena.data + 128);
    assert(arena.next == 384);

    memset(p1, 0xAA, 128);
    memset(p2, 0xBB, 256);
    assert(((unsigned char *)p1)[0] == 0xAA);
    assert(((unsigned char *)p2)[0] == 0xBB);

    BTR_Arena_destroy(&arena);
}

static void test5(void)
{
    printf("> test5\n");

    btr_arena_s arena = BTR_Arena_make(64, NULL);
    btr_allocator_s wrapper = BTR_Arena_getWrapper(&arena);

    void *p = BTR_unwrap(BTR_Allocator_allocate(&wrapper, 32));
    assert(p != NULL);

    btr_alloc_r r = BTR_Allocator_reallocate(&wrapper, p, 64);
    assert(r.status == BTR_ERR);
    assert(r.error == BTR_ALLOC_ERR_UNSUPPORTED_OPERATION);

    BTR_Arena_destroy(&arena);
}

static void test6(void)
{
    printf("> test6\n");

    btr_arena_s arena = BTR_Arena_make(64, NULL);
    btr_allocator_s wrapper = BTR_Arena_getWrapper(&arena);

    BTR_Allocator_deallocate(&wrapper, NULL);
    BTR_Allocator_deallocate(&wrapper, (void *)0x1234);

    assert(arena.next == 0);

    BTR_Arena_destroy(&arena);
}

int main(void) {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    printf("SUCCESS\n");
    return 0;
}
