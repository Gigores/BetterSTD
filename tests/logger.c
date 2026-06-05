#include "btrstd/logger.h"

int main(void)
{
    btr_logger_t logger = {
        .file = fopen("log.log", "w+"),
    };
    BTR_Logger_log(&logger, LOG_DEBUG, "this is a test log");
    BTR_Logger_log(&logger, LOG_INFO, "this is a test log");
    BTR_Logger_log(&logger, LOG_WARN, "this is a test log");
    BTR_Logger_log(&logger, LOG_ERROR, "this is a test log");
    BTR_Logger_log(&logger, LOG_FATAL, "this is a test log");
    fclose(logger.file);
    return 0;
}
