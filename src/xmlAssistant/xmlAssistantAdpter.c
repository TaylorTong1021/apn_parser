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

#include "xmlAssistant/xmlAssistantAdapter.h"

#include <string.h>
#include <stdbool.h>

xml_search_info *g_search_info_ptr = NULL;
xml_data_save_list* g_xml_data_list_header = NULL;

xml_data_list *g_xml_attr_list_header = NULL;

static int get_xml_node_info_callback(xml_data_type* xml_data);
static int get_xml_attr_info_callback(xml_data_type* xml_data);
static xml_data_type* create_xml_data_node();
static int save_data_to_volte_node(xml_data_type* xml_data);
static void add_node_to_data_list(xml_data_list* node_list, xml_data_type* xml_data);
static int save_data_to_apn_node(xml_data_type* xml_data);

/**
 * parse the volte-config xml file. you need to run one time in the inital phase.
 *
 * Author: Yongli.Tong
 * Date  : 2019/07/03
 */
int parseConfigXml(char* fileName, char* numberic, unsigned int parse_type)
{
    int result = RETURN_ERROR;

    if(NULL == numberic || NULL == fileName) {
        LOG("numberic or fileName is NULL!\n");
        return RETURN_ERROR;
    }

    result = xml_open(fileName);
    if(RETURN_OK != result)
    {
        LOG("xml_open error.\n");
        return RETURN_ERROR;
    }

    if(parse_type == NODE_TYPE) {
        result = xml_parse(get_xml_node_info_callback, NODE_TYPE);
    } else if(parse_type == ATTRIBUTE_TYPE) {
        result = xml_parse(get_xml_attr_info_callback, ATTRIBUTE_TYPE);
    }
    if(RETURN_OK != result)
    {
        LOG("xml_parse error.\n");
        return RETURN_ERROR;
    }

    xml_close();

    return RETURN_OK;
}

/**
 * get field through key.The data is saved in g_xml_data_list_header.
 *
 * Author: Yongli.Tong
 * Date  : 2019/07/11
 */
char* getConfigUri(char* field, char* value)
{
    xml_data_save_list* p_xml_data_save_list = g_xml_data_list_header;
    xml_data_type* p_xml_data = NULL;
    char* result =  NULL;
    char* p_key_value = NULL;
    if(NULL == g_xml_data_list_header) {
        return NULL;
    }

    while((p_xml_data_save_list != NULL) && (p_xml_data_save_list->xml_data != NULL)) {
        p_xml_data = p_xml_data_save_list->xml_data;
        result =  NULL;
        p_key_value = NULL;
        while (p_xml_data != NULL) {
            //printf("p_xml_data->key=%s,p_xml_data->value=%s,field=%s.\n",p_xml_data->key,p_xml_data->value,field);
            if(strcmp(p_xml_data->key, g_search_info_ptr->key) == 0) {
                p_key_value = p_xml_data->value;
            }
            if(strcmp(p_xml_data->key, field) == 0) {
                result = p_xml_data->value;
                //printf("key=%s,result=%s.\n",field,result);
            }
            if( (p_key_value != NULL) && (result != NULL) ) {
                return result;
            }
            p_xml_data = p_xml_data->next;
        }
        
        p_xml_data_save_list = p_xml_data_save_list->next;
    }
    return result;

}

