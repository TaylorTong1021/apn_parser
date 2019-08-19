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
#include "log.h"

#include <string.h>
#include <stdbool.h>

/**
 * parse the volte-config xml file. you need to run one time in the inital phase.
 *
 * Author: Yongli.Tong
 * Date  : 2019/07/03
 */
int parseConfigXml(char* fileName, char* numberic, unsigned int parse_type, 
                    unsigned int get_node_type, int (*callfunc)(xml_data_node*))
{
    int result = RETURN_ERROR;

    if(NULL == numberic || NULL == fileName) {
        LOG("numberic or fileName is NULL!");
        return RETURN_ERROR;
    }

    result = xml_open(fileName);
    if(RETURN_OK != result)
    {
        LOG("xml_open error.");
        return RETURN_ERROR;
    }

    result = xml_parse(callfunc, parse_type, get_node_type);

    if(RETURN_OK != result)
    {
        LOG("xml_parse error.");
        return RETURN_ERROR;
    }

    xml_close();

    return RETURN_OK;
}

