
//#include "draw_functions.h"
#include "maintence.h"
#include "rtc_ts.h"
#include <Pixels_PPI8.h> 
#include <Pixels_ST7735.h>


Pixels pxs(DISPLAY_WIDTH, DISPLAY_HEIGHT);

uint8_t prev_current_pos = 0;

extern osMessageQueueId_t button_Queue;
int16_t pic_width = 0;
int16_t pic_height = 0;
extern TIM_HandleTypeDef htim4;
extern RTC_HandleTypeDef hrtc;
on_off_t on_off_tmp = { 0, 0 };
temper_t temp_tmp = { 0, 0 };
menu_item_t *current_menu = NULL;
menu_item_t *tmp_current_menu = NULL;
settings_t device_config;

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

	//uint32_t time_rtc_ts = 0;
	//time_rtc_ts = RTC_timestamp_get(6, 7, 2020, 12, 32, 50);
	
	//RTC_SetTimestamp(&hrtc, time_rtc_ts);
	//RTC_SetTimestampAlarm(&hrtc, time_rtc_ts+10);
	//time_rtc_ts = 0;
	//time_rtc_ts = RTC_GetTimestamp(&hrtc, time_rtc_ts);
	
    draw_main_screen();
	screen_smooth_transition(1);
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
		if (current_menu->item_count)
		{
			current_menu->selected_menu = 0;
		}
		prepare_settings(current_menu);
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
	if (current_menu != NULL)
	{
		pxs.clear();
		if (current_menu->prev_menu == NULL) 
		{
			current_menu = NULL;
			draw_main_screen();
		}
		else
		{
			tmp_current_menu = current_menu;
			current_menu = tmp_current_menu->prev_menu;		
			draw_main_menues();
		}	
	}
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
		temp_tmp.new_temp_p++;
		temp_tmp.new_temp_p == 36 ? temp_tmp.new_temp_p = 10 : temp_tmp.new_temp_p;
		break;
	case 11:
		temp_tmp.new_temp_p++;
		temp_tmp.new_temp_p == 8 ? temp_tmp.new_temp_p = 3 : temp_tmp.new_temp_p;
		break;
	case 12:
		temp_tmp.new_temp_p++;
		temp_tmp.new_temp_p == 8 ? temp_tmp.new_temp_p = 3 : temp_tmp.new_temp_p;
		break;
	}
	
	draw_submenus();
}

void dec_temp()
{
	switch (current_menu->ID)
	{
	case 10:
		temp_tmp.new_temp_p--;
		temp_tmp.new_temp_p == 9 ? temp_tmp.new_temp_p = 35 : temp_tmp.new_temp_p;
		break;
	case 11:
		temp_tmp.new_temp_p--;
		temp_tmp.new_temp_p == 2 ? temp_tmp.new_temp_p = 7 : temp_tmp.new_temp_p;
		break;
	case 12:
		temp_tmp.new_temp_p--;
		temp_tmp.new_temp_p == 2 ? temp_tmp.new_temp_p = 7 : temp_tmp.new_temp_p;
		break;
	}
	draw_submenus();
}

void set_on()
{
	on_off_tmp.new_p = true;
	draw_submenus();
}

void set_off()
{
	on_off_tmp.new_p = false;
	draw_submenus();
}

void confirm_params()
{
	pxs.clear();
	pxs.sizeCompressedBitmap(pic_width, pic_height, img_ok_png_comp);
	pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2), img_ok_png_comp);
	osDelay(1000);
	pxs.setColor(BG_COLOR);
	pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2), pic_width, pic_height);
	pxs.setColor(MAIN_COLOR);
	
	if (current_menu->selected_menu)
	{
		switch (current_menu->ID)
		{
		case 330:
			if (on_off_tmp.new_p)
			{
				accept_settings(current_menu);
				draw_submenus();
			}
			else
			{
				menu_back();
			}
			break;
		}
	}
	else
	{
		accept_settings(current_menu);
		menu_back();
	}
}


