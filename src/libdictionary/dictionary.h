#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
typedef struct str_dict {
    void *dict;    
    int size;
} dict_t;

dict_t *new_dict(int size);
int dict_data_count(dict_t*);
int dict_add_data(dict_t*, char* key, char *data);
int dict_remove_data(dict_t*, char*key);
char *dict_search(dict_t*, char* key);
char *dict_get_data(dict_t*, int i, char** key);

void free_dict(dict_t*);
#endif 

