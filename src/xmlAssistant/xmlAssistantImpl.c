/*
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "xmlAssistant/xmlAssistantImpl.h"
#include "log.h"
#include "memory.h"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <string.h>

xml_data_node* p_xml_data_header;
xmlDocPtr mDoc;
xmlNodePtr mCurNode;

static int xml_parser_per_tag(xmlNodePtr node);
static int xml_parser_per_attribute(xmlNodePtr node);
static void clear_xml_data(xml_data_node* xml_data);
static xml_data_node* create_and_set_data_to_node(char* key, char* value);
static void add_node_to_list(xml_data_node* p_xml_data_node);

/**
 * read xml file and get node ptr from xml.
 *
 * Author: Yongli.Tong
 * Date  : 2019/08/18
 */
int xml_open(char* fileName)
{
	mDoc = xmlParseFile( fileName );
	if( !mDoc ){
		LOG("parseVOlteConfigXml doc is null.");
		return RETURN_ERROR;
	}

	mCurNode = xmlDocGetRootElement( mDoc );
	if( !mCurNode  ){
		LOG("parseVOlteConfigXml cur is null.");
		return RETURN_ERROR;
	}

	return RETURN_OK;
}

/**
 * parse all xml info from xml files
 *
 * Author: Yongli.Tong
 * Date  : 2019/08/18
 */
int xml_parse(int (*callfunc)(xml_data_node*), unsigned int parse_type, unsigned int node_type/*return one or more node*/)
{
    xmlNodePtr curNode = mCurNode;
    xml_data_node* p_tmp_xml_data;
    int result = RETURN_ERROR;
	if( !mDoc || !curNode ){
		LOG("extractXml error");
		return RETURN_ERROR;
	}

	//first Tag.
	curNode = curNode->xmlChildrenNode;
	if( !curNode ){
		LOG("xml element not found.");
		return RETURN_ERROR;
	}
    //alloc memory.
    p_xml_data_header = (xml_data_node *)_alloc_memory(sizeof(xml_data_node));

	while( curNode ){
        p_xml_data_header->next = NULL;
        if(parse_type == NODE_TYPE) {
		    result = xml_parser_per_tag( curNode );
        } else if(parse_type == ATTRIBUTE_TYPE) {
            result = xml_parser_per_attribute(curNode);
        } else {
            LOG("parser type is illegue!");
            return RETURN_ERROR;
        }
        if(RETURN_ERROR != result) {
            p_tmp_xml_data = p_xml_data_header->next;
            if(NULL == p_tmp_xml_data) {
                curNode = curNode->next;
                continue;
            }
            result = callfunc(p_tmp_xml_data);
            if(result == RETURN_OK) {
                if(GET_MULTI_NODE == node_type) {
                    curNode = curNode->next;
                    clear_xml_data(p_tmp_xml_data);
                    continue;
                }
                p_xml_data_header->next = NULL;
                break;
            } else {
                clear_xml_data(p_tmp_xml_data);
            }
        }
		curNode = curNode->next;
	}

    return RETURN_OK;
}

/**
 * close xml doc
 *
 * Author: Yongli.Tong
 * Date  : 2019/07/03
 */
void xml_close()
{
    xmlFreeDoc(mDoc);
    mDoc = NULL;
    mCurNode = NULL;
}

static xml_data_node* create_and_set_data_to_node(char* key, char* value) {
    xml_data_node * p_xml_data_node = (xml_data_node *)_alloc_memory(sizeof(xml_data_node));
    //key
    p_xml_data_node->key = (char *)_alloc_memory(sizeof(char) * (strlen((char *)key) + 1));
    strcpy(p_xml_data_node->key, key);
    //value
    p_xml_data_node->value = (char *)_alloc_memory(sizeof(char) * (strlen((char*)value) + 1));
    strcpy(p_xml_data_node->value, value);
    return p_xml_data_node;
}

static void add_node_to_list(xml_data_node* p_xml_data_node) {
    xml_data_node *p_tmp_xml_data_node = p_xml_data_header;

    while(p_tmp_xml_data_node->next != NULL) {
        p_tmp_xml_data_node = p_tmp_xml_data_node->next;
    }
    p_tmp_xml_data_node->next = p_xml_data_node;
    p_xml_data_node->next = NULL;
}

/**
 * parse every elemnts and deliver informations to  business layer.
 *
 * Author: Yongli.Tong
 * Date  : 2019/07/03
 */
static int xml_parser_per_tag(xmlNodePtr node)
{
	xmlNodePtr curNode = node->xmlChildrenNode;
    xml_data_node * p_xml_data_node = NULL;

	while( curNode ){
		if( curNode->name ){
			// xmlNodeListGetString return a duplicated pointer, need to release it at local
			xmlChar* value = xmlNodeListGetString(mDoc, curNode->xmlChildrenNode, 1);
            if(NULL == value) {
                curNode = curNode->next;
                continue;
            }
            //save xml data to linknode.
            p_xml_data_node = create_and_set_data_to_node((char *)curNode->name, (char*)value);
            
            //add node to list
            add_node_to_list(p_xml_data_node);
			if( value ){
				xmlFree( value );
			}
		}
		curNode = curNode->next;
	}
    if(NULL == p_xml_data_header) {
        return RETURN_ERROR;
    }
    return RETURN_OK;
}

static int xml_parser_per_attribute(xmlNodePtr node)
{
    xmlAttr* attribute = node->properties;

    xml_data_node * p_xml_data_node = NULL;

    while(attribute)
    {
        xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
        if(NULL == value) {
            attribute = attribute->next;
            continue;
        }
        //LOG("xml_parser_per_attribute name = %s, value=%s.",attribute->name, value);
        //save xml data to linknode.
        p_xml_data_node = create_and_set_data_to_node((char *)attribute->name, (char*)value);

        //add node to list
        add_node_to_list(p_xml_data_node);

        if( value ) {
            xmlFree(value);
        }
        attribute = attribute->next;
    }
    if(NULL == p_xml_data_header) {
        return RETURN_ERROR;
    }
    return RETURN_OK;
}

/**
 * clean up heap memory which allocated
 *
 * Author: Yongli.Tong
 * Date  : 2019/07/03
 */
static void clear_xml_data(xml_data_node* xml_data) {
    xml_data_node* tmp_xml_data = xml_data;
    while(tmp_xml_data != NULL) {
        free(tmp_xml_data->key);
        free(tmp_xml_data->value);
        tmp_xml_data = tmp_xml_data->next;
    }
    while(xml_data != NULL) {
        tmp_xml_data = xml_data;
        xml_data = xml_data->next;
        free(tmp_xml_data);
    }
}

