#include "btrstd/containers/bit_set.h"

int main(void)
{
    btr_bit_set_t bitSet = BTR_BitSet_make(16, NULL);
    BTR_BitSet_free(&bitSet);
}
