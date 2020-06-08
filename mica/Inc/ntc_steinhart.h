#pragma once
#include "stdint.h"

class temperature_sensor_ntc
{
public:
	temperature_sensor_ntc(uint32_t R25, uint32_t BALANCE_RESISTOR, uint16_t B25_100, uint32_t(*adc_raw_cb)());
	float get_sensor_temp();
	uint32_t(*raw_adc_callback)();
private:
	const uint32_t R25;
	const uint32_t BALANCE_RESISTOR;
	const uint16_t BETA25_100;
	float KELVIN;
};