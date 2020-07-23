#pragma once
//#include "maintence.h"
//#include "stdint.h"
#include "Pixels.h"
#include <Pixels_PPI8.h> 
#include <Pixels_ST7735.h>
#include "fonts.h"
#define DISPLAY_HEIGHT 161
#define DISPLAY_WIDTH  130
#define DX0 2
#define DY0 1

#define BG_COLOR	RGB(0x00, 0x00, 0x00)
#define MAIN_COLOR	RGB(0xFF, 0xFF, 0xFF)
#define Green_COLOR	RGB(0x00, 0xFF, 0x00)




Pixels pxs(DISPLAY_WIDTH, DISPLAY_HEIGHT);

void draw_main_menues();
void enter_confirm();
void next_menu_param();
void prev_menu_param();

void DrawTextSelected(int16_t x, int16_t y, char* text, bool selected, bool underline, int16_t oX, int16_t oY);
void DrawTextAligment(int16_t x, int16_t y, int16_t w, int16_t h, char* text, bool selected, bool underline, uint8_t border, RGB fore, RGB back);

