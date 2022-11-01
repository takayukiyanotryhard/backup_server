#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <pthread.h>

typedef struct registered_ue {
    char* name;
    char* idevice;
    uint64_t macaddr;
    int valid;
} registered_ue_t;

typedef struct config {
    int threads_num;
    char* stored_directory;
    registered_ue_t* registered;
} config_t;

config_t* load_config();
pthread_t launch_setting_receptor();

#endif
