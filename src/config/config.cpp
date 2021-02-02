/*
 * config.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#include <config/config.h>

void config_read(config_network_t * cfg)
{
	String config_str = spiffs_read(SPIFFS, CONFIG_FILE_NETWORK);
	StaticJsonDocument<512> doc;

	DeserializationError err;
	err = deserializeJson(doc, config_str);
	if(err)
	{
		LOG_PRINTLN(err.c_str());
	}

	cfg->net_mode = doc["net_mode"]; // 0
	cfg->apn = (const char*)doc["apn"]; // "internet"
	cfg->telegram_token = (const char*)doc["telegram_token"]; // "internet"

	cfg->ap.ssid = (const char*)doc["ap"]["ssid"]; // "envisense"
	cfg->ap.password = (const char*)doc["ap"]["password"]; // "envi1234"


	cfg->sta.ssid = (const char*)doc["sta"]["ssid"]; // "lts"
	cfg->sta.password = (const char*)doc["sta"]["password"]; // "55555555"

	if(cfg->ap.ssid == "")
	{
		tool tool;
		cfg->ap.ssid = tool.device_name("envi-");
		config_write(cfg); //write default ap ssid
	}

}

void config_write(config_network_t *cfg)
{
	StaticJsonDocument<512> doc;

	doc["net_mode"] = cfg->net_mode;
	doc["apn"] = cfg->apn;
	doc["telegram_token"] = cfg->telegram_token;

	JsonObject ap = doc.createNestedObject("ap");
	ap["ssid"] = cfg->ap.ssid;
	ap["password"] = cfg->ap.password;

	JsonObject sta = doc.createNestedObject("sta");
	sta["ssid"] = cfg->sta.ssid;
	sta["password"] = cfg->sta.password;

	String output;
	serializeJson(doc, output);

	//spiffs_delete(SPIFFS, CONFIG_FILE_NETWORK);
	spiffs_write(SPIFFS, CONFIG_FILE_NETWORK, output.c_str());
}

void config_read(config_display_t * cfg)
{
	String config_str = spiffs_read(SPIFFS, CONFIG_FILE_DISPLAY);
	StaticJsonDocument<384> doc;

	DeserializationError err;

	err = deserializeJson(doc, config_str);
	if(err)
	{
		LOG_PRINTLN(err.c_str());
	}

	cfg->dim = doc["dim"];  // 25

	JsonObject dust = doc["dust"];
	cfg->dust.thLow = dust["thLow"]; // 12
	cfg->dust.thHi = dust["thHi"]; // 12

	JsonObject temp = doc["temp"];
	cfg->temp.thLow = temp["thLow"]; // 12
	cfg->temp.thHi = temp["thHi"]; // 12
	cfg->temp.sclLow = temp["sclLow"]; // 44
	cfg->temp.sclHi = temp["sclHi"]; // 44

	JsonObject humid = doc["humid"];
	cfg->humid.thLow= humid["thLow"]; // 40
	cfg->humid.thHi = humid["thHi"]; // 70
	cfg->humid.sclLow = humid["sclLow"]; // 44
	cfg->humid.sclHi= humid["sclHi"]; // 44
}

void config_write(config_display_t *cfg)
{
	String json_str;
	StaticJsonDocument<256> doc;

	doc["dim"] = cfg->dim;

	JsonObject dust = doc.createNestedObject("dust");
	dust["thLow"] = cfg->dust.thLow;
	dust["thHi"] = cfg->dust.thHi;

	JsonObject temp = doc.createNestedObject("temp");
	temp["thLow"] = cfg->temp.thLow;
	temp["thHi"] = cfg->temp.thHi;
	temp["sclLow"] = cfg->temp.sclLow;
	temp["sclHi"] = cfg->temp.sclHi;

	JsonObject humid = doc.createNestedObject("humid");
	humid["thLow"] = cfg->humid.thLow;
	humid["thHi"] = cfg->humid.thHi;
	humid["sclLow"] = cfg->humid.sclLow;
	humid["sclHi"] = cfg->humid.sclHi;

	String output;
	serializeJson(doc, json_str);

	//spiffs_delete(SPIFFS, CONFIG_FILE_DISPLAY);
	spiffs_write(SPIFFS, CONFIG_FILE_DISPLAY, json_str.c_str());
}


void config_init(config_t * config)
{
	if(!SPIFFS.begin(true))
	{
		LOG_PRINTLN("An Error has occurred while mounting SPIFFS");
		return;
	}
	config_read(&config->network);
	config_read(&config->display);
}
