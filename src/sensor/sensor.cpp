#include "sensor.h"
#include "app_config.h"

DHT dht(DHTPIN, DHTTYPE);

void sensor_init(sensor_value_t * sensor)
{
	pinMode(SHARP_LED_PIN, OUTPUT);
	dht.begin();delay(500);
	read_sensor(sensor); //read for firts time
}

bool read_dht(float *humidity, float *temperature)
{
	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float h = dht.readHumidity();
	// Read temperature as Celsius (the default)
	float t = dht.readTemperature();

	// Check if any reads failed and exit early (to try again).
	if(isnan(h) || isnan(t))
	{
		LOG_PRINTLN(F("Failed to read from DHT sensor!"));
		h = 0;
		t = 0;
		return true;
	}

	*humidity = h;
	*temperature = t;
	return false;
}

float read_dust_density(uint16_t samples)
{
	uint32_t total = 0;
	uint16_t avg_raw;
	uint16_t raw = 0;
	static float zero_dust_v = 0.25;
	static float data[10] = {0};
	float avg_data = 0;
	static int n=0;

	for(uint8_t i = 0; i < samples; i++)
	{
		// Turn on the dust sensor LED by setting digital pin LOW.
		digitalWrite(SHARP_LED_PIN, LOW);

		// Wait 0.28ms before taking a reading of the output voltage as per spec.
		delayMicroseconds(280);

		// Record the output voltage. This operation takes around 100 microseconds.
		raw = analogRead(SHARP_VO_PIN);

		// Turn the dust sensor LED off by setting digital pin HIGH.
		digitalWrite(SHARP_LED_PIN, HIGH);

		total += raw;
		// Wait for remainder of the 10ms cycle = 10000 - 280 - 100 microseconds.
		delayMicroseconds(9620);
	}

	avg_raw = total / samples;

	float v = (avg_raw * 3.3) / 4096.0;
	//float dustDensity = dV / K * 100.0;
	float d = (v - zero_dust_v) / 0.25 * 100.0;
	if(d <= 0)
	{
		zero_dust_v = v;
		d = 0.1;
	}

	data[n] = d; //get data, put on the right-most array

	for(int i=0; i<=n; i++)
	{
		avg_data += data[i];
	}
	avg_data /= n+1;

	if(n ==  9) // shift data to left if array is full
	{
		for(int i = 0; i<=8 ; ++i)
		{
			data[i] = data[i+1];
		}
	}
	else
	{
		n++; //shift array pointer to right
	}

	return avg_data;
}

bool read_sensor(sensor_value_t * sensor)
{
	bool err = read_dht(&sensor->humidity, &sensor->temperature);
	float dd = read_dust_density(10);

	sensor->dust_density = dd;

	if(err)return false;
	else return true;
}

char* get_sensor_as_string(sensor_value_t * sensor)
{
	//	humidity: 85.20%
	//	temperature: 27.50 degreeC
	//	dustdensity: 22.80 uG/M

	static char result[100] =
	{ 0 };

	sprintf(result, "humidity: %2.2f%%\ntemperature: %2.2f degreeC\ndustdensity: %3.2f uG/M3\n",
				sensor->humidity, sensor->temperature, sensor->dust_density);

	LOG_PRINTLN(result);
	return result;
}


