#include "main.h"
#include "buttons.h"

button ::button(GPIO_TypeDef *_button_port, uint32_t _button_pin, uint16_t _long_press_time)
{
	button_port = _button_port;
	button_pin = _button_pin;
	button_state = 0;
	pressed_time = 0;
	short_press_time = 10;
	continious_press_time = 100;
	long_press_time = _long_press_time;
	button_short_pressed = 0;
	button_continious_pressed = 0;
	button_long_pressed = 0;
	button_last_state = 0;
	click_count = 0;
}
void button::init()
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = button_pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(button_port, &GPIO_InitStruct);
}



bool button::button_short_is_pressed()
{
	if (button_short_pressed) 
	{
		button_short_pressed = false;
		return true;
	}
	return false;		
}
	
bool button::button_continious_is_pressed()
{
	if (button_continious_pressed) 
	{
		button_continious_pressed = false;
		return true;
	}
	return false;		
}
	
bool button::button_long_is_pressed()
{
	if (button_long_pressed) 
	{
		button_long_pressed = false;
		return true;
	}
	return false;		
}	