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
	uint16_t ID;
	uint8_t item_count;
	prog_uchar *icon;
	char text[20];
	menu_item *next_menu;
	menu_item *prev_menu;

} menu_item_t;

extern ADC_HandleTypeDef hadc1;
extern IWDG_HandleTypeDef hiwdg;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;

#define BG_COLOR	RGB(0x0, 0x0, 0x0)
#define MAIN_COLOR	RGB(0xFF, 0xFF, 0xFF)

Pixels pxs(DISPLAY_WIDTH, DISPLAY_HEIGHT);


button enter_key(key2_GPIO_Port, key2_Pin);
button back_key(key3_GPIO_Port, key3_Pin);
button up_key(key4_GPIO_Port, key4_Pin);
button down_key(key5_GPIO_Port, key5_Pin);

static menu_item_t heatmode_menu[] = { 
	{ 0, 1, img_menu_mode_comfort_png_comp, "Comfort" },
	{ 1, 1 ,img_menu_mode_eco_png_comp, "Eco" },
	{ 2, 1 ,img_menu_mode_anti_png_comp, "Anti-frost" }
};

static menu_item_t timer_menu[] = { 
	{ 0, 0, img_menu_settimer_png_comp, "Set timer" }

};

static menu_item_t main_menu[] = { 
	{0, 3, img_menu_heatmode_icon_png_comp, "Heat mode", heatmode_menu, NULL},
	{1, 1, img_menu_timer_icon_png_comp, "Timer", timer_menu, NULL},
	{2, 4, img_menu_setting_icon_png_comp, "Settings", NULL},
	{3, 3, img_menu_program_icon_png_comp, "Programme", NULL}
	};

static menu_item_t menu[] = { 
	{ 0, 4, NULL, "", main_menu, NULL }
};
menu_item_t *current_menu = NULL;
menu_item_t *tmp_current_menu = NULL;
uint8_t prev_current_pos = 0;
void button_timer_callback(void *argument)
{
	enter_key.check_button_state();	
	back_key.check_button_state();
	up_key.check_button_state();	
	down_key.check_button_state();	
	
}

void buttons_task(void *argument)
{
	pxs.setOrientation(PORTRAIT);
	pxs.enableAntialiasing(true);
	pxs.init();
	pxs.setBackground(BG_COLOR);
	pxs.setColor(MAIN_COLOR); 
	pxs.clear();
	pxs.displayOn();
	pxs.setFont(ElectroluxSansRegular10a);
	TIM3->CCR1 = 65535;
	
	
	int16_t pic_width = 0;
	int16_t pic_height = 0;
	uint8_t menu_item = 0;
	char disp_out[3];	
	uint8_t menu_size = 0;

	for (;;)
	{
		
		if (enter_key.button_short_is_pressed())
		{
			//menu_item = 0;
			pxs.clear();
			if(current_menu == NULL) 
			{
				
				current_menu = main_menu;
				current_menu->prev_menu = menu;
			}
			else
			{
				tmp_current_menu = current_menu;
				current_menu = current_menu[menu_item].next_menu;
				current_menu->prev_menu = tmp_current_menu;
				prev_current_pos = menu_item;
			}
			menu_item = 0;
			pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu[0].icon);
			pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu[0].icon);
			pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu[0].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu[0].text);
			pxs.setColor(MAIN_COLOR);
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu[0].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu[0].text);	

		}		
		
		if (back_key.button_short_is_pressed())
		{
			
			if(current_menu == NULL) 
			{
				continue;
			}
			else
			{
				tmp_current_menu = current_menu;
				current_menu = tmp_current_menu->prev_menu;
				//current_menu->prev_menu = ;
			}			
			menu_item = prev_current_pos;
			pxs.clear();
			pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu[menu_item].icon);
			pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu[menu_item].icon);
			pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu[menu_item ? (menu_item - 1) : 3].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu[menu_item ? (menu_item - 1) : 3].text);
			pxs.setColor(MAIN_COLOR);
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu[menu_item].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu[menu_item].text);	

			
		}
		
		if (up_key.button_short_is_pressed() || up_key.button_continious_is_pressed())
		{
			menu_item++;
			if (menu_item >= current_menu->prev_menu->item_count) menu_item = 0;
			//pxs.clear();
			pxs.setColor(BG_COLOR);
			pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, pic_width, pic_height);
			
			//pxs.clear();
			pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu[menu_item].icon);
			pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu[menu_item].icon);
			pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu[menu_item ? (menu_item - 1) : current_menu->prev_menu->item_count - 1].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu[menu_item ? (menu_item - 1) : current_menu->prev_menu->item_count - 1].text);
			pxs.setColor(MAIN_COLOR);
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu[menu_item].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu[menu_item].text);	

		}
		
		if (down_key.button_short_is_pressed() || down_key.button_continious_is_pressed())
		{
			menu_item--;
			if (menu_item == 255) menu_item = current_menu->prev_menu->item_count - 1;
			//pxs.clear();
			pxs.setColor(BG_COLOR);
			pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, pic_width, pic_height);
			
			//pxs.clear();
			pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu[menu_item].icon);
			pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu[menu_item].icon);
			pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu[menu_item == (current_menu->prev_menu->item_count - 1) ? 0 : (menu_item + 1)].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu[menu_item == (current_menu->prev_menu->item_count - 1) ? 0 : (menu_item + 1)].text);
			pxs.setColor(MAIN_COLOR);
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu[menu_item].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu[menu_item].text);

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