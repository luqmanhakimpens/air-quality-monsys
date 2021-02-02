/*
 * network.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_NETWORK_NETWORK_CPP_
#define SRC_NETWORK_NETWORK_CPP_

#include <network/network.h>
#include "serial_log.h"

bool wifi_init(config_network_t * config)
{
	LOG_PRINTLN("wifi init");
	if(config->ap.password == "")
	{
		WiFi.softAP(config->ap.ssid.c_str(), NULL);
	}
	else
	{
		WiFi.softAP(config->ap.ssid.c_str(), config->ap.password.c_str());
	}

	LOG_PRINT("AP: ");
	LOG_PRINTLN(config->ap.ssid);
	LOG_PRINT("Connecting to: ");
	LOG_PRINTLN(config->sta.ssid);

	WiFi.begin(config->sta.ssid.c_str(), config->sta.password.c_str());
	if(WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		LOG_PRINTLN("WiFi Failed!\n");
		return false;
	}

	LOG_PRINTLN("");
	LOG_PRINTLN("WiFi connected.");
	LOG_PRINTLN(WiFi.localIP());

	return true;
}


#endif /* SRC_NETWORK_NETWORK_CPP_ */
