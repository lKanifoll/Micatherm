#pragma once

#include "images.h"
#include "fonts.h"


typedef enum
{
	enter = 1,
	back,
	up,
	down
} button_state;
/*
typedef struct menu_item
{
	uint16_t ID;
	uint8_t item_count;
	prog_uchar *icon;
	char text[20];
	menu_item *menu_items;
	menu_item *prev_menu;
	uint8_t selected_menu;
	void(*draw_edit_menu)();

} menu_item_t;
*/
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
	{ 10, 1, NULL },
	{ 11, 1, NULL },
	{ 12, 1, NULL }
};

static menu_item_t timer_menu[] = {
	{ 20, 2, NULL },
	{ 21, 2, NULL }
};

static menu_item_t service_menu[] = {
	{ 330, 0, NULL },
	{ 331, 0, NULL }	
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
	{ 1, 3, heatmode_menu },
	{ 2, 2, timer_menu },
	{ 3, 4, settings_menu },
	{ 4, 3, programme_menu }
};
static menu_item_t menu[] = { 
	{ 0, 4, main_menu, NULL }
};





void screen_smooth_transition();
uint32_t get_raw_adc_meas();