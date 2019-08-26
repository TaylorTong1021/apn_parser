
#include "log.h"

#include <stdlib.h>
#include <apnConfig.h>
#include <volteConfig.h>

#define APN_CONFIG_FILE_NAME   "./res/apns-conf_8.xml"
#define VOLTE_CONFIG_FILE_NAME "./res/volte-conf.xml"

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

    //volte
    char *domain, *xCap, *bsfAddr, *conferenceUri, *enable;

    //apn list
    xml_data_list* p_apn_data_list = NULL;
    xml_data_list* p_tmp_apn_data_list = NULL;
    xml_data_node* p_tmp_data = NULL;

    /* --- get every elements of volte info --- */
    numberic = "46002";
    result = parseVolteConfigXml(VOLTE_CONFIG_FILE_NAME, numberic);
    if(RETURN_OK != result)
    {
        LOG("parseVolteConfigXml failed!\n");
        return RETURN_ERROR;
    }

    domain = getVolteConfigUri( "domain", numberic );
    xCap = getVolteConfigUri( "xcap", numberic );
    bsfAddr = getVolteConfigUri( "bsf", numberic );
    conferenceUri = getVolteConfigUri( "configuri", numberic );
    enable = getVolteConfigUri( "enable", numberic );

    LOG("domain: %s", domain);
    LOG("xCap: %s", xCap);
    LOG("bsfAddr: %s", bsfAddr);
    LOG("configuri: %s", conferenceUri);
    LOG("enable: %s", enable);

    /* --- get every elements of apn info --- */
    numberic = "46002";
    result = parseApnConfigXml(APN_CONFIG_FILE_NAME, numberic);
    if(RETURN_OK != result)
    {
        LOG("parseApnConfigXml failed!");
        return result;
    }

    apn = getApnConfigUri("apn", numberic, "ims");
    protocol = getApnConfigUri("protocol", numberic, "ims");
    authType = getApnConfigUri("authType", numberic, "ims");
    username = getApnConfigUri("username", numberic, "ims");
    password = getApnConfigUri("password", numberic, "ims");

    LOG("apn: %s", apn);
    LOG("protocol: %s", protocol);
    LOG("authType: %s", authType);
    LOG("username: %s", username);
    LOG("password: %s", password);

    /* --- apn get list --- */
    numberic = "46000";
    result = parseApnConfigXml(APN_CONFIG_FILE_NAME, numberic);
    if(RETURN_OK != result)
    {
        LOG("parseApnConfigXml failed!");
        return result;
    }

    p_apn_data_list = getApnConfigList(numberic);

    p_tmp_apn_data_list = p_apn_data_list;
    while(p_tmp_apn_data_list != NULL) {
        p_tmp_data = p_tmp_apn_data_list->xml_data;
        LOG("------------------------------------------");
        while(p_tmp_data != NULL) {
            LOG("key=%s,value=%s.",p_tmp_data->key, p_tmp_data->value);
            p_tmp_data = p_tmp_data->next;
        }
        p_tmp_apn_data_list = p_tmp_apn_data_list->next;
    }
    
    return 0;
}