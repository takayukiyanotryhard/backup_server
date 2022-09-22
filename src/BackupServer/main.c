#include <stdio.h>

#include "server.h"
#include "log.h"
#include "config.h"
#include "thread_pool.h"
#include "polling.h"
#include "procqueue.h"
#include "backup.h"

int main(void){
    int result;

    log_initialize();

    config_t* config = load_config();
    if (!config) return -1;

    // スレッドプールを生成
    result = make_thread(config);
    if (!result) return -1;

    // ポーリングスレッドの起動
    launch_polling(config);

    for(;;){
        // キューの取り出し
        queue_t* queue = pop();
        result = backup(config, queue);
        if (result == LATOR){
            push(queue);
        }
    }

    // ここには来ない
    return 0;
}
