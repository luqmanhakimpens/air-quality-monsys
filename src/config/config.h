/*
 * config.h
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_CONFIG_CONFIG_H_
#define SRC_CONFIG_CONFIG_H_

#include <spiffs_io/spiffs_io.h>
#include "ArduinoJson.h"
#include "serial_log.h"
#include "tool/tool.h"

#define CONFIG_FILE_NETWORK "/config/network.json"
#define CONFIG_FILE_DISPLAY "/config/display.json"

typedef struct
{
	String ssid;
	String password;
}wifi_credential_t;
typedef struct
{
	bool net_mode;
	String apn;
	String telegram_token;
	wifi_credential_t ap;
	wifi_credential_t sta;
} config_network_t;

typedef struct
{
	uint32_t thLow;
	uint32_t thHi;
}display_dust_t;

typedef struct
{
	uint32_t thLow;
	uint32_t thHi;
	uint32_t sclLow;
	uint32_t sclHi;
}display_humid_t;

typedef struct
{
	uint32_t thLow;
	uint32_t thHi;
	uint32_t sclLow;
	uint32_t sclHi;
}display_temp_t;

typedef struct
{
	uint8_t dim;
	display_temp_t temp;
	display_humid_t humid;
	display_dust_t dust;
}config_display_t;

typedef struct
{
	config_display_t display;
	config_network_t network;
} config_t;

void config_read(config_network_t * cfg);
void config_write(config_network_t *config);
void config_read(config_display_t * cfg);
void config_write(config_display_t *cfg);
void config_init(config_t * config);

#endif /* SRC_CONFIG_CONFIG_H_ */
