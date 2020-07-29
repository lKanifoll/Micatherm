#pragma once

#include "Pixels.h"

#define DISPLAY_HEIGHT 161
#define DISPLAY_WIDTH  130
#define DX0 2
#define DY0 1

#define BG_COLOR	RGB(0x00, 0x00, 0x00)
#define MAIN_COLOR	RGB(0xFF, 0xFF, 0xFF)
#define Green_COLOR	RGB(0x00, 0xFF, 0x00)


void draw_main_menues();
void draw_submenus();
void enter_confirm();
void menu_back();
void next_menu_param();
void prev_menu_param();

void inc_temp();
void dec_temp();
void confirm_params();
void DrawTextSelected(int16_t x, int16_t y, char* text, bool selected, bool underline, int16_t oX, int16_t oY);
void DrawTextAligment(int16_t x, int16_t y, int16_t w, int16_t h, char* text, bool selected, bool underline, uint8_t border, RGB fore, RGB back);

