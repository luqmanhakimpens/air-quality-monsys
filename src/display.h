#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include "Nextion.h"
#include "time_manager/time_manager.h"
#include "serial_log.h"
#include "callbacks.h"
#include "config/config.h"

#define DISPLAY_COLOR_ORANGE	64520
#define DISPLAY_COLOR_RED	 	63488
#define DISPLAY_COLOR_YELLOW	65504
#define DISPLAY_COLOR_GREEN		2016
#define DISPLAY_COLOR_BLACK		0

// Declare your Nextion objects - Example (page id = 0, component id = 1, component name = "b0")
NexText textDate = NexText(0, 1, "textDate");
NexText textTime = NexText(0, 17, "textTime");
NexText textBatt = NexText(0, 3, "textBatt");

NexText textDust = NexText(0, 16, "textDust");
NexText textTemp = NexText(0, 11, "textTemp");
NexText textHumid = NexText(0, 12, "textHumid");

NexText textTempHigh = NexText(0, 10, "textTempHigh");
NexText textTempMid = NexText(0, 9, "textTempMid");
NexText textTempLow = NexText(0, 8, "textTempLow");

NexVariable varTemp = NexVariable(0,19,"varTemp");
NexVariable varHumid = NexVariable(0,20,"varHumid");
NexVariable varDust = NexVariable(0,21,"varDust");

NexVariable varDate = NexVariable(0,22,"varDate");
NexVariable varTime = NexVariable(0,23,"varTime");
NexVariable varBatt = NexVariable(0,24,"varBatt");
NexVariable varDim = NexVariable(0,25,"varDim");
NexVariable varNet = NexVariable(0,36,"varNet");

NexVariable varDustThLow = NexVariable(0,26,"varDustThLow");
NexVariable varDustThHi = NexVariable(0,27,"varDustThHi");

NexVariable varTempThLow = NexVariable(0,28,"varTempThLow");
NexVariable varTempThHi = NexVariable(0,29,"varTempThHi");
NexVariable varHumidThLow = NexVariable(0,30,"varHumidThLow");
NexVariable varHumidThHi = NexVariable(0,31,"varHumidThHi");

NexVariable varTempSclLow = NexVariable(0,32,"varTempSclLow");
NexVariable varTempSclHi = NexVariable(0,33,"varTempSclHi");
NexVariable varHumidSclLow = NexVariable(0,34,"varHumidSclLow");
NexVariable varHumidSclHi = NexVariable(0,34,"varHumidSclHi");

NexGauge gaugeDust = NexGauge(0,15,"gaugeDust");
NexSlider sliderTemp = NexSlider(0, 13, "sliderTemp");
NexSlider sliderHumid = NexSlider(0, 14, "sliderHumid");

NexSlider sliderHSclLow = NexSlider(1,4,"sliderHSclLow");
NexSlider sliderHSclHi = NexSlider(1,5,"sliderHSclHi");
NexSlider sliderHThLow = NexSlider(1,8,"sliderHThLow");
NexSlider sliderHThHi = NexSlider(1,9,"sliderHThHi");

NexSlider sliderTSclLow = NexSlider(2,4,"sliderTSclLow");
NexSlider sliderTSclHi = NexSlider(2,5,"sliderTSclHi");
NexSlider sliderTThLow = NexSlider(2,8,"sliderTThLow");
NexSlider sliderTThHi = NexSlider(2,9,"sliderTThHi");

NexSlider sliderDThLow = NexSlider(3,4,"sliderDThLow");
NexSlider sliderDThHi = NexSlider(3,5,"sliderDThHi");

NexSlider sliderBright = NexSlider(4,4,"sliderBright");

NexText textMessageH = NexText(1, 28, "t4");
NexText textMessageT = NexText(2, 28, "t4");
NexText textMessageD = NexText(3, 17, "t4");
NexText textMessageC = NexText(4, 11, "t4");

NexButton buttonSaveH = NexButton(1,12,"buttonSave");
NexButton buttonSaveT = NexButton(2,12,"buttonSave");
NexButton buttonSaveD = NexButton(3,8,"buttonSave");
NexButton buttonSaveC = NexButton(4,5,"buttonSave");

NexTimer timerPage0 = NexTimer(0,18,"tm0");
NexTimer timerPage1 = NexTimer(1,27,"tm0");
NexTimer timerPage2 = NexTimer(2,29,"tm0");
NexTimer timerPage3 = NexTimer(3,18,"tm0");
NexTimer timerPage4 = NexTimer(4,12,"tm0");

NexRadio radioGsm = NexRadio(4, 13, "radioGsm");

NexRtc nexRtc;

