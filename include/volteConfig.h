#ifndef _VOLTE_CONFIG_H_
#define _VOLTE_CONFIG_H_

#include "memory.h"

typedef struct volte_data_list {
    char* numberic;
    xml_data_list *xml_data_list_ptr; 
} volte_data_list;

int parseVolteConfigXml(char* file_name, char* numberic );
char* getVolteConfigUri(char* key, char* numberic);
xml_data_list* getVolteConfigList(char* numberic);
#endif