xml_data_list* getConfigList(xml_search_info* search_info_ptr) {
    xml_data_list* p_xml_data_save_list = g_xml_attr_list_header;
    xml_data_type* p_xml_data = NULL;
    xml_data_type* p_tmp_xml_data = NULL;
    xml_data_list* p_xml_data_list = NULL;
    bool flag = false;
    
    if(NULL == p_xml_data_save_list) {
        return NULL;
    }
    while(p_xml_data_save_list != NULL) {
        p_xml_data = p_xml_data_save_list->xml_data;
        p_tmp_xml_data = p_xml_data;
        while (p_tmp_xml_data != NULL) {
            if( (strcmp(p_tmp_xml_data->value, search_info_ptr->value) == 0)
              && (strcmp(p_tmp_xml_data->key, search_info_ptr->key) == 0) ) {
                flag = true;
            }
            if(flag && (strcmp(p_tmp_xml_data->value, search_info_ptr->value2) == 0)
              && (strcmp(p_tmp_xml_data->key, search_info_ptr->key2) == 0) ) {
                //printf("p_tmp_xml_data->key=%s,p_tmp_xml_data->value=%s.\n",p_tmp_xml_data->key,p_tmp_xml_data->value);
                if(NULL == p_xml_data_list) {
                    p_xml_data_list = (xml_data_list*)malloc(sizeof(xml_data_list));
                    if(NULL == p_xml_data_list) {
                        printf("getConfigList alloc memory failed!\n");
                        return NULL;
                    }
                    p_xml_data_list->next = NULL;
                }
                add_node_to_data_list(p_xml_data_list, p_xml_data);
            }
            p_tmp_xml_data = p_tmp_xml_data->next;
        }
        flag = false;
        
        p_xml_data_save_list = p_xml_data_save_list->next;
    }
    return p_xml_data_list->next;
}

static void add_node_to_data_list(xml_data_list* node_list, xml_data_type* xml_data) {
    xml_data_list* p_xml_data_list = NULL;
    //xml_data_list* p_tmp_xml_data_list = NULL;
    p_xml_data_list = node_list;
    while(p_xml_data_list != NULL) {
        if(p_xml_data_list->next == NULL) {
            p_xml_data_list->next = (xml_data_list*)malloc(sizeof(xml_data_list));
            p_xml_data_list = p_xml_data_list->next;
            p_xml_data_list->next = NULL;
            p_xml_data_list->xml_data = NULL;
            break;
        }
        
        p_xml_data_list = p_xml_data_list->next;
    }
    if(NULL != p_xml_data_list) {
        p_xml_data_list->xml_data = xml_data;
        //printf("xml_data=%p.\n",p_xml_data_list->xml_data);
    }

}

static int get_xml_node_info_callback(xml_data_type* xml_data) {
    int result = RETURN_ERROR;
    //TODO
    return result;
}

static int get_xml_attr_info_callback(xml_data_type* xml_data) {
    int result = RETURN_ERROR;
    bool flag = false;
    xml_data_type* xml_data_ptr = xml_data;
    
    while(xml_data_ptr != NULL) {
        //printf("xml_data->key = %s, --- value = %s.\n",xml_data->key, xml_data->value);
        if( strcmp(xml_data_ptr->key, g_search_info_ptr->key) == 0
            && strcmp(xml_data_ptr->value, g_search_info_ptr->value) == 0 ) {
            flag = true;
        }
        if(flag && (strcmp(xml_data_ptr->key, g_search_info_ptr->key2) == 0) 
            && (strcmp(xml_data_ptr->value, g_search_info_ptr->value2) == 0) ){
            save_data_to_apn_node(xml_data);
            result = RETURN_OK;
            //printf("curNode->name = %s, --- value = %s.\n",xml_data->key, xml_data->value);
            break;
        }
        xml_data_ptr = xml_data_ptr->next;
    }
    return result;
}

static xml_data_type* create_xml_data_node() {
    xml_data_type* p_xml_data_node;
    p_xml_data_node = (xml_data_type*) malloc(sizeof(xml_data_type));
    p_xml_data_node->key = NULL;
    p_xml_data_node->value = NULL;
    p_xml_data_node ->next = NULL;
    return p_xml_data_node;
}

