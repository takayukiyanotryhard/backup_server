#include <stdio.h>
#include <stdlib.h>

#include "backup.h"
#include "config.h"
#include "log.h"
#include "polling.h"
#include "procqueue.h"
#include "server.h"
#include "thread_pool.h"


#define FUNC_MAX 5
typedef enum {
    NOTICE_NONE,
    DEVICE_CONNECTED,
    CONFIG_CHANGED,
    NOTICE_KIND_MAX,
}MAIN_NOTICE_KIND ;

void polling_callback(int iptype, uint64_t address);

int dummy_func(config_t* config,int length, void* args,...);
int OnConnectedDevice(config_t* config,int length, void* args);

static int (*funcs[NOTICE_KIND_MAX])(config_t*,int length, void* args,...);

void init_func() {
    // ここはenumにする
    funcs[NOTICE_NONE] = dummy_func;
    funcs[DEVICE_CONNECTED] = dummy_func;
    funcs[CONFIG_CHANGED] = dummy_func;
}

int main(void) {
    int result;

    log_initialize();

    config_t *config = load_config();
    if (!config) return -1;

    // スレッドプールを生成
    result = make_thread(config->threads_num);
    if (result != OK) return -1;

    // ポーリングスレッドの起動
    launch_polling(config, polling_callback);

    // 設定変更受付スレッドの起動
    launch_setting_receptor();

    for (;;) {
        // キューの取り出し
        queue_t *queue = pop();

        result = funcs[queue->kind](config,queue->length,  queue->args);
        // result = backup(config, queue);
        if (result == LATOR) {
            push(queue);
        }
    }

    // ここには来ない
    return 0;
}


void polling_callback(int iptype, uint64_t address) {
    queue_t queue;
    queue.kind = DEVICE_CONNECTED;
    int size = sizeof(arg_t) + sizeof(iptype_t);
    queue.args = (arg_t*)malloc(size);
    if (!(queue.args)) {
        log("malloc failed.");
        return;
    }
    queue.args->size = size;
    iptype_t *ip = (iptype_t*)queue.args->data;
    ip->address = address;
    ip->type = iptype;
}


int dummy_func(config_t* config, int length, void* args,...) {
    return 0;
}
