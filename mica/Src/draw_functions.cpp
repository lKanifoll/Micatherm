
//#include "draw_functions.h"
#include "maintence.h"
#include <Pixels_PPI8.h> 
#include <Pixels_ST7735.h>


Pixels pxs(DISPLAY_WIDTH, DISPLAY_HEIGHT);


menu_item_t *current_menu = NULL;
menu_item_t *tmp_current_menu = NULL;
uint8_t prev_current_pos = 0;

extern osMessageQueueId_t button_Queue;
int16_t pic_width = 0;
int16_t pic_height = 0;


extern TIM_HandleTypeDef htim4;




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
	//TIM4->CCR3 = 64535;

	
	osStatus_t status;
	uint8_t button_status;
	for (;;)
	{
		status = osMessageQueueGet(button_Queue, &button_status, NULL, 0xFFFFFFFF);      
		
		switch(button_status)
		{
		case enter:			
			enter_confirm();
			break;
		case back:
			menu_back();
			break;
		case up:
			next_menu_param();
			break;
		case down:
			prev_menu_param();
			break;			
		}
	}
}


void enter_confirm()
{

	if (current_menu == NULL) //to main
	{
		current_menu = menu;
		pxs.clear();
		current_menu->selected_menu = 0;
		draw_main_menues();
	}
	else if (current_menu->menu_items != NULL) // next to menu
	{
		tmp_current_menu = &current_menu->menu_items[current_menu->selected_menu];
		tmp_current_menu->prev_menu = current_menu;
		current_menu = tmp_current_menu;
		current_menu->selected_menu = 0;
		draw_main_menues();
	}
	else // ok or confirm
	{
		if (current_menu->confirm != NULL)
		{
			current_menu->confirm();
		}
	}
}

void menu_back()
{
	pxs.clear();
	if (current_menu->prev_menu == NULL) 
	{
		current_menu = NULL;
	}
	else
	{
		tmp_current_menu = current_menu;
		current_menu = tmp_current_menu->prev_menu;		
	}	
	draw_main_menues();
}

void next_menu_param()
{
	if (current_menu->forward != NULL)
	{
		current_menu->forward();
	}
	else
	{
		current_menu->selected_menu++;
		if (current_menu->selected_menu >= current_menu->item_count)
		{
			current_menu->selected_menu = 0;
		}
		draw_main_menues();
	}
}

void prev_menu_param()
{
	if (current_menu->forward != NULL)
	{
		current_menu->backward();
	}
	else
	{
		current_menu->selected_menu--;
		if (current_menu->selected_menu == 255) 
		{
			current_menu->selected_menu = current_menu->item_count - 1;
		}
		draw_main_menues();
	}
}

void inc_temp()
{
	switch (current_menu->ID)
	{
	case 10:
		device_config.comfort_temp++;
		break;
	case 11:
		device_config.econom_temp ++;
		break;
	case 12:
		device_config.antifrost_temp++;
		break;
	}
	
	draw_submenus();
}

void dec_temp()
{
	switch (current_menu->ID)
	{
	case 10:
		device_config.comfort_temp--;
		break;
	case 11:
		device_config.econom_temp--;
		break;
	case 12:
		device_config.antifrost_temp--;
		break;
	}
	draw_submenus();
}

void set_on()
{
	device_config.timer_on_off = true;
}

void set_off()
{
	device_config.timer_on_off = false;
}

void confirm_params()
{
	pxs.clear();
	pxs.sizeCompressedBitmap(pic_width, pic_height, img_ok_png_comp);
	pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2), img_ok_png_comp);
	osDelay(2000);
	menu_back();
}