// Register a button object to the touch event list.
NexTouch *nex_listen_list[] = {
	&buttonSaveH,
	&buttonSaveT,
	&buttonSaveD,
	&buttonSaveC,
	NULL
};

void display_loop()
{
	nexLoop(nex_listen_list);
}

bool display_init(config_t* cfg)
{
	config_display_t cfg_display = cfg->display;

	if(nexInit())
	{
		LOG_PRINTLN("init display values");
		varNet.setValue(cfg->network.net_mode);
		varDim.setValue(cfg_display.dim);

		varDustThLow.setValue(cfg_display.dust.thLow);
		varDustThHi.setValue(cfg_display.dust.thHi);

		varTempThLow.setValue(cfg_display.temp.thLow);
		varTempThHi.setValue(cfg_display.temp.thHi);
		varTempSclLow.setValue(cfg_display.temp.sclLow);
		varTempSclHi.setValue(cfg_display.temp.sclHi);

		varHumidSclLow.setValue(cfg_display.humid.sclLow);
		varHumidSclHi.setValue(cfg_display.humid.sclHi);
		varHumidThLow.setValue(cfg_display.humid.thLow);
		varHumidThHi.setValue(cfg_display.humid.thHi);

		buttonSaveH.attachPop(button_save_humidity, &buttonSaveH);
		buttonSaveT.attachPop(button_save_temperature, &buttonSaveT);
		buttonSaveD.attachPop(button_save_dust, &buttonSaveD);
		buttonSaveC.attachPop(button_save_device, &buttonSaveC);

		return true;
	}
	else
	{
		return false;
	}
}

void display_value_update(float humid, float temp, float dust, config_display_t cfg)
{
	char value[10] = {0};
	sprintf(value,"%.1f%%",humid);
	textHumid.setText(value);
	varHumid.setValue((uint32_t)humid);
	memset(value,0,sizeof value);

	const char degrees_symbol = 0xb0;
	sprintf(value,"%.1f%cC",temp, degrees_symbol);
	textTemp.setText(value);
	varTemp.setValue((uint32_t)temp);
	memset(value,0,sizeof value);

	uint32_t d_to_angle = dust*180/300;
	if(d_to_angle>180)d_to_angle=180; //max gauge value

	varDust.setValue((uint32_t)dust);
	gaugeDust.setValue(d_to_angle);
	sprintf(value,"%d",(uint32_t)dust);

	if((uint32_t)dust >= cfg.dust.thHi)
	{
		gaugeDust.Set_font_color_pco(DISPLAY_COLOR_RED);
		textDust.Set_font_color_pco(DISPLAY_COLOR_RED);
	}
	else if((uint32_t)dust >= cfg.dust.thLow)
	{
		gaugeDust.Set_font_color_pco(DISPLAY_COLOR_ORANGE);
		textDust.Set_font_color_pco(DISPLAY_COLOR_ORANGE);
	}
	else
	{
		gaugeDust.Set_font_color_pco(DISPLAY_COLOR_GREEN);
		textDust.Set_font_color_pco(DISPLAY_COLOR_GREEN);
	}
	textDust.setText(value);

	String date = get_iso_time();
	String time = date.substring(date.indexOf('T') + 1, date.lastIndexOf(':'));
	date = date.substring(date.indexOf('-') + 1 , date.indexOf('T'));
	date.replace('-','/');

	uint8_t batt_value = 50; //random(50,100);
	char batt_value_str[4] = {0};
	itoa(batt_value,batt_value_str,10);

	varBatt.setText(batt_value_str);
	varDate.setText(date.c_str());
	varTime.setText(time.c_str());

	//char time_buf[30] = {0};
	//nexRtc.read_rtc_time(time_buf,30);
}

void nex_rtc_write()
{
	time_t nowtime;
	timeinfo_t *nowtm;
	nowtime = time(nullptr);
	nowtm = localtime(&nowtime);

	uint32_t time[7] =
	{ (uint32_t) nowtm->tm_year + 1900, (uint32_t) nowtm->tm_mon + 1, (uint32_t) nowtm->tm_mday, (uint32_t) nowtm
			->tm_hour, (uint32_t) nowtm->tm_min, (uint32_t) nowtm->tm_sec };

	nexRtc.write_rtc_time(time);
}

void nex_rtc_read(tm *time)
{
	uint32_t t[7] =
	{ 0 };
	nexRtc.read_rtc_time(t, 7);

	time->tm_year = t[0] - 1900; // time since 1900
	time->tm_mon = t[1] - 1;
	time->tm_mday = t[2];
	time->tm_hour = t[3];
	time->tm_min = t[4];
	time->tm_sec = t[5];
}

#endif
