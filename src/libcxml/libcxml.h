#ifdef __cplusplus
extern "C" {
#endif

typedef struct str_node node_t;
typedef struct str_node {
    void *node;
    int (*child_count)(char *name, node_t*);
    node_t *(* child)(char* name, int index, node_t*);
    char *value;
} node_t;

typedef struct str_xml {
    int node_count;
    void *doc;
    node_t **node;
    node_t *(*get_node)(char*);
} xml_t;


xml_t *xml_new(char *file_name, char *err_buf);
void xml_free(xml_t*);
#ifdef __cplusplus
}
#endif
