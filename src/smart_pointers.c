#include "btrstd/smart_pointers.h"

#include "stdlib.h"

void BTR_autoFileCleanup(FILE **file)
{
    fclose(*file);
}
void BTR_autoHeapCleanup(void *p)
{
    free(*(void **)p);
}
