

#pragma once
#include "draw_functions.h"


#include "fonts.h"
#include "images.h"

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
	uint8_t comfort_temp;
	uint8_t econom_temp;
	uint8_t antifrost_temp;
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
	{ 11, 0, NULL }, // eco
	{ 12, 0, NULL } // anti
};

static menu_item_t timer_menu[] = {
	{ 20, 2, NULL }, //set
	{ 21, 2, NULL } // on\off
};

static menu_item_t service_menu[] = {
	{ 330, 0, NULL }, //reset
	{ 331, 0, NULL }  // info
};

static menu_item_t settings_menu[] = {
	{ 30, 2, NULL },
	{ 31, 2, NULL },
	{ 32, 0, NULL },
	{ 33, 2, service_menu }
};

static menu_item_t programme_menu[] = {
	
	{ 40, 0, NULL },
	{ 41, 0, NULL },
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





void screen_smooth_transition();
uint32_t get_raw_adc_meas();