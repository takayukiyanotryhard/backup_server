#include <stdio.h>
#include "log.h"

FILE* g_log_fp;
char g_log_buf[FILENAME_MAX];

void log_initialize() {
    g_log_fp = fopen("log.log", "a");
    log("=============== log initialized ==============");
}

void log_finalize(){
    if (g_log_fp) {
        fclose(g_log_fp);
    }
}
