
#ifndef _XML_ASSISTANT_IMPL_H_
#define _XML_ASSISTANT_IMPL_H_
#include "memory.h"

enum{
	RETURN_ERROR = -1,
	RETURN_OK = 0,
    RETURN_FINISHED,
    RETURN_MATCHED,
    RETURN_NOT_MATCH
};

enum{
	NODE_TYPE = 1,
	ATTRIBUTE_TYPE
};

enum {
    GET_ONE_NODE = 1,
    GET_MULTI_NODE
};

int xml_open(char* fileName);
int xml_parse(int (*callfunc)(xml_data_node*), unsigned int parse_type, unsigned int node_type/*return one or more node*/);
void xml_close();

#endif /* _XML_ASSISTANT_H_ */
