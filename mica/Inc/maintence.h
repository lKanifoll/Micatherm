

#pragma once
#include "draw_functions.h"
#include "fonts.h"
#include "images.h"




typedef struct
{
	uint8_t old_p;
	uint8_t new_p;
} on_off_t;

typedef struct
{
	uint8_t old_temp_p;
	uint8_t new_temp_p;
} temper_t;

typedef enum
{
	enter = 1,
	back,
	up,
	down
} button_state;

typedef struct 
{
	uint8_t on_off;
	uint8_t block;
	uint8_t calendar_on_off;
	uint8_t brightness;
	uint8_t auto_off_bkl;
	uint8_t buzzer_on_off;
	uint8_t heat_mode;
	uint8_t power_level;
	uint8_t working_mode;
	uint8_t open_window_on_off;
	uint8_t comfort_temp = 24;
	uint8_t econom_temp = 7;
	uint8_t antifrost_temp = 5;
	uint8_t timer_on_off;
	uint16_t timer;
	uint32_t crc;
	
}settings_t;

typedef struct menu_item
{
	uint16_t ID;
	uint8_t item_count;
	menu_item *menu_items;
	void(*confirm)();
	void(*forward)();
	void(*backward)();
	menu_item *prev_menu;
	uint8_t selected_menu;

} menu_item_t;






static menu_item_t heatmode_menu[] = { 
	{ 10, 0, NULL, confirm_params, inc_temp, dec_temp }, // comfort
	{ 11, 0, NULL, confirm_params, inc_temp, dec_temp }, // eco
	{ 12, 0, NULL, confirm_params, inc_temp, dec_temp } // anti
};

static menu_item_t timer_menu[] = {
	
	{ 20, 2, NULL, confirm_params, set_off, set_on} , // on\off
	{ 21, 2, NULL }  //set
};
static menu_item_t date_time[] = {
	{ 300, 0, NULL }, // date
	{ 301, 0, NULL }  // time
};

static menu_item_t display[] = {
	{ 310, 0, NULL, confirm_params, set_off, set_on }, 
	{ 311, 0, NULL, confirm_params, set_off, set_on }  
};

static menu_item_t service_menu[] = {
	{ 330, 0, NULL }, //reset
	{ 331, 0, NULL }  // info
};

static menu_item_t settings_menu[] = {
	{ 30, 2, date_time },
	{ 31, 2, display },
	{ 32, 0, NULL, confirm_params, set_off, set_on },
	{ 33, 2, service_menu }
};

static menu_item_t programme_menu[] = {
	
	{ 40, 0, NULL },
	{ 41, 0, NULL, confirm_params, set_off, set_on },
	{ 42, 0, NULL },
};

static menu_item_t main_menu[] = { 
	{ 1, 3, heatmode_menu, NULL, NULL, NULL },
	{ 2, 2, timer_menu, NULL, NULL, NULL },
	{ 3, 4, settings_menu, NULL, NULL, NULL },
	{ 4, 3, programme_menu, NULL, NULL, NULL }
};
static menu_item_t menu[] = { 
	{ 0, 4, main_menu, NULL, NULL, NULL, NULL }
};





void screen_smooth_transition(uint8_t on_off);
uint32_t get_raw_adc_meas();
void prepare_settings(menu_item_t *current_menu);
void accept_settings(menu_item_t *current_menu);