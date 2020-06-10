#pragma once

#include "main.h"

class button
{
public:
	button(GPIO_TypeDef *_button_port, uint32_t _button_pin)
	{
		button_port = _button_port;
		button_pin = _button_pin;
		button_state = 0;
		pressed_time = 0;
		short_press_time = 3;
		long_press_time = 2000;
		button_is_pressed = 0;
		button_last_state = 1;
		click_count = 0;
	}
	void init()
	{
		LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		GPIO_InitStruct.Pin = button_pin;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
		LL_GPIO_Init(button_port, &GPIO_InitStruct);
	}
	
	inline void check_button_state()
	{
		button_state = LL_GPIO_IsInputPinSet(button_port, button_pin);
		
		if (button_state != button_last_state)
		{
			pressed_time = HAL_GetTick();
			button_is_pressed = 0;
		}
		
		if ((HAL_GetTick() - pressed_time) > short_press_time && (button_state))
		{
			click_count++;			
		}
		else
		{
			click_count = 0;
			button_is_pressed = 0;
		}

		if (click_count == 15)
		{			
			button_is_pressed = true;
			click_count = 0;
			button_last_state = 0;
		}
		else
		{
			button_last_state = button_state;
		}
			
	}
	inline bool button_pressed()
	{
		return (bool)button_is_pressed;
	}
protected:
	GPIO_TypeDef* button_port;
	uint32_t button_pin;
	uint32_t button_state;
	uint32_t pressed_time;
	uint32_t short_press_time;
	uint32_t long_press_time;
	uint8_t button_is_pressed;
	uint8_t button_last_state;
	uint16_t click_count;
};


