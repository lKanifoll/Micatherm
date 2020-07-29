
#include "maintence.h"

#include "buttons.h"
#include "ntc_steinhart.h"


extern ADC_HandleTypeDef hadc1;
extern IWDG_HandleTypeDef hiwdg;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;
extern uint8_t complete_dma;
extern osMessageQueueId_t button_Queue;

button enter_key(key2_GPIO_Port, key2_Pin);
button back_key(key3_GPIO_Port, key3_Pin);
button down_key(key4_GPIO_Port, key4_Pin);
button up_key(key5_GPIO_Port, key5_Pin);

const uint32_t FlashSize = 512 * 1024;
const uint32_t InfoSize = 2 * 1024;



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
	for(uint32_t i = 0 ; i < sizeof(settings) / 4 ; i++)
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

