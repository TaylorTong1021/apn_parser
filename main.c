
#include "log/log.h"

#include <stdlib.h>
#include <apnConfig.h>

#define APN_CONFIG_FILE_NAME   "../res/apns-conf_8.xml"

enum{
	RETURN_ERROR = -1,
	RETURN_OK = 0
};

int main(int argc, char** argv)
{
    int result = RETURN_ERROR;

    char* numberic;

    //apn info
    char* apn, *protocol, *authType, *username, *password;

    //apn list
    apn_data_list* p_apn_data_list = NULL;
    apn_data_list* p_tmp_apn_data_list = NULL;
    apn_data_type* p_tmp_data = NULL;

    //apn get every elements
    numberic = "46002";
    result = parseApnConfigXml(APN_CONFIG_FILE_NAME, numberic);
    if(RETURN_OK != result)
    {
        LOG("parseApnConfigXml failed!\n");
        return result;
    }

    apn = getApnConfigUri("apn", numberic);
    protocol = getApnConfigUri("protocol", numberic);
    authType = getApnConfigUri("authType", numberic);
    username = getApnConfigUri("username", numberic);
    password = getApnConfigUri("password", numberic);

    LOG("apn: %s\n", apn);
    LOG("protocol: %s\n", protocol);
    LOG("authType: %s\n", authType);
    LOG("username: %s\n", username);
    LOG("password: %s\n", password);

    //apn get list
    numberic = "46001";
    result = parseApnConfigXml(APN_CONFIG_FILE_NAME, numberic);
    if(RETURN_OK != result)
    {
        LOG("parseApnConfigXml failed!\n");
        return result;
    }

    p_apn_data_list = getApnConfigList(numberic);

    p_tmp_apn_data_list = p_apn_data_list;
    while(p_tmp_apn_data_list != NULL) {
        p_tmp_data = p_tmp_apn_data_list->xml_data;
        LOG("------------------------------------------\n");
        while(p_tmp_data != NULL) {
            LOG("key=%s,value=%s.\n",p_tmp_data->key, p_tmp_data->value);
            p_tmp_data = p_tmp_data->next;
        }
        p_tmp_apn_data_list = p_tmp_apn_data_list->next;
    }
    if(NULL != p_apn_data_list) {
        free(p_apn_data_list);
    }
    
    return 0;
}