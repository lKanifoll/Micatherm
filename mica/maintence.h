#pragma once


#include "main.h"
#include "Pixels.h"
#include <Pixels_PPI8.h> 

#include <Pixels_ST7735.h>
#include "images.h"
#include "fonts.h"
#include "ntc_steinhart.h"
#include "buttons.h"
#include "draw_functions.h"

#define DISPLAY_HEIGHT 161
#define DISPLAY_WIDTH  130
#define DX0 2
#define DY0 1

#define BG_COLOR	RGB(0x00, 0x00, 0x00)
#define MAIN_COLOR	RGB(0xFF, 0xFF, 0xFF)
#define Green_COLOR	RGB(0x00, 0xFF, 0x00)

Pixels pxs(DISPLAY_WIDTH, DISPLAY_HEIGHT);
void draw_set_timer();



typedef enum
{
	enter = 1,
	back,
	up,
	down
} button_state;


typedef struct menu_item
{
	uint16_t ID;
	uint8_t item_count;
	prog_uchar *icon;
	char text[20];
	menu_item *next_menu;
	menu_item *prev_menu;
	uint8_t selected_menu;
	void(*draw_edit_menu)();

} menu_item_t;

static menu_item_t heatmode_menu[] = { 
	{ 0, 1, img_menu_mode_comfort_png_comp, "Comfort" },
	{ 1, 1, img_menu_mode_eco_png_comp, "Eco" },
	{ 2, 1, img_menu_mode_anti_png_comp, "Anti-frost" }
};

static menu_item_t timer_menu[] = {
	{ 7, 2, img_menu_settimer_png_comp, "Set timer", NULL, NULL, 0, draw_set_timer }
};

static menu_item_t service_menu[] = {
	{ 0, 0, img_menu_setting_reset_png_comp, "Reset" },
	{ 1, 0, img_menu_setting_info_png_comp, "Info" }	
};

static menu_item_t settings_menu[] = {
	{ 0, 2, img_menu_setting_datetime_png_comp, "Date & time" },
	{ 1, 2, img_menu_display_png_comp, "Display" },
	{ 2, 0, img_menu_setting_sound_on_png_comp, "Sound" },
	{ 3, 2, img_menu_setting_service_png_comp, "Service", service_menu, NULL }
};

static menu_item_t programme_menu[] = {
	
	{ 0, 0, img_menu_program_setup_icon_png_comp, "Setup" },
	{ 1, 0, img_program_cal_on_icon_png_comp, "On" },
	{ 2, 0, img_menu_program_custom_png_comp, "Custom" },
};

static menu_item_t main_menu[] = { 
	{ 0, 3, img_menu_heatmode_icon_png_comp, "Heat mode", heatmode_menu, NULL },
	{ 1, 1, img_menu_timer_icon_png_comp, "Timer", timer_menu, NULL },
	{ 2, 4, img_menu_setting_icon_png_comp, "Settings", settings_menu, NULL },
	{ 3, 3, img_menu_program_icon_png_comp, "Programme", programme_menu, NULL }
};

static menu_item_t menu[] = { 
	{ 0, 4, NULL, "", main_menu, NULL }
};





void screen_smooth_transition();
uint32_t get_raw_adc_meas();