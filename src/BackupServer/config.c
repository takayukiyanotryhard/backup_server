#include <sys/utsname.h>
#include <search.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/event.h>
#include "config.h"
#include "log.h"
#include "stdlib.h"
#include "../libcxml/libcxml.h"
#include "../libdictionary/dictionary.h"

#define SFILE "/System/Library/CoreServices/SystemVersion.plist"
#define SETTING_PORT 2200

void* general_command_loop_v4(void *arg);

struct hsearch_data* g_os = 0;
dict_t* g_hash = 0;


void load_os() {
    char *key;
    char *data;

    char *pname, *ver, *build;
    xml_t *xml = xml_new(SFILE, NULL);
    if (!xml) {
        printf("xml load failed.\n");
        goto ERROR;
    }

    node_t *plist = xml->get_node("plist");
    if (!plist) {
        printf("plist not found.\n");
        goto ERROR;
    }
    node_t *dict = plist->child("dict", 0, plist);
    if (!dict) {
        printf("dict not found.\n");
        goto ERROR;
    }

    int count = dict->child_count("key", dict);
    g_hash = new_dict(10);
    // printf("found %d values\n", count);
    for(int i=0;i<count;i++){
//        printf("%s\n", dict->child("key", i, dict)->value);
//        printf("%s\n", dict->child("string", i, dict)->value);
        dict_add_data(g_hash, dict->child("key", i, dict)->value, dict->child("string", i, dict)->value);
    }

    count = dict_data_count(g_hash);
    for(int i=0;i<count;i++) {
        data = dict_get_data(g_hash, i, &key);
        if (data)
            // printf("%d k:%s v:%s\n", i, key, data);
            ;
        else
            printf("error\n");
    }
    pname = dict_search(g_hash, "ProductName");
    ver = dict_search(g_hash, "ProductVersion");
    build = dict_search(g_hash, "ProductBuildVersion");
    log("%s %s build:%s", pname, ver, build);
ERROR:
    xml_free(xml);
    
}

config_t* load_config() {
    config_t* config = malloc(sizeof(config_t));
    if (!config) {
        log("load config failed.\n");
        goto ERROR;
    }

    struct utsname ubuf;
    if (uname(&ubuf)){
        free(config);
        config = (void*)0;
        goto ERROR;
    }

    log("%s", ubuf.version);

    load_os();
ERROR:
    return config;
}

pthread_t launch_setting_receptor() {
    // スレッドの起動
    pthread_t handle;
    log("in");
    pthread_create(&handle, NULL, general_command_loop_v4, NULL);
    
    // ループバックしか受け付け無いのでV4のみ起動
    return handle;    
}

/**
 * 
 */
void* general_command_loop_v4(void *arg) {
    int sock_optval = 1;
    int port = SETTING_PORT;
    struct sockaddr_in sin;
    int kq;
    struct kevent kev;

    int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &sock_optval, sizeof(sock_optval))) {
        log("error");
        perror("setsockopt");
        exit(1);
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listening_socket, (struct sockaddr*)&sin, sizeof(sin))){
        log("error");
        perror("bind");
        exit(1);
    }

    if (listen(listening_socket, SOMAXCONN)) {
        log("error");
        perror("listen");
        exit(1);
    }

    log("listening port %d", port);
    if ((kq = kqueue()) < 0) {
        log("error");
        perror("kqueue");
        exit(1);
    }
    EV_SET(&kev, listening_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &kev, 1, NULL, 0, NULL)) {
        log("error");
        perror("kevent");
        exit(1);
    }

    for(;;) {
        int n;
        struct timespec waitspec;
        waitspec.tv_sec = 2;
        waitspec.tv_nsec = 500000;

        n = kevent(kq, NULL, 0, &kev, 1, &waitspec);
        if (n == -1) {
            log("else");    
            perror("kevent");
            exit(1);
        } else if (n > 0 ){
            log("n:%d", n);
            if ( kev.ident == listening_socket ){
                                 /* 新しいクライアントがやってきた */
                 int new_sock = accept_new_client(kev.ident);
                 if ( new_sock != -1 ){
                                 /* 監視対象に新たなソケットを追加 */
                     EV_SET(&kev, new_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
                     n = kevent(kq, &kev, 1, NULL, 0, NULL);
                     if ( n == -1 ){
                         perror("kevent");
                         exit(1);
                     }
                 }
 
             } else {
                 int sock = kev.ident;
                 printf("ディスクリプタ %d 番が読み込み可能です。\n", sock);
                 read_and_reply(sock);
             }
        } else {
            log("else n:%d", n);
        }
        usleep(3 * 1000 * 1000);
    }
}