static int save_data_to_apn_node(xml_data_type* xml_data) {
    xml_data_list* p_tmp_data_list = NULL;
    xml_data_list* p_save_data_list = NULL;
    xml_data_type* p_tmp_xml_data = NULL;
    xml_data_type* p_save_tmp_xml_data = NULL;

    p_tmp_xml_data = xml_data;
    if(NULL == g_xml_attr_list_header) {
        g_xml_attr_list_header = (xml_data_list*) malloc(sizeof(xml_data_list));
        if(NULL == g_xml_attr_list_header) {
            printf("g_xml_attr_list_header alloc memoery failed!\n");
            return RETURN_ERROR;
        }
        g_xml_attr_list_header->xml_data = NULL;
        g_xml_attr_list_header->next = NULL;
    }

    p_tmp_data_list = g_xml_attr_list_header;
    while(p_tmp_data_list != NULL) {
        if(p_tmp_data_list->next == NULL) {
            p_tmp_data_list->next = (xml_data_list*) malloc(sizeof(xml_data_list));
            if(NULL == p_tmp_data_list->next) {
                printf("p_save_data_list->next alloc memoery failed!\n");
                return RETURN_ERROR;
            }
            p_tmp_data_list->next->xml_data = NULL;
            p_tmp_data_list->next->next =  NULL;
            p_tmp_data_list = p_tmp_data_list->next;
            break;
        }
        
        p_tmp_data_list = p_tmp_data_list->next;
    }
    p_save_data_list = p_tmp_data_list;

    if(p_save_data_list->xml_data == NULL) {
        p_save_data_list->xml_data = (xml_data_type*)malloc(sizeof(xml_data_type));
        if(p_save_data_list->xml_data == NULL) {
            printf("alloc p_save_data_list->xml_data failed!\n");
            return RETURN_ERROR;
        }
    }

    p_save_tmp_xml_data = p_save_data_list->xml_data;
    p_save_tmp_xml_data->next = NULL;

    while(p_tmp_xml_data != NULL) {
        //key
        p_save_tmp_xml_data->key = (char *)malloc(sizeof(char) * (strlen((char *)p_tmp_xml_data->key) + 1));
        strcpy(p_save_tmp_xml_data->key, (char *)p_tmp_xml_data->key);
        //value
        p_save_tmp_xml_data->value = (char *)malloc(sizeof(char) * (strlen((char*)p_tmp_xml_data->value) + 1));
        strcpy(p_save_tmp_xml_data->value, (char*)p_tmp_xml_data->value);
        
        if(p_tmp_xml_data->next != NULL) {
            p_save_tmp_xml_data->next = (xml_data_type*)malloc(sizeof(xml_data_type));
            if(p_save_tmp_xml_data->next == NULL) {
                printf("alloc p_save_tmp_xml_data->next failed!\n");
                return RETURN_ERROR;
            }
            p_save_tmp_xml_data = p_save_tmp_xml_data->next;
        } else {
            p_save_tmp_xml_data->next = NULL;
        }
        p_tmp_xml_data = p_tmp_xml_data->next;
    }
    

    p_save_data_list->xml_data = p_save_data_list->xml_data;
    p_save_data_list->next = NULL;

    return RETURN_OK;
}

static int save_data_to_volte_node(xml_data_type* xml_data) {
    xml_data_type* p_tmp_xml_data = xml_data;
    xml_data_save_list* p_tmp_data_save_list;

    if(NULL == g_xml_data_list_header) {
        g_xml_data_list_header = (xml_data_save_list*) malloc(sizeof(xml_data_save_list));
        if(NULL == g_xml_data_list_header) {
            printf("g_xml_data_list_header alloc memoery failed!\n");
            return RETURN_ERROR;
        }
        g_xml_data_list_header->xml_data = NULL;
        g_xml_data_list_header->search_info = NULL;
        g_xml_data_list_header->next = NULL;
    }

    p_tmp_data_save_list = g_xml_data_list_header;

    while(NULL != p_tmp_data_save_list->next) {
        if( (p_tmp_data_save_list->search_info != NULL)
            && (strcmp(p_tmp_data_save_list->search_info->value, g_search_info_ptr->value) == 0) ) {
            return RETURN_OK;
        }
        p_tmp_data_save_list = p_tmp_data_save_list->next;
    }

    p_tmp_data_save_list->xml_data = xml_data;
    p_tmp_data_save_list->search_info = (xml_search_info *)malloc(sizeof(xml_search_info));
    if(NULL == p_tmp_data_save_list->search_info) {
        printf("save_data_to_volte_node alloc search_info failed!\n");
        return RETURN_ERROR;
    }
    p_tmp_data_save_list->search_info->key = g_search_info_ptr->key;
    p_tmp_data_save_list->search_info->value = g_search_info_ptr->value;
    p_tmp_data_save_list->next = NULL;
    return RETURN_OK;
}

