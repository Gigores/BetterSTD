#include "_util.h"
#include "btrstd/allocators/global.h"


static btr_allocator_s *getAllocator(btr_allocator_s *allocator)
{
    if (!allocator) return (btr_allocator_s *)BTR_getGlobalAllocator();
    return allocator;
}
