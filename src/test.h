/*
 * test.h
 *
 *  Created on: 29 Jan 2021
 *      Author: Luqman_Hakim
 */

#ifndef SRC_TEST_H_
#define SRC_TEST_H_

#include "main.h"

void tinyGsm_http_test()
{
	TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
	SerialMon.println("Initializing modem...");
	gsm.restart();
	// modem.init();

	String modemInfo = gsm.getModemInfo();
	SerialMon.print("Modem Info: ");
	SerialMon.println(modemInfo);

	gsm.gprsConnect(apn, gprsUser, gprsPass);

	SerialMon.print("Waiting for network...");
	if(!gsm.waitForNetwork())
	{
		SerialMon.println(" fail");
		delay(10000);
		return;
	}
	SerialMon.println(" success");

	if(gsm.isNetworkConnected())
	{
		SerialMon.println("Network connected");
	}

	// GPRS connection parameters are usually set after network registration
	SerialMon.print(F("Connecting to "));
	SerialMon.print(apn);
	if(!gsm.gprsConnect(apn, gprsUser, gprsPass))
	{
		SerialMon.println(" fail");
		delay(10000);
		return;
	}
	SerialMon.println(" success");

	if(gsm.isGprsConnected())
	{
		SerialMon.println("GPRS connected");
	}

	delay(5000);
retry:
	SerialMon.print(F("Performing HTTP GET request... "));
	int err = http_gsm.get(resource);
	if(err != 0)
	{
		SerialMon.println(F("failed to connect"));
		goto retry;
		delay(10000);
		return;
	}
	int status = http_gsm.responseStatusCode();
	SerialMon.print(F("Response status code: "));
	SerialMon.println(status);
	if(!status)
	{
		delay(10000);
		return;
	}

	SerialMon.println(F("Response Headers:"));
	while(http_gsm.headerAvailable())
	{
		String headerName = http_gsm.readHeaderName();
		String headerValue = http_gsm.readHeaderValue();
		SerialMon.println("    " + headerName + " : " + headerValue);
	}

	int length = http_gsm.contentLength();
	if(length >= 0)
	{
		SerialMon.print(F("Content length is: "));
		SerialMon.println(length);
	}
	if(http_gsm.isResponseChunked())
	{
		SerialMon.println(F("The response is chunked"));
	}

	String body = http_gsm.responseBody();
	SerialMon.println(F("Response:"));
	SerialMon.println(body);

	SerialMon.print(F("Body length is: "));
	SerialMon.println(body.length());

	// Shutdown

	http_gsm.stop();
	SerialMon.println(F("Server disconnected"));

	gsm.gprsDisconnect();
	SerialMon.println(F("GPRS disconnected"));
}

void esp_http_test()
{

	Serial.print("[HTTP] begin...\n");
	// configure traged server and url
	//http_esp.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
	http_esp.begin("http://example.com/index.html"); //HTTP

	Serial.print("[HTTP] GET...\n");
	// start connection and send HTTP header
	int httpCode = http_esp.GET();

	// httpCode will be negative on error
	if(httpCode > 0)
	{
		// HTTP header has been send and Server response header has been handled
		Serial.printf("[HTTP] GET... code: %d\n", httpCode);

		// file found at server
		if(httpCode == HTTP_CODE_OK)
		{
			String payload = http_esp.getString();
			Serial.println(payload);
		}
	}
	else
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http_esp.errorToString(httpCode).c_str());
	}

	http_esp.end();
}

void esp_twilio_test()
{
	http_esp.begin("https://api.twilio.com/2010-04-01/Accounts/AC8f985c03276eb425f492487c373e03df/Messages.json");
	//http.begin("http://192.168.0.9:8090/post");  //Specify destination for HTTP request
	http_esp.setAuthorization("AC8f985c03276eb425f492487c373e03df", "250a62d63ee0cb85476b95614ad96704");
	http_esp.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header

	String payload = get_sensor_as_string(&sensor);

	int httpResponseCode = http_esp.POST("To=whatsapp:%2B6283831076129"
											"&From=whatsapp:%2B14155238886"
											"&Body=" + payload); //Send the actual POST request

	if(httpResponseCode > 0)
	{

		String response = http_esp.getString();                       //Get the response to the request

		Serial.println(httpResponseCode);   //Print return code
		Serial.println(response);           //Print request answer

	}
	else
	{

		Serial.print("Error on sending POST: ");
		Serial.println(httpResponseCode);

	}

	http_esp.end();  //Free resources
}



#endif /* SRC_TEST_H_ */
