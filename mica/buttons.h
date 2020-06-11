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
		short_press_time = 20;
		long_press_time = 1000;
		button_short_pressed = 0;
		button_long_pressed = 0;
		button_last_state = 0;
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
	
	void check_button_state()
	{
		button_state = LL_GPIO_IsInputPinSet(button_port, button_pin);
		
		if ((button_state != button_last_state) && button_state)
		{
			pressed_time = osKernelGetSysTimerCount();
		}
		
		if ((osKernelGetSysTimerCount() - pressed_time) == short_press_time && (!button_short_pressed) && button_state)
		{
			button_short_pressed = true;
		}
		
		if ((osKernelGetSysTimerCount() - pressed_time) > long_press_time && button_state) 
		{
			click_count++;
		}
		else
		{
			click_count = 0;
			button_long_pressed = false;
		}
		
		if (click_count == 400)
		{			
			button_long_pressed = true;
			click_count = 0;
		}
		
		button_last_state = button_state;
		/*
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
		*/
			
	}
	inline bool button_short_is_pressed()
	{
		if (button_short_pressed) 
		{
			button_short_pressed = false;
			return true;
		}
		return false;		
	}
	
	inline bool button_long_is_pressed()
	{
		if (button_long_pressed) 
		{
			button_long_pressed = false;
			return true;
		}
		return false;		
	}
private:
	GPIO_TypeDef* button_port;

	bool button_state;
	bool button_last_state;
	bool button_short_pressed;
	bool button_long_pressed;
	uint8_t short_press_time;
	uint16_t long_press_time;
	uint16_t click_count;
	uint32_t pressed_time;
	uint32_t button_pin;
};


