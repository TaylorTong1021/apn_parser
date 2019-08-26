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

xml_volte_config_type g_xml_volte_config[MAX_ELEMENTS] = {
    {"domain", "ims.mnc", ".3gppnetwork.org", MAX_DOMAIN_LEN},
    {"xcap", "xcap.ims.mnc", ".pub.3gppnetwork.org", MAX_XCAP_LEN},
    {"bsf", "bsf.mnc", ".pub.3gppnetwork.org", MAX_BSF_LEN},
    {"configuri", "sip:mmtel@conf-factory.ims.mnc", ".3gppnetwork.org", MAX_CONFIGURI_LEN}
};

volte_data_list* g_volte_data_list_ptr = NULL;

static void delete_volte_data_list();
static int parser_and_add_xml_node_to_list(xml_data_node* xml_data);
static char* buildDefaultUriStr(char* mcc, char* mnc, char* head, char* tail, unsigned int max_len);
static char* getDefaultStr(char* key, char* mcc, char* mnc);
static void separate_mcc_mnc_from_numberic(char* mcc, char* mnc, char* numberic);

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

    char mcc[5];
    char mnc[5];

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

    separate_mcc_mnc_from_numberic(&mcc[0], &mnc[0], numberic);

    return getDefaultStr(key, mcc, mnc);
}

static void delete_volte_data_list() {
    free(g_volte_data_list_ptr->numberic);
    delete_xml_data_list(g_volte_data_list_ptr->xml_data_list_ptr);
    free(g_volte_data_list_ptr);
    g_volte_data_list_ptr = NULL;
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

static char* buildDefaultUriStr(char* mcc, char* mnc, char* head, char* tail, unsigned int max_len) {
    unsigned int index = 0;
    char *tmp_str = NULL;
    tmp_str = (char *)_alloc_memory(sizeof(char) * max_len);

    memset(tmp_str, '\0', sizeof(char) * max_len);

    strcpy(tmp_str, head);
    index += strlen(head);
    strcpy(tmp_str+index, mnc);
    index += strlen(mnc);
    strcpy(tmp_str + index, ".mcc");
    index += strlen(".mcc");
    strcpy(tmp_str + index, mcc);
    index += strlen(mcc);
    strcpy(tmp_str + index, tail);

    return tmp_str;
}

static char* getDefaultStr(char* key, char* mcc, char* mnc) {
    //default volte config info
    char* tmp_str = NULL;
    int index = 0;
    
    for(index  = 0; index < MAX_ELEMENTS; index++) {
        if( (g_xml_volte_config[index].field != NULL)
            && (strcmp(key, g_xml_volte_config[index].field) == 0) ) {
            tmp_str = buildDefaultUriStr(mcc, mnc, g_xml_volte_config[index].head,
                            g_xml_volte_config[index].tail, g_xml_volte_config[index].max_len);
            break;
        }
    }

    return tmp_str;
}

static void separate_mcc_mnc_from_numberic(char* mcc, char* mnc, char* numberic) {
    //init & build default uri.
    memset(mcc, '\0', sizeof(mcc));
    memset(mnc, '\0', sizeof(mnc));

    //get mcc
    strncpy(mcc, numberic, 3);

    //get mnc
    if(strlen(numberic) == 5) {
        mnc[0] = '0';
        strncpy(&mnc[1], &numberic[3], 2);
    } else if(strlen(numberic) == 6) {
        strncpy(mnc, &numberic[3], 3);
    }
}

