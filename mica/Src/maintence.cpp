
#include "maintence.h"
#include "buttons.h"
#include "ntc_steinhart.h"

extern ADC_HandleTypeDef hadc1;
extern IWDG_HandleTypeDef hiwdg;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim4;
extern uint8_t complete_dma;
extern osMessageQueueId_t button_Queue;
extern temper_t temp_tmp;
extern on_off_t on_off_tmp;
extern menu_item_t *current_menu;
extern settings_t device_config;


button enter_key(key2_GPIO_Port, key2_Pin);
button back_key(key3_GPIO_Port, key3_Pin);
button down_key(key4_GPIO_Port, key4_Pin);
button up_key(key0_GPIO_Port, key0_Pin);
button power_key(key1_GPIO_Port, key1_Pin);
button window_key(key5_GPIO_Port, key5_Pin);

const uint32_t FlashSize = 512 * 1024;
const uint32_t InfoSize = 2 * 1024;


void prepare_settings(menu_item_t *current_menu)
{
	switch (current_menu->ID)
	{
	case 10:
		temp_tmp.new_temp_p = device_config.comfort_temp;
		temp_tmp.old_temp_p = device_config.comfort_temp;
		break;
	case 11:
		temp_tmp.new_temp_p = device_config.econom_temp;
		temp_tmp.old_temp_p = device_config.econom_temp;
		break;
	case 12:
		temp_tmp.new_temp_p = device_config.antifrost_temp;
		temp_tmp.old_temp_p = device_config.antifrost_temp;
		break;
	case 20:
		on_off_tmp.new_p = device_config.timer_on_off;
		on_off_tmp.old_p = device_config.timer_on_off;
		break;
	case 41:
		on_off_tmp.new_p = device_config.calendar_on_off;
		on_off_tmp.old_p = device_config.calendar_on_off;
		break;
	case 32:
		on_off_tmp.new_p = device_config.buzzer_on_off;
		on_off_tmp.old_p = device_config.buzzer_on_off;
		break;
	case 310:
		on_off_tmp.new_p = device_config.brightness;
		on_off_tmp.old_p = device_config.brightness;
		break;
	case 311:
		on_off_tmp.new_p = device_config.auto_off_bkl;
		on_off_tmp.old_p = device_config.auto_off_bkl;
		break;
	}
	
}

void accept_settings(menu_item_t *current_menu)
{
	switch (current_menu->ID)
	{
	case 10:
		device_config.comfort_temp = temp_tmp.new_temp_p;
		break;
	case 11:
		device_config.econom_temp = temp_tmp.new_temp_p;
		break;
	case 12:
		device_config.antifrost_temp = temp_tmp.new_temp_p;
		break;
	case 20:
		device_config.timer_on_off = on_off_tmp.new_p;
		break;
	case 41:
		device_config.calendar_on_off = on_off_tmp.new_p;
		break;
	case 32:
		device_config.buzzer_on_off = on_off_tmp.new_p;
		break;
	case 310:
		device_config.brightness = on_off_tmp.new_p;
		break;
	case 311:
		device_config.auto_off_bkl = on_off_tmp.new_p;
		break;
	case 330:
		current_menu->selected_menu--;
		break;
	}	
}


void WriteSettings(void)
{
	uint32_t pageError, addr, *flashData;
	FLASH_EraseInitTypeDef FLASH_EraseInitStruct;
  
	//air_filter_settings.init_memory = INIT_MEMORY_KEY;
	//air_filter_settings.backup_filter_count = filter_alarm_count;
	addr = (0x08000000 + FlashSize - InfoSize);
	FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	FLASH_EraseInitStruct.Banks = FLASH_BANK_1;
	FLASH_EraseInitStruct.PageAddress = addr;
	FLASH_EraseInitStruct.NbPages = 1;
  
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &pageError);
	//flashData = (uint32_t*)&air_filter_settings;
	for(uint32_t i = 0 ; i < sizeof(settings_t) / 4 ; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i * 4, flashData[i]);
	}
	HAL_FLASH_Lock();
}


/* Callback for check button state*/
void button_timer_callback(void *argument)
{
	enter_key.check_button_state();	
	back_key.check_button_state();
	up_key.check_button_state();	
	down_key.check_button_state();	
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

void screen_smooth_transition(uint8_t on_off)
{
	if (on_off)
	{
		while (TIM4->CCR3 < 65000)
		{
			TIM4->CCR3 += 750;
			osDelay(2);
		}
	}
	else
	{
		while (TIM4->CCR3 > 1000)
		{
			TIM4->CCR3 -= 5000;
			osDelay(1);
		}
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