void draw_main_menues()
{
	screen_smooth_transition(0);
	pxs.setFont(ElectroluxSansRegular10a);
	
	if (current_menu->menu_items == NULL)
	{
		pxs.clear();
		draw_submenus();
	}
	else
	{
		const uint8_t *icon = NULL;
		static char* text = NULL;
		draw_arrows();
		pxs.cleanText(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 25, text);

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
			text = device_config.timer_on_off ? (char*)"Timer is ON" : (char*)"Timer is OFF";
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
			text = (char*)"Information";
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
		pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2) - 1, DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, pic_width, pic_height);
		pxs.sizeCompressedBitmap(pic_width, pic_height, icon);
		pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2) - 1, DY0 + DISPLAY_HEIGHT / 2 - (pic_height / 2) - 15, icon);
		pxs.setColor(MAIN_COLOR);
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(text) / 2), DY0 + DISPLAY_HEIGHT / 2 + 25, text);	
	}
	screen_smooth_transition(1);
}

void draw_submenus()
{
	static int16_t width_tmp;
	pxs.setFont(ElectroluxSansRegular14a);
	switch (current_menu->ID)
	{
	case 10:
	case 11:
	case 12:
		static char tmp_t[2];
		static char celsius[2] = { '\xB0', '\x43' };
		draw_arrows();
		pxs.setFont(ElectroluxSansLight41a);
		pxs.setColor(BG_COLOR);
		pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(tmp_t) / 2) - 15, DY0 + DISPLAY_HEIGHT / 2 - 20, pxs.getTextWidth(tmp_t)+30, pxs.getTextLineHeight());
		pxs.setColor(MAIN_COLOR);
		
		
		sprintf(tmp_t, "%d", temp_tmp.new_temp_p);
		width_tmp = pxs.getTextWidth(tmp_t);
		pxs.setColor(MAIN_COLOR);
		
		pxs.print((DX0 + DISPLAY_WIDTH / 2) - (width_tmp / 2) - 15, DY0 + DISPLAY_HEIGHT / 2 - 20, tmp_t);	
		
		pxs.setFont(ElectroluxSansLight16a);
		pxs.print((DX0 + (DISPLAY_WIDTH / 2)) + width_tmp / 2 - 15, DY0 + DISPLAY_HEIGHT / 2 - 20, celsius);	
		break;
	case 41:
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth((char*)"Programme") / 2), 10, (char*)"Programme");	
	case 32:
	case 20:
	case 311:
		DrawTextSelected(10, DY0 + DISPLAY_HEIGHT / 2 - pxs.getTextLineHeight() / 2, (char*)"ON", on_off_tmp.new_p, on_off_tmp.old_p, 5, 15);
		DrawTextSelected(80, DY0 + DISPLAY_HEIGHT / 2 - pxs.getTextLineHeight() / 2, (char*)"OFF", !on_off_tmp.new_p, !on_off_tmp.old_p, 5, 15);
		break;
	case 310:
		DrawTextSelected(8, DY0 + DISPLAY_HEIGHT / 2 - pxs.getTextLineHeight() / 2, (char*)"50%", on_off_tmp.new_p, on_off_tmp.old_p, 5, 15);
		DrawTextSelected(65, DY0 + DISPLAY_HEIGHT / 2 - pxs.getTextLineHeight() / 2, (char*)"100%", !on_off_tmp.new_p, !on_off_tmp.old_p, 5, 15);
		break;
	case 330:
		pxs.setColor(MAIN_COLOR);
		pxs.setFont(ElectroluxSansRegular10a); 
		if (current_menu->selected_menu)
		{
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth((char*)"Reset all device") / 2), 10, (char*)"Reset all device");
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth((char*)"settings?") / 2), 30, (char*)"settings?");
			DrawTextSelected(18, DY0 + DISPLAY_HEIGHT / 2 - pxs.getTextLineHeight() / 2 + 25, (char*)"Yes", on_off_tmp.new_p, NULL, 10, 15);
			DrawTextSelected(88, DY0 + DISPLAY_HEIGHT / 2 - pxs.getTextLineHeight() / 2 + 25, (char*)"No", !on_off_tmp.new_p, NULL, 10, 15);		
		}
		else
		{
			pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth((char*)"Are you sure?") / 2), 10, (char*)"Are you sure?");
			DrawTextSelected(18, DY0 + DISPLAY_HEIGHT / 2 - pxs.getTextLineHeight() / 2 + 25, (char*)"Yes", on_off_tmp.new_p, NULL, 10, 15);
			DrawTextSelected(88, DY0 + DISPLAY_HEIGHT / 2 - pxs.getTextLineHeight() / 2 + 25, (char*)"No", !on_off_tmp.new_p, NULL, 10, 15);	
		}
		break;
	case 331:
		pxs.setFont(ElectroluxSansRegular10a); 
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth((char*)"Current") / 2), 50, (char*)"Current");
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth((char*)"firmware:") / 2), 70, (char*)"firmware:");
		pxs.print(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth((char*)"12.2.8") / 2), 90, (char*)"12.2.8");
		break;
	}
}


