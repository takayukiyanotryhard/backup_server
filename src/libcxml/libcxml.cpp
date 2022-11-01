#include <iostream>
#include <string>
#include "rapidxml.hpp"
#include "libcxml.h"
#include <string.h>

using namespace std;
namespace rx = rapidxml;

#define BUF_SIZE 0x10000

xml_t *g_xml = 0;

xml_t *_xml_new(char *buf);
void doc_free(void* pdoc);

int get_child_count(char *name, node_t* pnode) {
    // printf("get child count\n");
    rx::xml_node<> * node =  (rx::xml_node<>*)pnode->node;
    if (!node) return 0;

    int count = 0;
    // printf("%p\n", node);
    for(rx::xml_node<>*child = node->first_node(name);child;child = child->next_sibling(name)) {
        count++;
    }
    // printf("child:%d\n", count);
    return count;
}

node_t *get_child(char * name, int i, node_t* parent) {
    if (!parent) return 0;
    
    node_t *node = 0;
    rx::xml_node<> *p = (rx::xml_node<>*)parent->node;

    int check = i + 1;
    int count = 0;
    for(rx::xml_node<> *child = p->first_node(name);child;child = child->next_sibling(name)) {
//        printf("cn:%p %s\n", child, child->name());
        count++;
        if (check == count) {
            node = (node_t*)malloc(sizeof(node_t));
            node->node = child;
            node->child = get_child;
            node->value = child->value();
            node->child_count = get_child_count;
            // Todo: ここでparentに追加しておく
            while(child = child->next_sibling(name)){;}
            break;
        }
    }
    return node;
}

node_t *_get_node(char *name) {
    node_t *node = 0;
    rx::xml_node<> *found = 0;
    rx::xml_document<> *doc = (rx::xml_document<>*)g_xml->doc;
    if (!doc) goto ERROR;

    // printf("doc:%p\n", doc);

    for(rx::xml_node<> *node = doc->first_node(name);node;node = node->next_sibling(name)) {
        // printf("n:%p %s\n", node, node->name());
        found = node;
        while(node) node = node->next_sibling(name);
        break;
    }

    if (found) {
        node = (node_t*)calloc(sizeof(node_t), 1);
        node->node = (void*)found;
        node->child = get_child;
        node->value = found->value();
        node->child_count = get_child_count;
    }

ERROR:
    return node;

}
node_t *get_node(char *name){
    return _get_node(name);
}

xml_t *_xml_new(char *buf) {
    rx::xml_document<> *doc;
    g_xml = (xml_t*)calloc(sizeof(xml_t), 1);

    doc = new rx::xml_document<>();
    doc->parse<0>(buf);
    g_xml->doc = (void*)doc;
    g_xml->get_node = get_node;
    return g_xml;
}

xml_t *xml_new(char *file_name, char *err_buf) {
    char buf[BUF_SIZE];

    xml_free(g_xml);

    FILE *fp = fopen(file_name, "r");
    if (!fp) goto ERROR;

    fread(buf, BUF_SIZE, 1, fp);
    fclose(fp);
    g_xml = _xml_new(buf);

ERROR:
    return g_xml;
}

void doc_free(void* pdoc) {
    if (!pdoc) return;
    rx::xml_document<>* doc = (rx::xml_document<>*)pdoc;
    delete doc;
}

void xml_free(xml_t* xml) {
    if (xml) {
        doc_free(g_xml->doc);
        free(xml);
    }
}

