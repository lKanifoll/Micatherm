#include "main.h"
#include "maintence.h"
#include "Pixels.h"
#include <Pixels_PPI8.h> 
#include <Pixels_ST7735.h>
#include "images.h"
#include "fonts.h"
#include "ntc_steinhart.h"
#include "buttons.h"


extern ADC_HandleTypeDef hadc1;
extern IWDG_HandleTypeDef hiwdg;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;

#define BG_COLOR	RGB(0x0, 0x0, 0x0)
#define MAIN_COLOR	RGB(0x00, 0xFF, 0xFF)

Pixels pxs(130, 161);


button power_key(key1_GPIO_Port, key1_Pin);

void button_timer_callback(void *argument)
{
	power_key.check_button_state();	
}

void buttons_task(void *argument)
{
	pxs.setOrientation(LANDSCAPE);
	pxs.enableAntialiasing(true);
	pxs.init();
	pxs.setBackground(BG_COLOR);
	pxs.setColor(MAIN_COLOR); 
	pxs.clear();
	pxs.displayOn();
	pxs.setFont(ElectroluxSansRegular14a);

	
	
	
	uint8_t onoff = 0;
	char temperature[3];	
	for (;;)
	{
		if (power_key.button_short_is_pressed() || power_key.button_continious_is_pressed())
		{
			pxs.cleanText(60, 40, temperature);
			snprintf(temperature, sizeof(temperature), "%d", ++onoff);
			pxs.print(60, 40, temperature);
		}
		osDelay(10);	
	}

}



void screen_smooth_transition()
{
	while (TIM3->CCR1 > 1000)
	{
		TIM3->CCR1 -= 1000;
		HAL_Delay(1);
	}
	HAL_Delay(100);
	while (TIM3->CCR1 < 65000)
	{
		TIM3->CCR1 += 500;
		HAL_Delay(2);
	}
}

uint32_t get_raw_adc_meas()
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	uint32_t raw = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	return raw;
}