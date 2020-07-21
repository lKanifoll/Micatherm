
#include "maintence.h"


void DrawTextAligment(int16_t x, int16_t y, int16_t w, int16_t h, char* text, bool selected, bool underline, uint8_t border, RGB fore, RGB back)
{
	int16_t width = pxs.getTextWidth(text);
	int16_t height = pxs.getTextLineHeight();
	
	int16_t cX = x + w / 2 - width / 2;
	int16_t cY = y + h / 2 - height / 2;

	pxs.setColor(selected ? fore : back);
	pxs.fillRectangle(x, y, w, h);
	
	pxs.setColor(!selected ? fore : back);
	
	if (border > 0 && !selected)
	{
		for (int i = 0; i < border; i++)
			pxs.drawRectangle(x + i, y + i, w - i * 2, h - i * 2);
	}
	
	pxs.setBackground(selected ? fore : back);
	pxs.print(cX, cY, text);

	if (underline)
	{
		pxs.setColor(!selected ? fore : back);
		pxs.fillRectangle(cX, cY + height + 5, width, 3);
	}

	pxs.setBackground(BG_COLOR);
}


void DrawTextSelected(int16_t x, int16_t y, char* text, bool selected, bool underline = false, int16_t oX = 5, int16_t oY = 5)
{
	pxs.setColor(selected ? MAIN_COLOR : BG_COLOR);
	int16_t width = pxs.getTextWidth(text);
	int16_t height = pxs.getTextLineHeight();
	pxs.fillRectangle(x - oX, y - oY, width + oX * 2, height + oY * 2);
	pxs.setColor(!selected ? MAIN_COLOR : BG_COLOR);
	pxs.setBackground(selected ? MAIN_COLOR : BG_COLOR);
	pxs.print(x, y, text);
	
	if (underline)
	{
		pxs.setColor(!selected ? MAIN_COLOR : BG_COLOR);
		pxs.fillRectangle(x, y + height + 5, width, 4);
	}

	pxs.setBackground(BG_COLOR);
}