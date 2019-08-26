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

/**
 * get informations of bsf/confuri/xcap/domain
 *
 * Author: Yongli.Tong
 * Date  : 2019/07/03
 */

#include "volteConfig.h"
#include "log.h"
#include "xmlAssistant/xmlAssistantAdapter.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

volte_data_list* g_volte_data_list_ptr = NULL;

static void delete_volte_data_list();
static int parser_and_add_xml_node_to_list(xml_data_node* xml_data);

int parseVolteConfigXml(char* file_name, char* numberic ) {
    LOG("parseVolteConfigXml...");
    int result = RETURN_ERROR;

    if(NULL == g_volte_data_list_ptr) {
        g_volte_data_list_ptr = (volte_data_list*)_alloc_memory(sizeof(volte_data_list));

        g_volte_data_list_ptr->xml_data_list_ptr = (xml_data_list *)_alloc_memory(sizeof(xml_data_list));

        memset(g_volte_data_list_ptr->xml_data_list_ptr, '\0', sizeof(xml_data_list));
    } else {
        if((g_volte_data_list_ptr != NULL) && (strcmp(g_volte_data_list_ptr->numberic, numberic) ==0)) {
            return RETURN_OK;
        } else {
            delete_volte_data_list();
            g_volte_data_list_ptr = (volte_data_list*)_alloc_memory(sizeof(volte_data_list));

            g_volte_data_list_ptr->xml_data_list_ptr = (xml_data_list *)_alloc_memory(sizeof(xml_data_list));

            memset(g_volte_data_list_ptr->xml_data_list_ptr, '\0', sizeof(xml_data_list));
        }
    }

    //save numberic
    g_volte_data_list_ptr->numberic = (char*)_alloc_memory(sizeof(numberic));
    strcpy(g_volte_data_list_ptr->numberic, numberic);

    //parser apn xml file
    result = parseConfigXml(file_name, numberic, NODE_TYPE, GET_ONE_NODE, parser_and_add_xml_node_to_list);
    return result;
}

char* getVolteConfigUri(char* key, char* numberic)
{
    xml_data_list* p_xml_data_list = g_volte_data_list_ptr->xml_data_list_ptr;
    xml_data_node* tmp_volte_data_node_ptr = NULL;

    if(strcmp(g_volte_data_list_ptr->numberic, numberic) != 0) {
        return "";
    }

    while(p_xml_data_list != NULL) {
        tmp_volte_data_node_ptr = p_xml_data_list->xml_data;
        while(tmp_volte_data_node_ptr != NULL) {
            if(strcmp(tmp_volte_data_node_ptr->key, key) == 0) {
                return tmp_volte_data_node_ptr->value;
            }
            tmp_volte_data_node_ptr = tmp_volte_data_node_ptr->next;
        }
        p_xml_data_list = p_xml_data_list->next;
    }

    return "";
}

xml_data_list* getVolteConfigList(char* numberic) {
    if(strcmp(g_volte_data_list_ptr->numberic, numberic) == 0) {
        return g_volte_data_list_ptr->xml_data_list_ptr;
    }
    return NULL;
}

static void delete_volte_data_list() {
    xml_data_list* p_tmp_xml_data_list = g_volte_data_list_ptr->xml_data_list_ptr;
    free(g_volte_data_list_ptr->numberic);
    while(p_tmp_xml_data_list != NULL) {
        while(p_tmp_xml_data_list->xml_data != NULL) {
            free(p_tmp_xml_data_list->xml_data->key);
            free(p_tmp_xml_data_list->xml_data->value);
            p_tmp_xml_data_list->xml_data = p_tmp_xml_data_list->xml_data->next;
        }
        p_tmp_xml_data_list = p_tmp_xml_data_list->next;
    }

    free(g_volte_data_list_ptr);
}

static int parser_and_add_xml_node_to_list(xml_data_node* xml_data) {
    int result = RETURN_NOT_MATCH;
    xml_data_node* xml_data_ptr = xml_data;
    char * numberic = NULL;
    while(xml_data_ptr != NULL) {
        if(strcmp(xml_data_ptr->key, "numeric") == 0) {
            numberic = xml_data_ptr->value;
            break;
        }
        xml_data_ptr = xml_data_ptr->next;
    }
    if(strcmp(numberic, g_volte_data_list_ptr->numberic) == 0) {
        result = RETURN_MATCHED;
        //add node to g_volte_data_list_ptr
        add_node_to_list(g_volte_data_list_ptr->xml_data_list_ptr, xml_data);
    }
    return result;
}

