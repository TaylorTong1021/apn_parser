
#include "memory.h"
#include "log.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define NDEBUG

void* _alloc_memory(unsigned int size) {
    void* ptr = NULL;
    if(size == 0) {
        assert(0);
    }
    ptr = malloc(size);
    assert(ptr != NULL);
    return ptr;
}

xml_data_node* create_and_set_data_to_node(char* key, char* value) {
    xml_data_node * p_xml_data_node = (xml_data_node *)_alloc_memory(sizeof(xml_data_node));
    //key
    p_xml_data_node->key = (char *)_alloc_memory(sizeof(char) * (strlen((char *)key) + 1));
    strcpy(p_xml_data_node->key, key);
    //value
    p_xml_data_node->value = (char *)_alloc_memory(sizeof(char) * (strlen((char*)value) + 1));
    strcpy(p_xml_data_node->value, value);
    return p_xml_data_node;
}

void add_node_to_list(xml_data_node* p_xml_data_header, xml_data_node* p_xml_data_node) {
    xml_data_node *p_tmp_xml_data_node = p_xml_data_header;

    while(p_tmp_xml_data_node->next != NULL) {
        p_tmp_xml_data_node = p_tmp_xml_data_node->next;
    }
    p_tmp_xml_data_node->next = p_xml_data_node;
    p_xml_data_node->next = NULL;
}