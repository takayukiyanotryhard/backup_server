#include <stdio.h>

#include "backup.h"
#include "config.h"
#include "log.h"
#include "polling.h"
#include "procqueue.h"
#include "server.h"
#include "thread_pool.h"


#define FUNC_MAX 5

void polling_callback(int iptype, uint64_t address);

int dummy_func(config_t* config, void* args,...);

static int (*funcs[FUNC_MAX])(config_t*, void* args,...);

void init_func() {
    // ここはenumにする
    funcs[0] = dummy_func;
    funcs[1] = dummy_func;
    funcs[2] = dummy_func;
    funcs[3] = dummy_func;
    funcs[4] = dummy_func;
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

        result = funcs[queue->kind](config, 0);
        // result = backup(config, queue);
        if (result == LATOR) {
            push(queue);
        }
    }

    // ここには来ない
    return 0;
}


void polling_callback(int iptype, uint64_t address) {
    // キューに検知を積む
}

int dummy_func(config_t* config, void* args,...) {
    return 0;
}
