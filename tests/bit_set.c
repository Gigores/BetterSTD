#include "btrstd/containers/bit_set.h"
#include "assert.h"
#include "stdio.h"

static void test1(void)
{
    printf("> test1\n");

    btr_bit_set_s bs = BTR_BitSet_make(0, NULL);
    BTR_BitSet_free(&bs);

    bs = BTR_BitSet_make(1, NULL);
    BTR_BitSet_free(&bs);

    bs = BTR_BitSet_make(64, NULL);
    BTR_BitSet_free(&bs);

    bs = BTR_BitSet_make(1024, NULL);
    BTR_BitSet_free(&bs);
}

static void test2(void)
{
    printf("> test2\n");

    btr_bit_set_s bs = BTR_BitSet_make(16, NULL);

    // all bits should start as 0
    for (long i = 0; i < 16; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, i)) == 0);

    // set individual bits
    for (long i = 0; i < 16; i += 2)
        BTR_BitSet_set(&bs, i);

    // verify even bits are 1, odd bits are 0
    for (long i = 0; i < 16; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, i)) == (i % 2 == 0));

    BTR_BitSet_free(&bs);
}

static void test3(void)
{
    printf("> test3\n");

    btr_bit_set_s bs = BTR_BitSet_make(8, NULL);

    BTR_BitSet_set(&bs, 0);
    BTR_BitSet_set(&bs, 1);
    BTR_BitSet_set(&bs, 2);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 0)) == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 1)) == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 2)) == 1);

    BTR_BitSet_unset(&bs, 1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 0)) == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 1)) == 0);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 2)) == 1);

    BTR_BitSet_unset(&bs, 0);
    BTR_BitSet_unset(&bs, 2);
    for (long i = 0; i < 8; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, i)) == 0);

    BTR_BitSet_free(&bs);
}

static void test4(void)
{
    printf("> test4\n");

    btr_bit_set_s bs = BTR_BitSet_make(8, NULL);

    // flip even bits
    for (long i = 0; i < 8; i += 2)
    {
        btr_bit_r r = BTR_BitSet_flip(&bs, i);
        assert(r.status == BTR_STATUS_OK);
        assert(r.value == 1);
    }

    for (long i = 0; i < 8; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, i)) == (i % 2 == 0));

    // flip them back
    for (long i = 0; i < 8; i += 2)
    {
        btr_bit_r r = BTR_BitSet_flip(&bs, i);
        assert(r.status == BTR_STATUS_OK);
        assert(r.value == 0);
    }

    for (long i = 0; i < 8; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, i)) == 0);

    BTR_BitSet_free(&bs);
}

static void test5(void)
{
    printf("> test5\n");

    btr_bit_set_s bs = BTR_BitSet_make(8, NULL);

    // -1 is last bit (index 7), -8 is first bit (index 0)
    BTR_BitSet_set(&bs, -1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 7)) == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, -1)) == 1);

    BTR_BitSet_set(&bs, -8);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 0)) == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, -8)) == 1);

    BTR_BitSet_unset(&bs, -1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, -1)) == 0);

    btr_bit_r r = BTR_BitSet_flip(&bs, -5);
    assert(r.status == BTR_STATUS_OK);
    assert(r.value == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 3)) == 1);

    BTR_BitSet_free(&bs);
}

static void test6(void)
{
    printf("> test6\n");

    btr_bit_set_s bs = BTR_BitSet_make(4, NULL);

    // get out of bounds
    btr_bit_r r = BTR_BitSet_get(&bs, 4);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    r = BTR_BitSet_get(&bs, -5);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    // flip out of bounds
    r = BTR_BitSet_flip(&bs, 4);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    r = BTR_BitSet_flip(&bs, -5);
    assert(r.status == BTR_STATUS_ERR);
    assert(r.error == BTR_CONTAINER_ERR_OUT_OF_BOUNDS);

    BTR_BitSet_free(&bs);
}

static void test7(void)
{
    printf("> test7\n");

    btr_bit_set_s bs = BTR_BitSet_make(16, NULL);

    // set all 8 bits in the first byte
    for (long i = 0; i < 8; i++)
        BTR_BitSet_set(&bs, i);

    // verify all 8 are set
    for (long i = 0; i < 8; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, i)) == 1);

    // check that bits in second byte are still 0
    for (long i = 8; i < 16; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, i)) == 0);

    // unset middle bits
    BTR_BitSet_unset(&bs, 3);
    BTR_BitSet_unset(&bs, 4);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 2)) == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 3)) == 0);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 4)) == 0);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 5)) == 1);

    // set bits in second byte
    BTR_BitSet_set(&bs, 15);
    assert(BTR_unwrap(BTR_BitSet_get(&bs, 15)) == 1);

    BTR_BitSet_free(&bs);
}

static void test8(void)
{
    printf("> test8\n");

    size_t nBits = 128;
    btr_bit_set_s bs = BTR_BitSet_make(nBits, NULL);

    // set all bits
    for (size_t i = 0; i < nBits; i++)
        BTR_BitSet_set(&bs, (long)i);

    for (size_t i = 0; i < nBits; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, (long)i)) == 1);

    // unset all bits
    for (size_t i = 0; i < nBits; i++)
        BTR_BitSet_unset(&bs, (long)i);

    for (size_t i = 0; i < nBits; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&bs, (long)i)) == 0);

    BTR_BitSet_free(&bs);
}

static void test9(void)
{
    printf("> test9\n");

    // test with sizes that aren't multiples of 8
    btr_bit_set_s bs = BTR_BitSet_make(11, NULL);

    for (long i = 0; i < 11; i++)
    {
        BTR_BitSet_set(&bs, i);
        assert(BTR_unwrap(BTR_BitSet_get(&bs, i)) == 1);
    }

    // bit 11 should be out of bounds
    assert(BTR_BitSet_get(&bs, 11).status == BTR_STATUS_ERR);
    assert(BTR_BitSet_flip(&bs, 11).status == BTR_STATUS_ERR);

    BTR_BitSet_free(&bs);
}

static void test10(void)
{
    printf("> test10\n");

    // clone an empty bitset
    btr_bit_set_s empty = BTR_BitSet_make(8, NULL);
    btr_bit_set_s c = BTR_BitSet_clone(&empty, NULL);
    assert(c.bitCount == empty.bitCount);
    for (long i = 0; i < 8; i++)
        assert(BTR_unwrap(BTR_BitSet_get(&c, i)) == 0);
    BTR_BitSet_free(&c);
    BTR_BitSet_free(&empty);

    // clone a bitset with bits set
    btr_bit_set_s orig = BTR_BitSet_make(16, NULL);
    for (long i = 0; i < 16; i += 3)
        BTR_BitSet_set(&orig, i);

    btr_bit_set_s clone = BTR_BitSet_clone(&orig, NULL);
    assert(clone.bitCount == orig.bitCount);

    for (long i = 0; i < 16; i++)
    {
        int origBit = BTR_unwrap(BTR_BitSet_get(&orig, i));
        int cloneBit = BTR_unwrap(BTR_BitSet_get(&clone, i));
        assert(origBit == cloneBit);
    }

    // modify clone independently
    BTR_BitSet_set(&clone, 1);
    BTR_BitSet_unset(&clone, 0);
    assert(BTR_unwrap(BTR_BitSet_get(&orig, 1)) == 0);
    assert(BTR_unwrap(BTR_BitSet_get(&orig, 0)) == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&clone, 1)) == 1);
    assert(BTR_unwrap(BTR_BitSet_get(&clone, 0)) == 0);

    BTR_BitSet_free(&clone);
    BTR_BitSet_free(&orig);
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
