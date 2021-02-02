#include "main.h"
#include "test.h"

void button_save_humidity(void *ptr)
{
	// NexButton *btn = (NexButton *)ptr; for referencing button obj
	uint32_t humid_th_low = 0;
	uint32_t humid_th_hi = 0;
	uint32_t humid_scl_low = 0;
	uint32_t humid_scl_hi = 0;

	sliderHSclLow.getValue(&humid_scl_low);
	sliderHSclHi.getValue(&humid_scl_hi);
	sliderHThLow.getValue(&humid_th_low);
	sliderHThHi.getValue(&humid_th_hi);

	LOG_PRINTLN(humid_scl_low);
	LOG_PRINTLN(humid_scl_hi);
	LOG_PRINTLN(humid_th_low);
	LOG_PRINTLN(humid_th_hi);

	config_display_t cfg;
	config_read(&cfg); // read first before overwriting to preserve old values that won't be updated
	// update new values
	cfg.humid.sclLow = humid_scl_low;
	cfg.humid.sclHi = humid_scl_hi;
	cfg.humid.thLow = humid_th_low;
	cfg.humid.thHi = humid_th_hi;
	config_write(&cfg);
	config.display = cfg;

	textMessageH.setText("config saved");
	timerPage1.enable(); //fires timer for message pop-up purpose
}

void button_save_temperature(void *ptr)
{
	// NexButton *btn = (NexButton *)ptr; for referencing button obj
	uint32_t temp_th_low = 0;
	uint32_t temp_th_hi = 0;
	uint32_t temp_scl_low = 0;
	uint32_t temp_scl_hi = 0;

	sliderTSclLow.getValue(&temp_scl_low);
	sliderTSclHi.getValue(&temp_scl_hi);
	sliderTThLow.getValue(&temp_th_low);
	sliderTThHi.getValue(&temp_th_hi);

	LOG_PRINTLN(temp_scl_low);
	LOG_PRINTLN(temp_scl_hi);
	LOG_PRINTLN(temp_th_low);
	LOG_PRINTLN(temp_th_hi);

	config_display_t cfg;
	config_read(&cfg); // read first before overwriting to preserve old values that won't be updated
	// update new values
	cfg.temp.sclLow = temp_scl_low;
	cfg.temp.sclHi = temp_scl_hi;
	cfg.temp.thLow = temp_th_low;
	cfg.temp.thHi = temp_th_hi;
	config_write(&cfg);
	config.display = cfg;

	textMessageT.setText("config saved");
	timerPage2.enable(); //fires timer for message pop-up purpose
}

void button_save_dust(void *ptr)
{
	// NexButton *btn = (NexButton *)ptr; for referencing button obj
	uint32_t dust_th_low = 0;
	uint32_t dust_th_hi = 0;

	sliderDThLow.getValue(&dust_th_low);
	sliderDThHi.getValue(&dust_th_hi);

	LOG_PRINTLN(dust_th_low);
	LOG_PRINTLN(dust_th_hi);

	config_display_t cfg;
	config_read(&cfg); // read first before overwriting to preserve old values that won't be updated

	// update new values
	cfg.dust.thLow = dust_th_low;
	cfg.dust.thHi = dust_th_hi;
	config_write(&cfg);
	config.display = cfg;

	textMessageH.setText("config saved");
	timerPage1.enable(); //fires timer for message pop-up purpose
}

void button_save_device(void *ptr)
{
	// NexButton *btn = (NexButton *)ptr; for referencing button obj
	uint32_t net_mode = 0;
	uint32_t dim = 0;

	sliderBright.getValue(&dim);
	radioGsm.getValue(&net_mode);

	LOG_PRINTLN(dim);
	LOG_PRINTLN(net_mode);

	config_t cfg;
	config_read(&cfg.display); // read first before overwriting to preserve old values that won't be updated
	// update new values
	cfg.display.dim = dim;
	config_write(&cfg.display);

	config_read(&cfg.network);
	cfg.network.net_mode = (bool) net_mode;
	config_write(&cfg.network);

	config.display = cfg.display;
	config.network = cfg.network;

	textMessageC.setText("config saved");
	timerPage4.enable(); //fires timer for message pop-up purpose
}

