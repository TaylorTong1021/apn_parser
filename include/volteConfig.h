#ifndef _VOLTE_CONFIG_H_
#define _VOLTE_CONFIG_H_

#include "memory.h"

#define MAX_ELEMENTS 10

#define MAX_XCAP_LEN 100
#define MAX_BSF_LEN 50
#define MAX_DOMAIN_LEN 40
#define MAX_CONFIGURI_LEN 120

typedef struct xml_volte_config_type {
    char* field;
    char* head;
    char* tail;
    unsigned int max_len;
} xml_volte_config_type;

typedef struct volte_data_list {
    char* numberic;
    xml_data_list *xml_data_list_ptr; 
} volte_data_list;

int parseVolteConfigXml(char* file_name, char* numberic );
char* getVolteConfigUri(char* key, char* numberic);
#endif