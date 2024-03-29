#ifndef _MEMORY_H_
#define _MEMORY_H_

typedef struct xml_data_node {
    char* key;
    char* value; 
    struct xml_data_node* next;
} xml_data_node;

typedef struct xml_data_list {
    xml_data_node* xml_data;
    struct xml_data_list* next;
}xml_data_list;

void* _alloc_memory(unsigned int size);
xml_data_node* create_and_set_data_to_node(char* key, char* value);
void add_node_to_list(xml_data_list* p_xml_data_list, xml_data_node* p_xml_data_node);
void add_node_to_tail(xml_data_node* p_xml_data_list, xml_data_node* p_xml_data_node);
void delete_xml_data_list(xml_data_list* p_xml_data_list);

#endif