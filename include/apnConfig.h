#ifndef _APN_CONFIG_H_
#define _APN_CONFIG_H_

#define MAX_PLMN_LEN 7

typedef struct apn_data_node {
    char* key;
    char* value; 
    struct apn_data_type* next;
} apn_data_type;

typedef struct apn_data_list {
    char* numberic;
    apn_data_type *xml_data; 
    struct apn_data_list* next;
} apn_data_list;

int parseApnConfigXml(char* file_name, char* numberic );
char* getApnConfigUri(char* key, char* numberic, char* apn_type);
apn_data_list* getApnConfigList(char* numberic);
#endif