#include "btrstd/logger.h"

int main(void)
{
    btr_logger_t logger = {
        .file = fopen("log.log", "w+"),
    };
    BTR_Logger_log(&logger, LOG_DEBUG, "Test", "this is a test log");
    BTR_Logger_log(&logger, LOG_INFO, "Test", "this is a test log");
    BTR_Logger_log(&logger, LOG_WARN, "Test", "this is a test log");
    BTR_Logger_log(&logger, LOG_ERROR, "Test", "this is a test log");
    BTR_Logger_log(&logger, LOG_FATAL, "Test", "this is a test log");
    fclose(logger.file);
    return 0;
}
