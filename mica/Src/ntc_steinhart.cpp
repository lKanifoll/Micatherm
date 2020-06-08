#include "ntc_steinhart.h"
#include "cmath"

temperature_sensor_ntc::temperature_sensor_ntc(uint32_t r25, uint32_t bal_res, uint16_t beta, uint32_t(*adc_raw_cb)())
	: R25(r25)
	, BALANCE_RESISTOR(bal_res)
	, BETA25_100(beta)
{
	KELVIN = 273.5;
	raw_adc_callback = adc_raw_cb;
}


float temperature_sensor_ntc::get_sensor_temp()
{
	uint32_t raw_adc = raw_adc_callback();
	
	if (raw_adc >= 50 && raw_adc <= 4000)
	{
		float R = BALANCE_RESISTOR * ((4095.0 / raw_adc) - 1);
		float steinhart;
		steinhart = R / R25;    // (R/Ro)
		steinhart = std::log(float(steinhart));   // ln(R/Ro)
		steinhart /= BETA25_100;    // 1/B * ln(R/Ro)
		steinhart += 1.0 / (25 + KELVIN);     // + (1/To)
		steinhart = 1.0 / steinhart;   // Invert
		steinhart -= KELVIN;    // convert to C
		/*if((steinhart <= (temp_steinhart + 3)) && (steinhart >= (temp_steinhart - 3))) // If temp increase or decrease impulsively, we skip averaging
		{
			steinhart = (steinhart * 0.3) + (temp_steinhart * 0.7); // Avereging
		}
		temp_steinhart = steinhart;*/
		return (steinhart);
	}
	else if (raw_adc < 50)
	{
		return -55.0;
	}
	else if (raw_adc > 4000)
	{
		return 127.0;
	}
	return 0;
}