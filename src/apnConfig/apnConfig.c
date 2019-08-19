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

#include "apnConfig.h"
#include "memory.h"
#include "log.h"
#include "xmlAssistant/xmlAssistantAdapter.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

apn_data_list* g_apn_data_list_ptr = NULL;

static void delete_apn_data_list();
static int parser_and_add_xml_node_to_list(xml_data_node* xml_data);

int parseApnConfigXml(char* file_name, char* numberic ) {
    LOG("parseApnConfigXml...");
    int result = RETURN_ERROR;

    if(NULL == g_apn_data_list_ptr) {
        g_apn_data_list_ptr = (apn_data_list*)_alloc_memory(sizeof(apn_data_list));
    } else {
        if((g_apn_data_list_ptr != NULL) && (strcmp(g_apn_data_list_ptr->numberic, numberic) ==0)) {
            return RETURN_OK;
        } else {
            delete_apn_data_list();
            g_apn_data_list_ptr = (apn_data_list*)_alloc_memory(sizeof(apn_data_list));
        }
    }

    //save numberic
    g_apn_data_list_ptr->numberic = (char*)_alloc_memory(sizeof(numberic));
    strcpy(g_apn_data_list_ptr->numberic, numberic);

    //parser apn xml file
    result = parseConfigXml(file_name, numberic, ATTRIBUTE_TYPE, GET_MULTI_NODE, parser_and_add_xml_node_to_list);
    return result;
}

char* getApnConfigUri(char* key, char* numberic, char* apn_type)
{
    apn_data_list* tmp_apn_data_list_ptr = g_apn_data_list_ptr;
    apn_data_type* tmp_apn_data_node_ptr;
    char* result = NULL;
    bool is_target_node = false;

    while(tmp_apn_data_list_ptr != NULL) {
        tmp_apn_data_node_ptr = tmp_apn_data_list_ptr->xml_data;
        while(tmp_apn_data_node_ptr != NULL) {
            if((strcmp(tmp_apn_data_node_ptr->key, "type") == 0)
              && (strcmp(tmp_apn_data_node_ptr->value, apn_type) == 0)) {
                is_target_node = true;
            }
            if(strcmp(tmp_apn_data_node_ptr->key, key) == 0) {
                result = tmp_apn_data_node_ptr->value;
            }
            tmp_apn_data_node_ptr = tmp_apn_data_node_ptr->next;
        }
        if(is_target_node && (result != NULL)) {
            return result;
        } else {
            result = NULL;
            is_target_node = false;
        }
        tmp_apn_data_list_ptr = tmp_apn_data_list_ptr->next;
    }

    return "";
}

apn_data_list* getApnConfigList(char* numberic) {
    apn_data_list* p_apn_data_list;
    memcpy(p_apn_data_list, g_apn_data_list_ptr, sizeof(apn_data_list));
    return p_apn_data_list;
}

static void delete_apn_data_list() {
    apn_data_list* p_tmp_apn_data_list = g_apn_data_list_ptr;
    while(g_apn_data_list_ptr != NULL) {
        while(g_apn_data_list_ptr->xml_data != NULL) {
            free(g_apn_data_list_ptr->xml_data->key);
            free(g_apn_data_list_ptr->xml_data->value);
            g_apn_data_list_ptr->xml_data = g_apn_data_list_ptr->xml_data->next;
        }
        g_apn_data_list_ptr = g_apn_data_list_ptr->next;
    }
    free(p_tmp_apn_data_list);
}

static int parser_and_add_xml_node_to_list(xml_data_node* xml_data) {
    int result = RETURN_ERROR;
    //TODO
    LOG("parser_and_add_xml_node_to_list...");
    return result;
}

