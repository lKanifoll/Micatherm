#pragma once
#include "maintence.h"

void DrawTextSelected(int16_t x, int16_t y, char* text, bool selected, bool underline, int16_t oX, int16_t oY);
void DrawTextAligment(int16_t x, int16_t y, int16_t w, int16_t h, char* text, bool selected, bool underline, uint8_t border, RGB fore, RGB back);