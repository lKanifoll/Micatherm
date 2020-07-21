
#include "maintence.h"



extern ADC_HandleTypeDef hadc1;
extern IWDG_HandleTypeDef hiwdg;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;
extern osMessageQueueId_t button_Queue;


button enter_key(key2_GPIO_Port, key2_Pin);
button back_key(key3_GPIO_Port, key3_Pin);
button up_key(key4_GPIO_Port, key4_Pin);
button down_key(key5_GPIO_Port, key5_Pin);




menu_item_t *current_menu = NULL;
menu_item_t *tmp_current_menu = NULL;
uint8_t prev_current_pos = 0;





/* Callback for check button state*/
void button_timer_callback(void *argument)
{
	enter_key.check_button_state();	
	back_key.check_button_state();
	up_key.check_button_state();	
	down_key.check_button_state();	
}

void graphic_task(void *argument)
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
	
	osStatus_t status;
	uint8_t button_status;
	for (;;)
	{
		status = osMessageQueueGet(button_Queue, &button_status, NULL, 0xFFFFFFFF);     // wait for message
		switch(button_status)
		{
		case enter:
			pxs.clear();
			if (current_menu == NULL) 
			{
				current_menu = menu;
			}
			else
			{
				tmp_current_menu = current_menu;
				current_menu = &current_menu->next_menu[current_menu->selected_menu];
				current_menu->prev_menu = tmp_current_menu;
			}
			
			current_menu->selected_menu = 0;
			
			if (current_menu->ID == 7)
			{
				current_menu->draw_edit_menu();
			}
			else
			{
				pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);
				pxs.setColor(MAIN_COLOR);
				pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);	
			}
			break;
		case back:
			if (current_menu == NULL) 
			{
				continue;
			}
			else
			{
				tmp_current_menu = current_menu;
				current_menu = tmp_current_menu->prev_menu;
			}			
			
			pxs.clear();
			pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu->next_menu[current_menu->selected_menu].icon);
			pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu->next_menu[current_menu->selected_menu].icon);
			pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu ? (current_menu->selected_menu - 1) : 3].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu ? (current_menu->selected_menu - 1) : 3].text);
			pxs.setColor(MAIN_COLOR);
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);	
			break;
		case up:
			current_menu->selected_menu++;
			if (current_menu->selected_menu >= current_menu->item_count) current_menu->selected_menu = 0;

			
			if (current_menu->ID == 7)
			{
				current_menu->draw_edit_menu();
			}
			else
			{
				pxs.setColor(BG_COLOR);
				pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, pic_width, pic_height);

				pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu ? (current_menu->selected_menu - 1) : current_menu->item_count - 1].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu ? (current_menu->selected_menu - 1) : current_menu->item_count - 1].text);
				pxs.setColor(MAIN_COLOR);
				pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);	
			}
			break;
		case down:
			current_menu->selected_menu--;
			if (current_menu->selected_menu == 255) current_menu->selected_menu = current_menu->item_count - 1;
			
			if (current_menu->ID == 7)
			{
				current_menu->draw_edit_menu();
			}
			else
			{
				pxs.setColor(BG_COLOR);
				pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, pic_width, pic_height);

				pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu == (current_menu->item_count - 1) ? 0 : (current_menu->selected_menu + 1)].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu == (current_menu->item_count - 1) ? 0 : (current_menu->selected_menu + 1)].text);
				pxs.setColor(MAIN_COLOR);
				pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);
			}
			break;
		}
	}
}



void buttons_task(void *argument)
{
	uint8_t button_state = 0;
	for (;;)
	{
		
		if (enter_key.button_short_is_pressed())
		{
			button_state = enter;
			osMessageQueuePut(button_Queue, &button_state, 0U, 0U);		
		}		
		
		if (back_key.button_short_is_pressed())
		{
			button_state = back;
			osMessageQueuePut(button_Queue, &button_state, 0U, 0U);				
		}
		
		if (up_key.button_short_is_pressed() || up_key.button_continious_is_pressed())
		{
			button_state = up;
			osMessageQueuePut(button_Queue, &button_state, 0U, 0U);		
		}
		
		if (down_key.button_short_is_pressed() || down_key.button_continious_is_pressed())
		{
			button_state = down;
			osMessageQueuePut(button_Queue, &button_state, 0U, 0U);		
		}		
		osDelay(10);	
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	complete_dma = 0;
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

void draw_set_timer()
{
	if (current_menu->selected_menu == 0)
	{
		pxs.cleanText(20, 50, "Bye");
		pxs.print(20, 50, "Hi");
	}
	else
	{
		pxs.cleanText(20, 50, "Hi");
		pxs.print(20, 50, "Bye");
	}
}