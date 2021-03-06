#pragma once

#include "Pixels.h"

#define DISPLAY_HEIGHT 161
#define DISPLAY_WIDTH  130
#define DX0 0
#define DY0 0

// Display has strange color order: RBG & BRG
// In Pixels.cpp conversion RGB: changed green<->blue

#define BG_COLOR	 RGB(0x00, 0x00, 0x00)
#define MAIN_COLOR	 RGB(0xFF, 0xFF, 0xFF)
#define Green_COLOR	 RGB(0x00, 0xFF, 0x00)
#define Orange_COLOR RGB(247, 147, 29)
#define Grey_COLOR   RGB(112, 111, 111)

void draw_main_menues();
void draw_submenus();
void enter_confirm();
void menu_back();
void next_menu_param();
void prev_menu_param();

void inc_temp();
void dec_temp();
void set_on();
void set_off();
void confirm_params();
void draw_arrows();
void draw_main_screen();
void DrawTextSelected(int16_t x, int16_t y, char* text, bool selected, bool underline, int16_t oX, int16_t oY);
void DrawTextAligment(int16_t x, int16_t y, int16_t w, int16_t h, char* text, bool selected, bool underline, uint8_t border, RGB fore, RGB back);