void draw_arrows()
{
	pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (28 / 2), 3, img_up_png_comp);
	pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (28 / 2), 142, img_down_png_comp);
}

void draw_main_screen()
{
	screen_smooth_transition(0);
	
	static int16_t width_tmp;
	static char tmp_t[2];
	static char celsius[2] = { '\xB0', '\x43' };

	pxs.setColor(BG_COLOR);
	pxs.fillRectangle(DX0 + DISPLAY_WIDTH / 2 - (pxs.getTextWidth(tmp_t) / 2) - 15, DY0 + DISPLAY_HEIGHT / 2 - 26, pxs.getTextWidth(tmp_t), pxs.getTextLineHeight());
	
		
	pxs.setFont(ElectroluxSansLight41a);
	sprintf(tmp_t, "%d", 25);
	width_tmp = pxs.getTextWidth(tmp_t);
	pxs.setColor(MAIN_COLOR);
		
	pxs.print((DX0 + DISPLAY_WIDTH / 2) - (width_tmp / 2) - 15, DY0 + DISPLAY_HEIGHT / 2 - 26, tmp_t);	
	pxs.setFont(ElectroluxSansLight16a);
	pxs.print((DX0 + (DISPLAY_WIDTH / 2)) + width_tmp / 2 - 15, DY0 + DISPLAY_HEIGHT / 2 - 26, celsius);
	
	pxs.setColor(Orange_COLOR); 
	pxs.fillRectangle(5, 151, 59, 6);
	pxs.setColor(Grey_COLOR); 
	pxs.fillRectangle(65, 151, 59, 6);
	
	
	pxs.setColor(MAIN_COLOR);
	pxs.fillRectangle(5, 42, 120, 2);
	
	pxs.drawCompressedBitmap(5 , 5, img_comfort_act_png_comp);
	pxs.sizeCompressedBitmap(pic_width, pic_height, img_eco_deact_png_comp);
	pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), 5, img_eco_deact_png_comp);
	pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH - (pic_width) - 5, 5, img_anti_deact_png_comp); 
	
	pxs.fillRectangle(5, DY0 + DISPLAY_HEIGHT - 56, 120, 2);
	pxs.sizeCompressedBitmap(pic_width, pic_height, img_timer_deact_png_comp);
	pxs.drawCompressedBitmap(5, DY0 + DISPLAY_HEIGHT - (pic_height) - 17, img_window_act_png_comp);
	pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH / 2 - (pic_width / 2), DY0 + DISPLAY_HEIGHT - (pic_height) - 17, img_timer_deact_png_comp);
	pxs.drawCompressedBitmap(DX0 + DISPLAY_WIDTH - (pic_width) - 5, DY0 + DISPLAY_HEIGHT - (pic_height) - 17, img_shed_deact_png_comp);
	screen_smooth_transition(1);
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
