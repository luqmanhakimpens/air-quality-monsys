#ifndef SENSOR_H
#define SENSOR_H

#include "DHT_U.h"
#include "Arduino.h"
#include <serial_log.h>


typedef struct
{
	float temperature;
	float humidity;
	float dust_density;
}sensor_value_t;

void sensor_init(sensor_value_t * sensor);
bool read_dht(float *humidity, float *temperature);
float read_dust_density(uint16_t samples);
bool read_sensor(sensor_value_t * sensor);
char* get_sensor_as_string(sensor_value_t * sensor);

#endif
