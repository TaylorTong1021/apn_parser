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
#include "log/log.h"
#include "xmlAssistant/xmlAssistantAdapter.h"

#include <string.h>
#include <stdbool.h>

apn_data_list* g_apn_data_list_ptr = NULL;

static void setPlmn(char * numberic);

char* getApnConfigUri(char* key, char* numberic)
{
    xml_data_list* p_xml_data_list = NULL;
    xml_data_list* p_tmp_xml_data_list = NULL;
    xml_data_type* p_tmp_data = NULL;
    xml_search_info search_info;
    bool flag_mcc_match = false;
    bool flag_mnc_match = false;
    bool flag_ims_match = false;

    search_info.key = "mcc";
    search_info.value = s_mcc;
    search_info.key2 = "mnc";
    search_info.value2 = s_mnc;
    setPlmn(numberic);
    p_xml_data_list = getConfigList(&search_info);
    p_tmp_xml_data_list = p_xml_data_list;
    while(p_tmp_xml_data_list != NULL) {
        p_tmp_data = p_tmp_xml_data_list->xml_data;
        while(p_tmp_data != NULL) {
            if( (strcmp(p_tmp_data->value, search_info.value) == 0)
              && (strcmp(p_tmp_data->key, search_info.key) == 0) ) {
                flag_mcc_match = true;
            }
            if(flag_mcc_match && (strcmp(p_tmp_data->value, search_info.value2) == 0)
              && (strcmp(p_tmp_data->key, search_info.key2) == 0)){
                flag_mnc_match = true;
            }
            if(flag_mcc_match && flag_mnc_match
              && ((strcmp(p_tmp_data->value, "ims") == 0)
              && (strcmp(p_tmp_data->key, "apn") == 0) ) ) {
                /*if(strcmp(p_tmp_data->key, key) == 0) {
                    return p_tmp_data->value;
                }*/
                flag_ims_match = true;
            }
            if(flag_mcc_match && flag_mnc_match && flag_ims_match
              && (strcmp(p_tmp_data->key, key) == 0)) {
                return p_tmp_data->value;
            }
            p_tmp_data = p_tmp_data->next;
        }
        flag_mcc_match = false;
        flag_mnc_match = false;
        flag_ims_match = false;
        p_tmp_xml_data_list = p_tmp_xml_data_list->next;
    }

    return "";
}

apn_data_list* getApnConfigList(char* numberic) {
    apn_data_list* p_apn_data_list = NULL;
    xml_data_list* p_xml_data_list = NULL;
    xml_search_info search_info;
    search_info.key = "numberic";
    search_info.value = numberic;
    setPlmn(numberic);
    p_xml_data_list = getConfigList(&search_info);
    p_apn_data_list = (apn_data_list*) p_xml_data_list;
    
    return p_apn_data_list;
}

int parseApnConfigXml(char* file_name, char* numberic ) {
    LOG("parseApnConfigXml...\n");
    int result = RETURN_ERROR;

    if(NULL == g_apn_data_list_ptr) {
        g_apn_data_list_ptr = (apn_data_list*)_alloc_memory(sizeof(apn_data_list));
    } else {
        if(g_apn_data_list_ptr != NULL) && (strcmp(g_apn_data_list_ptr->numberic, numberic) ==0) {
            return RETURN_OK;
        } else {
            delete_apn_data_list();
            g_apn_data_list_ptr = (apn_data_list*)_alloc_memory(sizeof(apn_data_list));
        }
    }

    result = parseConfigXml(file_name, numberic, ATTRIBUTE_TYPE);
    return result;
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