void draw_main_menues()
{
	screen_smooth_transition(0);
	if (current_menu->menu_items == NULL)
	{
		pxs.clear();
		draw_submenus();
	}
	else
	{
		const uint8_t *icon = NULL;
		static char* text = NULL;
		
		pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, text);

		switch (current_menu->menu_items[current_menu->selected_menu].ID)
		{
		case 1:
			icon = img_menu_heatmode_icon_png_comp;
			text = (char*)"Heat mode";
			break;	
		case 10:
			icon = img_menu_mode_comfort_png_comp;
			text = (char*)"Comfort";
			break;	
		case 11:
			icon = img_menu_mode_eco_png_comp;
			text = (char*)"Eco";
			break;	
		case 12:
			icon = img_menu_mode_anti_png_comp;
			text = (char*)"Anti-frost";
			break;		
		case 2:
			icon = img_menu_timer_icon_png_comp;
			text = (char*)"Timer";
			break;
		case 20:
			icon = device_config.timer_on_off ? img_menu_timer_on_png_comp : img_menu_timer_off_png_comp;
			text = device_config.timer_on_off ? (char*)"Timer ON" : (char*)"Timer OFF";
			break;	
		case 21:
			icon = img_menu_settimer_png_comp;
			text = (char*)"Set timer";
			break;			
		case 3:
			icon = img_menu_setting_icon_png_comp;
			text = (char*)"Settings";
			break;
		case 30:
			icon = img_menu_setting_datetime_png_comp;
			text = (char*)"Date & time";
			break;	
		case 31:
			icon = img_menu_display_png_comp;
			text = (char*)"Display";
			break;		
		case 32:
			icon = device_config.buzzer_on_off ? img_menu_setting_sound_on_png_comp : img_menu_setting_sound_off_png_comp;
			text = (char*)"Sound";
			break;	
		case 33:
			icon = img_menu_setting_service_png_comp;
			text = (char*)"Service";
			break;
		case 300:
			icon = img_menu_program_icon_png_comp;
			text = (char*)"Set date";
			break;		
		case 301:
			icon = img_menu_settime_png_comp;
			text = (char*)"Set time";
			break;		
		case 310:
			icon = img_menu_display_bri_png_comp;
			text = (char*)"Brightness";
			break;		
		case 311:
			icon = img_menu_display_auto_png_comp;
			text = (char*)"Auto switch off";
			break;			
		case 330:
			icon = img_menu_setting_reset_png_comp;
			text = (char*)"Reset";
			break;		
		case 331:
			icon = img_menu_setting_info_png_comp;
			text = (char*)"Info";
			break;			
		case 4:
			icon = img_menu_program_icon_png_comp;
			text = (char*)"Programme";
			break;	
		case 40:
			icon = img_menu_program_setup_icon_png_comp;
			text = (char*)"Setup";
			break;		
		case 41:
			icon = device_config.calendar_on_off ? img_program_cal_on_icon_png_comp : img_program_cal_off_icon_png_comp;
			text = device_config.calendar_on_off ? (char*)"On" : (char*)"Off";
			break;	
		case 42:
			icon = img_menu_program_custom_png_comp;
			text = (char*)"Custom";
			break;			
		}
		pxs.setColor(BG_COLOR);
		pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, pic_width, pic_height);
		pxs.sizeCompressedBitmap(pic_width, pic_height, icon);
		pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, icon);
		pxs.setColor(MAIN_COLOR);
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, text);	
	}
	screen_smooth_transition(1);
}
char temp[3];
char temp1[3];
char temp2[3];
void draw_submenus()
{
	switch (current_menu->ID)
	{
	case 10:
		pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(temp) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, temp);
		sprintf(temp, "%d", device_config.comfort_temp);
		
		pxs.setColor(MAIN_COLOR);
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(temp) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, temp);	
		break;	
	case 11:
		pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(temp1) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, temp1);
		sprintf(temp1, "%d", device_config.econom_temp );
		
		pxs.setColor(MAIN_COLOR);
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(temp1) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, temp1);	
		break;	
	case 12:
		pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(temp2) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, temp2);
		sprintf(temp2, "%d", device_config.antifrost_temp );
		
		pxs.setColor(MAIN_COLOR);
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(temp2) / 2), DY0 + DISPLAY_HEIGHT / 2 + 30, temp2);	
		break;	
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
