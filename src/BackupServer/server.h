#ifndef __SERVER_H__
#define __SERVER_H__

#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define OK 0
#define NG -1

extern FILE* g_log_fp;
extern char g_log_buf[];

#ifndef log
#define log(...)                                                          \
    do {                                                                  \
        if (g_log_fp) {                                                   \
            struct timespec ts;                                           \
            struct tm t;                                                  \
            uint64_t tid;                                                 \
            clock_gettime(CLOCK_REALTIME, &ts);                           \
            localtime_r(&ts.tv_sec, &t);                                  \
            pthread_threadid_np(NULL, &tid);                              \
            sprintf(g_log_buf, __VA_ARGS__);                              \
            fprintf(g_log_fp, "%d%02d%02d_%02d%02d%02d.%03d %5lld %s\n",          \
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, \
                    t.tm_min, t.tm_sec, (int16_t)(ts.tv_nsec / 1000000),  \
                    tid, g_log_buf);                                 \
            fflush(g_log_fp);                                             \
        }                                                                 \
    } while (0)

#endif
#endif
