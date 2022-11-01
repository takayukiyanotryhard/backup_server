#define _GNU_SOURCE

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "search.h"
#include "dictionary.h"

#define DEFAULT_SIZE 128

typedef struct str_ent_dummy {
    unsigned int used;
    ENTRY entry;
} ent_t;

dict_t *g_dicts = 0;
int g_dict_count = 0;
int *g_dict_data_count = 0;

dict_t *new_dict(int size) {
    dict_t *dict = 0;
    if (size <= 0) size = DEFAULT_SIZE;

    struct hsearch_data *tab = calloc(sizeof(struct hsearch_data), 1);
    int ret = hcreate_r(size, tab);

    if (!ret) {
        if (errno == ENOMEM) {
            perror("NOMEM\n");
        }
    } else {
        if (!g_dicts) {
            g_dicts = (dict_t*)malloc(sizeof(dict_t));
            g_dict_data_count = (int*)malloc(sizeof(int));
        } else {
            g_dicts = (dict_t*)realloc(g_dicts, sizeof(dict_t) * (g_dict_count + 1));
            g_dict_data_count = (int*)realloc(g_dict_data_count, sizeof(int) * (g_dict_count + 1));
        }
        dict = &g_dicts[g_dict_count];
        dict->dict = (void*)tab;
        dict->size = size;
        g_dict_count++;
    }
    return dict;
}
int dict_data_count(dict_t* dict) {
    if (!dict) return 0;
    if (!dict->dict) return 0;
    
    return ((struct hsearch_data*)dict->dict)->filled;
}

int dict_add_data(dict_t* d, char* key, char *data) {
    int result = -1;    
    int ret = 0;
    if (!d) goto ERROR;

    ENTRY e, *er;
    e.key = key;
    e.data = data;
    ret = hsearch_r(e, ENTER, &er, (struct hsearch_data*)d->dict);
    if (!ret) {
        perror("hash full.\n");
        goto ERROR;
    }
    result = 0;
    
ERROR:
    return result;
}
int dict_remove_data(dict_t* d, char* key) {
    return dict_add_data(d, key, 0);
}
char *dict_search(dict_t* d, char* key) {
    ENTRY e, *er;
    e.key = key;
    int ret = hsearch_r(e, FIND, &er, (struct hsearch_data*)d->dict);
    if (!ret) {
        printf("%s is not found.\n", key);
        return 0;
    } else {
        return er->data;
    }
}
char *dict_get_data(dict_t* d, int index, char** key) {
    struct hsearch_data* tab = d->dict;
    ent_t* table = (ent_t*)tab->table;
    ENTRY *entry;
    int check = index + 1;
    int j = 0;
    for (int i=0;i<tab->size;i++) {
        if (table[i].entry.data) {
            // printf("check:%d i:%d j:%d index:%d\n", check, i, j, index);
            j++;
            if (check == j) {   
                *key = table[i].entry.key;
                return table[i].entry.data;
            }
        }
    }
    
    printf("loop exit\n");
    return 0;
}

void free_dict(dict_t* dict) {
    if (!dict) return;
    if (!g_dicts) return;

    int found = 0;
    for(int i=0;i<g_dict_count;i++) {
        if (&g_dicts[i] == dict) {
            hdestroy_r((struct hsearch_data*)dict->dict);
            free(dict->dict);
            g_dict_count--;
            g_dict_data_count[i] = 0;
        }
        if (found && g_dict_count) {
            g_dicts[i] = g_dicts[i + 1];
            g_dict_data_count[i] = g_dict_data_count[i + 1];
        }
    }
    if (!g_dict_count) {
        free(g_dicts);
        g_dicts = 0;
    }
}
