#include "btrstd/allocators/pool.h"

#include "stdio.h"
#include "assert.h"


int main(void)
{
    btr_pool_t pool = BTR_Pool_make(sizeof(int), 256, NULL);
    BTR_Pool_destroy(&pool);
    printf("SUCCESS\n");
}
