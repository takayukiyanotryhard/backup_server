#include <stdio.h>
#include "log.h"

const char LOG_FILE[]="/tmp/backup_server.log";
FILE* g_log_fp = 0;
char g_log_buf[FILENAME_MAX];

void log_initialize() {
    g_log_fp = fopen(LOG_FILE, "a");
    log("=============== log initialized ==============");
}

void log_finalize(){
    if (g_log_fp) {
        fclose(g_log_fp);
    }
}
