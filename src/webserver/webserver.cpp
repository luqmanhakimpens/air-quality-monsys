/*
 * webserver.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#include <webserver/webserver.h>
#include "serial_log.h"

AsyncWebServer server(80);
config_t *_config;

static String html_template_processor(const String &var)
{
	tool tool;

	if(var == "FW_VERSION")
	{
		return FW_VERSION;
	}
	else if(var == "AP_SSID")
	{
		return _config->network.ap.ssid;;
	}
	else if(var == "AP_PASSWORD")
	{
		return _config->network.ap.password;
	}
	else if(var == "STA_SSID")
	{
		return _config->network.sta.ssid;
	}
	else if(var == "STA_PASSWORD")
	{
		return _config->network.sta.password;
	}
	else if(var == "TELEGRAM_TOKEN")
	{
		return _config->network.telegram_token;
	}
	else if(var == "APN")
	{
		return _config->network.apn;
	}
	else if(var == "NET_MODE")
	{
		return String(_config->network.net_mode);
	}
	else if(var == "DEVICE_ID")
	{
		return tool.device_id();
	}
	return "";
}

void network(AsyncWebServerRequest *request)
{
	if(request->hasArg("network_mode"))
	{
		//config_t cfg;
		config_read(&_config->network);

		String net_mode = request->arg("network_mode");
		if(net_mode == "gsm")
		{
			_config->network.net_mode = 1;
			_config->network.apn = request->arg("apn");
		}
		else
		{
			_config->network.net_mode = 0;
			_config->network.sta.ssid = request->arg("sta_ssid");
			_config->network.sta.password = request->arg("sta_password");
		}

		//config->network.net_mode = request->arg("net_mode");
		_config->network.telegram_token = request->arg("telegram_token");

		Serial.println(request->arg("network_mode"));
		Serial.println(request->arg("apn"));
		Serial.println(request->arg("telegram_token"));

		_config->network.ap.ssid = request->arg("ap_ssid");
		_config->network.ap.password = request->arg("ap_password");
		config_write(&_config->network);

		request->send(
				200,
				"text/html",
				"Setting parameter success, please return home page and reboot soon! <br><a href=\"/\">Return to Home Page</a>");
	}
	request->send(
			200,
			"text/html",
			"Setting parameter Failed, bad request, please return home page! <br><a href=\"/\">Return to Home Page</a>");

}

void root(AsyncWebServerRequest *request)
{
	request->send(SPIFFS, "/index.html", String(), false, html_template_processor);
}


void reboot(AsyncWebServerRequest *request)
{
	request->send(200, "text/html", "Device Rebooted");
	vTaskDelay(3000 / portTICK_RATE_MS);
	ESP.restart();
}

void async_webserver_init(config_t *config)
{
	_config = config;

	server.on("/", HTTP_GET, root);
	server.on("/reboot", HTTP_GET, reboot);
	server.on("/network", HTTP_POST, network);

	server.on("/src/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
	{
		request->send(SPIFFS, "/src/bootstrap.bundle.min.js", "text/javascript");
	});

	server.on("/src/jquery-3.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
	{
		request->send(SPIFFS, "/src/jquery-3.3.1.min.js", "text/javascript");
	});

	server.on("/src/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
	{
		request->send(SPIFFS, "/src/bootstrap.min.css", "text/css");
	});

	server.addHandler(new SPIFFSEditor(SPIFFS, "admin", "admin"));
	server.begin();
}
