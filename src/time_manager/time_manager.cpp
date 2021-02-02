/*
 * time.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */


#include <serial_log.h>
#include <time_manager/time_manager.h>
#include "WiFi.h"

bool time_init(const char* ntp_server, long gmt_offset_sec, int daylight_offset_sec)
{
	bool ret_val = false;

	if(WiFi.status() == WL_CONNECTED)
	{
		LOG_PRINT("Setting time");
		configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
		while(time(nullptr) < 24 * 3600) //wait for time loaded correctly
		{
			static int timeout = 0;
			if(timeout++ < 30)
			{
				LOG_PRINT(".");
				delay(200);
			}
			else
			{
				ret_val = false;
				break;
			}
		}
		ret_val = true;
	}
	else
	{
		ret_val = false;
	}
	return ret_val;
}

char* get_iso_time()
{
	time_t nowtime;
	timeinfo_t *nowtm;
	char tmbuf[64];
	static char result[26];

	nowtime = time(nullptr);
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%Y-%m-%dT%H:%M:%S", nowtm); //2021-01-06T17:28:16
	snprintf(result, sizeof result, "%s", tmbuf);
	return result;
}

void set_time(struct tm *tm)
{
	time_t t = mktime(tm);
	Serial.printf("Setting time: %s", asctime(tm));

	struct timeval now =
	{ .tv_sec = t };
	settimeofday(&now, NULL);
}
