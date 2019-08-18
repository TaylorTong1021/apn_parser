#ifndef _XML_ASSISTANT_ADAPTER_H_
#define _XML_ASSISTANT_ADAPTER_H_

#include "xmlAssistant/xmlAssistantImpl.h"

int parseConfigXml(char* fileName, xml_search_info* search_info_ptr, unsigned int parse_type);
char* getConfigUri(char* field, char* value);
xml_data_list* getConfigList(xml_search_info* search_info_ptr);
#endif