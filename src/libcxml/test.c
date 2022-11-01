#include <stdio.h>
#include <stdlib.h>

#include "libcxml.h"

#define BUFSIZE 0x10000
#define SFILE "/System/Library/CoreServices/SystemVersion.plist"

int main(void){
    xml_t *xml = xml_new(SFILE, NULL);
    if (!xml) {
        printf("xml load failed.\n");
        goto ERROR;
    }

//    printf("xml:%p\n", xml);
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
//    printf("dict:%p\n", dict);

    int count = dict->child_count("key", dict);
    printf("found %d values\n", count);
    for(int i=0;i<count;i++){
        printf("%s\n", dict->child("key", i, dict)->value);
        printf("%s\n", dict->child("string", i, dict)->value);
    }
ERROR:
    xml_free(xml);
    return 0;
}
