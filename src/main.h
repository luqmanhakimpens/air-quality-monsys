/*
 * main.h
 *
 *  Created on: 29 Jan 2021
 *      Author: Luqman_Hakim
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#include <config/config.h>
#include <network/network.h>
#include <time_manager/time_manager.h>
#include <tool/tool.h>
#include <webserver/webserver.h>
#include "Arduino.h"
#include "sensor/sensor.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "display.h"

#include <TinyGsmClient.h>
#include "ArduinoHttpClient.h"  //http for gsm client
#include "HTTPClient.h" //http for wifi client

// Server details
const char server[] = "vsh.pp.ua";
const char resource[] = "/TinyGSM/logo.txt";
const int port = 80;

// Your GPRS credentials, if any
const char apn[] = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

TinyGsm gsm(SerialAT);
TinyGsmClient client_gsm(gsm);
HttpClient http_gsm(client_gsm, server, port);
HTTPClient http_esp;

//HttpClient http(client_gsm, server, port);

WiFiClientSecure secured_client; // @suppress("Abstract class cannot be instantiated")
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

const unsigned long BOT_MTBS = 3000; // mean time between scan messages
// last time messages' scan has been done
bool Start = false;
bool report_mode = MODE_REQUEST;
String chat_id;
time_t now;

sensor_value_t sensor;

config_t config;
TaskHandle_t task_handle_sensing;
TaskHandle_t task_handle_telegram;

uint16_t telegram_report_interval_mnt = 15;


#endif /* SRC_MAIN_H_ */
