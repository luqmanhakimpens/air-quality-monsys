/*
 * webserver.h
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_WEBSERVER_WEBSERVER_H_
#define SRC_WEBSERVER_WEBSERVER_H_


#include <app_config.h>
#include <config/config.h>
#include <tool/tool.h>
#include <SPIFFSEditor.h>
#include "ESPAsyncWebServer.h"

#define APP_JSON F("application/json")
#define TEXT_HTML F("text/html")

void async_webserver_init(config_t * config);

#endif /* SRC_WEBSERVER_WEBSERVER_H_ */
