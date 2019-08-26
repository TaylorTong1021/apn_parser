
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

void add_node_to_list(xml_data_list* p_xml_data_list, xml_data_node* p_xml_data_node) {
    xml_data_list *p_tmp_xml_data_list = p_xml_data_list;
    xml_data_list *p_new_xml_data_list_node = NULL;

    while(p_tmp_xml_data_list->next != NULL) {
        p_tmp_xml_data_list = p_tmp_xml_data_list->next;
    }

    if(p_tmp_xml_data_list->xml_data != NULL) {
        p_new_xml_data_list_node = (xml_data_list *)_alloc_memory(sizeof(xml_data_list));
        p_new_xml_data_list_node->xml_data = p_xml_data_node;
        p_new_xml_data_list_node->next = NULL;

        p_tmp_xml_data_list->next = p_new_xml_data_list_node;
    }else {
        p_tmp_xml_data_list->xml_data = p_xml_data_node;
        p_tmp_xml_data_list ->next = NULL;
    }
    
}

void add_node_to_tail(xml_data_node* p_xml_data_list, xml_data_node* p_xml_data_node) {
    xml_data_node *p_tmp_xml_data_list = p_xml_data_list;

    while(p_tmp_xml_data_list->next != NULL) {
        p_tmp_xml_data_list = p_tmp_xml_data_list->next;
    }
    p_tmp_xml_data_list->next = p_xml_data_node;
    p_xml_data_node->next = NULL;
}

void delete_xml_data_list(xml_data_list* p_xml_data_list) {
    xml_data_list* p_tmp_xml_data_list = p_xml_data_list;
    xml_data_node* p_tmp_xml_data_node = NULL;
    while(p_tmp_xml_data_list != NULL) {
        while(p_tmp_xml_data_list->xml_data != NULL) {
            p_tmp_xml_data_node = p_tmp_xml_data_list->xml_data;
            free(p_tmp_xml_data_node->key);
            free(p_tmp_xml_data_node->value);
            p_tmp_xml_data_list->xml_data = p_tmp_xml_data_list->xml_data->next;
            free(p_tmp_xml_data_node);
        }
        p_tmp_xml_data_list = p_tmp_xml_data_list->next;
    }
    p_tmp_xml_data_list = p_xml_data_list;
    while(p_xml_data_list != NULL) {
        p_tmp_xml_data_list = p_xml_data_list;
        p_xml_data_list = p_xml_data_list->next;
        free(p_tmp_xml_data_list);
    }
}

