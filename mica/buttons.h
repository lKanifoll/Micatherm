

#pragma once


/*
 *  Class button 
 **/

class button
{
public:
/*
 *  Init button hardware port, pin and timeout for long press button
 **/	
	button(GPIO_TypeDef *_button_port, uint32_t _button_pin, uint16_t _long_press_time = 2000);
/*
 *	Set hardware pin to be button
 **/
	void init();
/*
 *  Put this method in 1ms timer callback for button maintance
 **/
	inline void check_button_state()
	{		
		static uint8_t clicks = 0;
		button_state = LL_GPIO_IsInputPinSet(button_port, button_pin);
		
		if ((button_state != button_last_state) && button_state)
		{
			pressed_time = osKernelGetTickCount();
		}
		/*short press button*/
		if (((osKernelGetTickCount() - pressed_time) == short_press_time) && (!button_short_pressed) && button_state)
		{
			button_short_pressed = true;
		}

		/*hold button*/
		if ((osKernelGetTickCount() - pressed_time) > continious_press_time && button_state) 
		{
			click_count++;
		}
		else
		{
			clicks = 0;
			click_count = 0;
			button_continious_pressed = false;
		}
		
		if ((click_count == 300) && (clicks < 6))
		{			
			button_continious_pressed = true;
			click_count = 0;
			clicks++;
		}
		if ((click_count == 120) && (clicks > 5))
		{			
			button_continious_pressed = true;
			click_count = 0;
			
		}
		
		/*long pressed button*/
		if (((osKernelGetTickCount() - pressed_time) == long_press_time) && (!button_long_pressed) && button_state)
		{
			button_long_pressed = true;
		}		
		
		button_last_state = button_state;
			
	}
/*
 *  Call this method for check short press the button
 **/
	bool button_short_is_pressed();
/*
 *  Call this method for check continious pressing the button
 **/	
	bool button_continious_is_pressed();
/*
 *  Call this method for check long press the button
 **/	
	bool button_long_is_pressed();
private:
	GPIO_TypeDef* button_port;

	bool button_state;
	bool button_last_state;
	bool button_short_pressed;
	bool button_long_pressed;
	bool button_continious_pressed;
	uint8_t short_press_time;
	uint16_t continious_press_time;
	uint16_t long_press_time;
	uint16_t click_count;
	uint32_t pressed_time;
	uint32_t button_pin;
};


