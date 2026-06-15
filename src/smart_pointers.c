#include "btrstd/smart_pointers.h"
#include "btrstd/allocators/global.h"

void BTR_autoFileCleanup(FILE **file)
{
    if (*file)
        fclose(*file);
}
void BTR_autoHeapCleanup(void *p)
{
    if (p)
        BTR_deallocate(*(void **)p);
}
