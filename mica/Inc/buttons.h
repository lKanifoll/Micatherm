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
	void check_button_state();
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
	uint8_t clicks;
	uint16_t continious_press_time;
	uint16_t long_press_time;
	uint16_t click_count;
	uint32_t pressed_time;
	uint32_t button_pin;
};


