#include "btrstd/smart_pointers.h"
#include "btrstd/logger.h"
#include "btrstd/allocators/global.h"


int main(void)
{
    BTR_autoFile file = fopen("log.log", "w+");
    BTR_setLogger((btr_logger_t) {
        .file = file,
    });
    BTR_log(LOG_INFO, "We are logging!");

    BTR_autoHeap(int) BUFFER = BTR_unwrap(BTR_allocate(sizeof(int) * 1024));

    return 0;
}

