#include <stdio.h>

#include "backup.h"
#include "config.h"
#include "log.h"
#include "polling.h"
#include "procqueue.h"
#include "server.h"
#include "thread_pool.h"

int main(void) {
    int result;

    log_initialize();

    config_t *config = load_config();
    if (!config) return -1;

    // スレッドプールを生成
    result = make_thread(config->threads_num);
    if (result != OK) return -1;

    // ポーリングスレッドの起動
    launch_polling(config);

    // 設定変更受付スレッドの起動
    launch_setting_receptor();

    for (;;) {
        // キューの取り出し
        queue_t *queue = pop();
        result = backup(config, queue);
        if (result == LATOR) {
            push(queue);
        }
    }

    // ここには来ない
    return 0;
}
