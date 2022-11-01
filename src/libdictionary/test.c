#include <stdio.h>
#include "dictionary.h"

int main(void) {
    dict_t *dict = new_dict(5);
    printf("dict:%p\n", dict);

    int count = dict_data_count(dict);
    printf("now:%d\n", count);
    dict_add_data(dict, "a", "abc");
    count = dict_data_count(dict);
    printf("now:%d\n", count);

    char * data = dict_search(dict, "a");
    printf("data:%s\n",data);

    dict_add_data(dict, "c", "cba");
    dict_add_data(dict, "b", "bba");
    dict_add_data(dict, "a", "abc");
    dict_add_data(dict, "b", "bba");

    char *key;
    data = dict_get_data(dict, 1, &key);
    printf("data:%s\n",data);
    free_dict(dict);
    return 0;
}

