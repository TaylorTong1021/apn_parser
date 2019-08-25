#ifndef _APN_CONFIG_H_
#define _APN_CONFIG_H_

#define MAX_PLMN_LEN 7
#define __CONSEQUENT_NODE

#include "memory.h"

typedef struct apn_data_list {
    char* numberic;
    xml_data_list *xml_data_list_ptr; 
} apn_data_list;

int parseApnConfigXml(char* file_name, char* numberic );
char* getApnConfigUri(char* key, char* numberic, char* apn_type);
xml_data_list* getApnConfigList(char* numberic);
#endif