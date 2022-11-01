#include <sys/utsname.h>
#include <search.h>
#include "config.h"
#include "log.h"
#include "stdlib.h"
#include "../libcxml/libcxml.h"
#include "../libdictionary/dictionary.h"

#define SFILE "/System/Library/CoreServices/SystemVersion.plist"

struct hsearch_data* g_os = 0;

dict_t* g_hash = 0;
void load_os() {
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
    printf("found %d values\n", count);
    for(int i=0;i<count;i++){
//        printf("%s\n", dict->child("key", i, dict)->value);
//        printf("%s\n", dict->child("string", i, dict)->value);
        dict_add_data(g_hash, dict->child("key", i, dict)->value, dict->child("string", i, dict)->value);
    }

    count = dict_data_count(g_hash);
    printf("data:%d\n", count);
    for(int i=0;i<count;i++) {
        char *key;
        char *data = dict_get_data(g_hash, i, &key);
        if (data)
        printf("%d v:%s\n", i, data);
        else
        printf("error\n");
    }

ERROR:
    xml_free(xml);
    
}

config_t* load_config(){
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