void telegram_on_message(int numNewMessages)
{
	LOG_PRINTLN("handleNewMessages");
	LOG_PRINTLN(String(numNewMessages));

	for(int i = 0; i < numNewMessages; i++)
	{
		chat_id = bot.messages[i].chat_id;
		String message = bot.messages[i].text;

		if(message.indexOf("/continuous") >= 0)
		{
			report_mode = MODE_CONTINUOUS;
			telegram_report_interval_mnt = message.substring(message.indexOf('=') + 1).toInt();
			String message = "continuous mode for every: " + String(telegram_report_interval_mnt) + " mnt";
			bot.sendMessage(chat_id, message);
			LOG_PRINTLN(message);
		}

		else if(message == "/sensor")
		{
			report_mode = MODE_REQUEST;
			LOG_PRINTLN("mode request");
			bot.sendMessage(chat_id, get_sensor_as_string(&sensor));
		}
		else
		{
			String hello_message =
				"bot ready, /sensor to request sensor value, /continuous=xx to get continuous report every xx minute.";
			bot.sendMessage(chat_id, hello_message);
		}
	}
}

void task_telegram(void *paramater)
{
	while(1)
	{
		if(WiFi.status() == WL_CONNECTED)
		{
			digitalWrite(LED, HIGH);
			LOG_PRINT("check for telegram message. time: ");
			LOG_PRINT(get_iso_time());
			LOG_PRINT(".....");
			bot.longPoll = 1;
			int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

			while(numNewMessages)
			{
				Serial.println("got response");
				telegram_on_message(numNewMessages);
				numNewMessages = bot.getUpdates(bot.last_message_received + 1);
			}
			LOG_PRINTLN("DONE");

			static uint16_t counter = 0;
			static unsigned long last_millis = 0;
			if(millis() - last_millis > 60000)
			{
				last_millis = millis();
				if(report_mode == MODE_CONTINUOUS && ++counter >= telegram_report_interval_mnt)
				{
					LOG_PRINTLN("send telegram report");
					bot.sendMessage(chat_id, get_sensor_as_string(&sensor));
					counter = 0;
				}
			}

		}

		digitalWrite(LED, LOW);
		vTaskDelay(TELEGRAM_UPDATE_INTERVAL_MS / portTICK_RATE_MS);
	}
}

void task_sensing(void *paramater)
{
	static unsigned long last_millis = 0;
	LOG_PRINTLN("sensor ready");
	while(1)
	{
		display_loop();
		if(millis() - last_millis > SENSING_INTERVAL_MS)
		{
			if(read_sensor(&sensor))
			{
				display_value_update(sensor.humidity, sensor.temperature, sensor.dust_density, config.display);
			}
			last_millis = millis();
		}
	}
}

void task_create_sensing()
{
	xTaskCreatePinnedToCore(task_sensing, /* Task function. */
							"task_sensing", /* name of task. */
							15000, /* Stack size of task */
							NULL, /* parameter of the task */
							2, /* priority of the task */
							&task_handle_sensing, /* Task handle to keep track of created task */
							1); /* pin task to core 1 */
	delay(100);
}

void task_create_telegram()
{
	xTaskCreatePinnedToCore(task_telegram, /* Task function. */
							"task_telegram", /* name of task. */
							15000, /* Stack size of task */
							NULL, /* parameter of the task */
							1, /* priority of the task */
							&task_handle_telegram, /* Task handle to keep track of created task */
							0); /* pin task to core 1 */
	delay(100);
}

void setup()
{
	Serial.begin(115200);
	pinMode(LED, OUTPUT);

	config_init(&config);
	LOG_PRINTLN("config init OK");

	if(wifi_init(&config.network))
	{
		LOG_PRINTLN("wifi init OK");
	}
	else
	{
		LOG_PRINTLN("wifi init FAILED");
	}

	secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
	async_webserver_init(&config);
	LOG_PRINTLN("async webserver init OK");

	delay(100);
	if(display_init(&config))
	{
		LOG_PRINTLN("display init OK");
	}
	else
	{
		LOG_PRINTLN("display init FAILED");
	}
	delay(100);

	if(time_init(NTP_SERVER, GMT_PLUS_7, DAYLIGHT_OFFSET_NONE))
	{
		LOG_PRINTLN(" time init OK, sync to nex_rtc");
		nex_rtc_write();
	}
	else
	{
		LOG_PRINTLN(" time init FAILED, sync from nex_rtc");
		tm time;
		nex_rtc_read(&time);
		set_time(&time);
	}
	randomSeed(now);

	sensor_init(&sensor);
	while(!read_sensor(&sensor))
	{
		LOG_PRINTLN("wait for sensor ready");
		vTaskDelay(500/portTICK_RATE_MS);
	}

	LOG_PRINTLN("sensor init OK");
	LOG_PRINTLN("enter loop");

	//tinyGsm_http_test();
	//esp_http_test();
	//esp_twilio_test();

	if(config.network.telegram_token != "")
	{
		bot.updateToken(config.network.telegram_token);
	}

	task_create_sensing();

	if(bot.getToken() != "")
	{
		task_create_telegram();
	}

}

void loop()
{
	vTaskDelay(1000 / portTICK_RATE_MS);
}
