
#include "draw_functions.h"
#include "maintence.h"


menu_item_t *current_menu = NULL;
menu_item_t *tmp_current_menu = NULL;
uint8_t prev_current_pos = 0;

extern osMessageQueueId_t button_Queue;

void draw_set_timer()
{
	if (current_menu->selected_menu == 0)
	{
		pxs.cleanText(20, 50, "Bye");
		pxs.print(20, 50, "Hi");
	}
	else
	{
		pxs.cleanText(20, 50, "Hi");
		pxs.print(20, 50, "Bye");
	}
}
void graphic_task(void *argument)
{
	pxs.setOrientation(PORTRAIT);
	pxs.enableAntialiasing(true);
	pxs.init();
	pxs.setBackground(BG_COLOR);
	pxs.setColor(MAIN_COLOR); 
	pxs.clear();
	pxs.displayOn();
	pxs.setFont(ElectroluxSansRegular10a);
	TIM3->CCR1 = 65535;

	
	int16_t pic_width = 0;
	int16_t pic_height = 0;
	
	osStatus_t status;
	uint8_t button_status;
	for (;;)
	{
		status = osMessageQueueGet(button_Queue, &button_status, NULL, 0xFFFFFFFF);      
		
		
		switch(button_status)
		{
		case enter:
			pxs.clear();
			if (current_menu == NULL) 
			{
				current_menu = menu;
			}
			else
			{
				tmp_current_menu = current_menu;
				current_menu = &current_menu->next_menu[current_menu->selected_menu];
				current_menu->prev_menu = tmp_current_menu;
			}
			
			current_menu->selected_menu = 0;
			
			if (current_menu->ID == 7)
			{
				current_menu->draw_edit_menu();
			}
			else
			{
				pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.setColor(MAIN_COLOR);
				pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);	
			}
			break;
		case back:
			if (current_menu == NULL) 
			{
				continue;
			}
			else
			{
				tmp_current_menu = current_menu;
				current_menu = tmp_current_menu->prev_menu;
			}			
			
			pxs.clear();
			pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu->next_menu[current_menu->selected_menu].icon);
			pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu->next_menu[current_menu->selected_menu].icon);
			pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu ? (current_menu->selected_menu - 1) : 3].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu ? (current_menu->selected_menu - 1) : 3].text);
			pxs.setColor(MAIN_COLOR);
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);	
			break;
		case up:
			current_menu->selected_menu++;
			if (current_menu->selected_menu >= current_menu->item_count) current_menu->selected_menu = 0;

			
			if (current_menu->ID == 7)
			{
				current_menu->draw_edit_menu();
			}
			else
			{
				pxs.setColor(BG_COLOR);
				pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, pic_width, pic_height);

				pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu ? (current_menu->selected_menu - 1) : current_menu->item_count - 1].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu ? (current_menu->selected_menu - 1) : current_menu->item_count - 1].text);
				pxs.setColor(MAIN_COLOR);
				pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);	
			}
			break;
		case down:
			current_menu->selected_menu--;
			if (current_menu->selected_menu == 255) current_menu->selected_menu = current_menu->item_count - 1;
			
			if (current_menu->ID == 7)
			{
				current_menu->draw_edit_menu();
			}
			else
			{
				pxs.setColor(BG_COLOR);
				pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, pic_width, pic_height);

				pxs.sizeCompressedBitmap(pic_width, pic_height, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, current_menu->next_menu[current_menu->selected_menu].icon);
				pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu == (current_menu->item_count - 1) ? 0 : (current_menu->selected_menu + 1)].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu == (current_menu->item_count - 1) ? 0 : (current_menu->selected_menu + 1)].text);
				pxs.setColor(MAIN_COLOR);
				pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(current_menu->next_menu[current_menu->selected_menu].text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, current_menu->next_menu[current_menu->selected_menu].text);
			}
			break;
		}
	}
}




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
