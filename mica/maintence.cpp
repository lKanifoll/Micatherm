#include "main.h"
#include "maintence.h"
#include "Pixels.h"
#include <Pixels_PPI8.h> 
#include <Pixels_ST7735.h>
#include "images.h"
#include "fonts.h"
#include "ntc_steinhart.h"
#include "buttons.h"

typedef struct menu_item
{
	uint8_t ID;
	prog_uchar *icon;
	char text[20];

} menu_item_t;

extern ADC_HandleTypeDef hadc1;
extern IWDG_HandleTypeDef hiwdg;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;

#define BG_COLOR	RGB(0x0, 0x0, 0x0)
#define MAIN_COLOR	RGB(0xFF, 0xFF, 0xFF)

Pixels pxs(130, 161);


button power_key(key1_GPIO_Port, key1_Pin);



static menu_item_t main_menu[] = { 
	{1, img_ok_png_comp, "OK"},
	{2, img_menu_setting_sound_on_png_comp, "Sound On"},
	{3, img_menu_setting_sound_off_png_comp, "Sound Off"}
	};



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

	
	
	
	uint8_t onoff = 128;
	char disp_out[3];	
	for (;;)
	{
		if (power_key.button_short_is_pressed() || power_key.button_continious_is_pressed())
		{
			pxs.clear();
			pxs.setColor(pxs.computeColor(MAIN_COLOR, onoff));
			pxs.fillRectangle(0,0,160,128);
			
			//pxs.clear();
			//pxs.drawCompressedBitmap(20, 20, main_menu[onoff].icon);
			//onoff++;
			//if (onoff == 3) onoff = 0;
			/*
			pxs.cleanText(60, 40, disp_out);
			snprintf(disp_out, sizeof(disp_out), "%d", main_menu[onoff].ID);
			pxs.print(60, 40, disp_out);
			onoff++;
			if (onoff == 3) onoff = 0;
			*/
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