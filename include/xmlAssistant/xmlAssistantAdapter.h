#ifndef _XML_ASSISTANT_ADAPTER_H_
#define _XML_ASSISTANT_ADAPTER_H_

#include "xmlAssistant/xmlAssistantImpl.h"
#include "memory.h"

int parseConfigXml(char* fileName, char* numberic, unsigned int parse_type, 
                    unsigned int get_node_type, int (*callfunc)(xml_data_node*));
#endif