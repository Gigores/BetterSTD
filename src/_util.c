#include "_util.h"
#include "btrstd/allocators/global.h"


static btr_allocator_t *getAllocator(btr_allocator_t *allocator)
{
    if (!allocator) return (btr_allocator_t *)BTR_getGlobalAllocator();
    return allocator;
}
