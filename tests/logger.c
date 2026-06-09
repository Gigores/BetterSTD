#include "btrstd/logger.h"

int main(void)
{
    BTR_setLogger((btr_logger_t) {
        .file = fopen("log.log", "w+"),
        .logTime = true,
        .logFile = true,
        .logFunc = true,
        .logLine = true,
    });
    BTR_log(LOG_DEBUG, "this is a test log");
    BTR_log(LOG_INFO, "this is a test log");
    BTR_log(LOG_WARN, "this is a test log");
    BTR_log(LOG_ERROR, "this is a test log");
    BTR_log(LOG_FATAL, "this is a test log");
    fclose(BTR_getLogger()->file);
    return 0;
}
