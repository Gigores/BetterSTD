#include "btrstd/logger.h"

int main(void)
{
    BTR_setLogger((btr_logger_s) {
        .file = fopen("log.log", "w+"),
        .logTime = true,
        .logFile = true,
        .logFunc = true,
        .logLine = true,
    });
    BTR_log(BTR_LOG_DEBUG, "this is a test log");
    BTR_log(BTR_LOG_INFO, "this is a test log");
    BTR_log(BTR_LOG_WARN, "this is a %s log", "test");
    BTR_log(BTR_LOG_ERROR, "this is a %d log", 12);
    BTR_log(BTR_LOG_FATAL, "this is a test log");
    int var = 15;
    BTR_debugInt(var);
    double var2 = 15.312;
    BTR_debugFloat(var2);
    const char *var3 = "hell yeah";
    BTR_debugString(var3);
    fclose(BTR_getLogger()->file);
    return 0;
}
