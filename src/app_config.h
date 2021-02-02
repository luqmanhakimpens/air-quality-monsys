/*
 * config.h
 *
 *  Created on: Jul 23, 2019
 *      Author: luqma
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#define LOG_ENABLE
#define LOG_PORT Serial

#define FW_VERSION "0.1"

#define NTP_SERVER "pool.ntp.org"
#define GMT_PLUS_7 3600*7
#define DAYLIGHT_OFFSET_NONE 0

#define SENSING_INTERVAL_MS 5000
#define TELEGRAM_UPDATE_INTERVAL_MS 3000

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

#define LED 2

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "1495074745:AAH-gZZsuVAFpMloyXXicOmoK8XTHcBCZ90"

#define MODE_REQUEST 0
#define MODE_CONTINUOUS 1

#define SerialAT Serial2
#define SerialMon Serial

#define TINY_GSM_MODEM_SIM800 //must be defined before including tinyGsmClient.h

#define SHARP_LED_PIN 33  // Sharp Dust/particle sensor Led Pin
#define SHARP_VO_PIN 32 // sharp vout

#define DHTPIN 25     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#endif /* APP_CONFIG_H_ */
