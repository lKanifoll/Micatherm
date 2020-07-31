/*
 * Pixels. Graphics library for TFT displays.
 *
 * Copyright (C) 2012-2013  Igor Repinetski
 *
 * The code is written in C/C++ for Arduino and can be easily ported to any microcontroller by rewritting the low level pin access functions.
 *
 * Text output methods of the library rely on Pixelmeister's font data format. See: http://pd4ml.com/pixelmeister
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/
 *
 * This library includes some code portions and algoritmic ideas derived from works of
 * - Andreas Schiffler -- aschiffler at ferzkopp dot net (SDL_gfx Project)
 * - K. Townsend http://microBuilder.eu (lpc1343codebase Project)
 */

/*
 * Pixels port to ILI9341 controller, SPI mode (ElecFreaks TFT2.2SP Shield)
 * SPI is in bit banging mode, as the shield does not connect the hardware SPI (SCL=13, SDA=11)
 * to the display controller
 */

#include "Pixels.h"

#ifndef PIXELS_ST7735_H
#define PIXELS_ST7735_H
#define PIXELS_MAIN

#if defined(PIXELS_ANTIALIASING_H)
#define PixelsBase PixelsAntialiased
#endif
extern uint8_t complete_dma;
class Pixels : public PixelsBase
#if defined(PIXELS_SPISW_H)
                                    , public SPIsw
#elif defined(PIXELS_SPIHW_H)
                                    , public SPIhw
#elif defined(PIXELS_PPI8_H)
                                    , public PPI8
#elif defined(PIXELS_PPI16_H)
                                    , public PPI16
#endif
{
protected:
    void deviceWriteData(uint8_t high, uint8_t low);

    void setRegion(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void quickFill(int b, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void setFillDirection(uint8_t direction);
	void *memset16safe(void *m, uint16_t val, size_t count);
    void scrollCmd();

public:
    Pixels(uint16_t width, uint16_t height) : PixelsBase(width, height) { // ElecFreaks TFT1.8SP shield pins
        scrollSupported = true;
    }

    Pixels() : PixelsBase(128, 160) { // ElecFreaks TFT1.8SP shield as default
        scrollSupported = true;
    }

    void init();

		void displayOn()
		{
			writeCmd(0x29);    //Display on
			LL_GPIO_SetOutputPin(LCD_BL_GPIO_Port, LCD_BL_Pin);
		}
		
		void displayOff()
		{
			writeCmd(0x28);    //Display off
			LL_GPIO_ResetOutputPin(LCD_BL_GPIO_Port, LCD_BL_Pin);
		}
};

#if defined(PIXELS_ANTIALIASING_H)
#undef PixelsBase
#endif

void Pixels::init() {

    //initInterface();
    reset();
	chipDeselect();
    chipSelect();
    writeCmd(0x11);
	chipDeselect();
    HAL_Delay(120);

	chipSelect();
    writeCmd(0xB1);
    writeData(0x05);
    writeData(0x3C);
    writeData(0x3C);
	
    writeCmd(0xB2);
	writeData(0x05);
	writeData(0x3C);
	writeData(0x3C);
	
    writeCmd(0xB3);
	writeData(0x05);
	writeData(0x3C);
	writeData(0x3C);
	writeData(0x05);
	writeData(0x3C);
	writeData(0x3C);

    writeCmd(0xB4);
    writeData(0x03);

    writeCmd(0xC0);
    writeData(0x28);
    writeData(0x08);
    writeData(0x04);
	
    writeCmd(0xC1);
    writeData(0xC0);
	
    writeCmd(0xC2);
    writeData(0x0D);
    writeData(0x00);
	
    writeCmd(0xC3);
    writeData(0x8D);
    writeData(0x2A);
	
    writeCmd(0xC4);
    writeData(0x8D);
    writeData(0xEE);

    writeCmd(0xC5);
    writeData(0x18);
	
	writeCmd(0x36);
	writeData(0xC0);    // 0xC8 - BGR	
	/*

	
	writeCmd(0x2a);
	writeData(0x00);
	writeData(0x02);
	writeData(0x00);
	writeData(0x80);
	writeCmd(0x2b);
	writeData(0x00);
	writeData(0x01);
	writeData(0x00);
	writeData(0xA0);
	*/
    writeCmd(0xE0);     
	writeData(0x04);   
	writeData(0x1B);   
	writeData(0x1C);   
	writeData(0x1E);   
	writeData(0x3C);   
	writeData(0x33);
	writeData(0x29); 
	writeData(0x2B);   
	writeData(0x2A);   
	writeData(0x26);   
	writeData(0x2E);   
	writeData(0x39);   
	writeData(0x00);   
	writeData(0x02);   
	writeData(0x02);   
	writeData(0x10);   

	writeCmd(0xE1);     
	writeData(0x04);   
	writeData(0x1B);   
	writeData(0x1C);   
	writeData(0x1E);  
	writeData(0x3C);   
	writeData(0x33);  
	writeData(0x29);  
	writeData(0x2B);   
	writeData(0x29);   
	writeData(0x26);   
	writeData(0x2E);   
	writeData(0x39);   
	writeData(0x00);   
	writeData(0x02);   
	writeData(0x02);   
	writeData(0x10);   
/*
    writeCmd(0xe0);
    writeData(0x0f);
    writeData(0x1a);
    writeData(0x0f);
    writeData(0x18);
    writeData(0x2f);
    writeData(0x28);
    writeData(0x20);
    writeData(0x22);
    writeData(0x1f);
    writeData(0x1b);
    writeData(0x23);
    writeData(0x37);
    writeData(0x00);
    writeData(0x07);
    writeData(0x02);
    writeData(0x10);
	
    writeCmd(0xe1);
    writeData(0x0f);
    writeData(0x1b);
    writeData(0x0f);
    writeData(0x17);
    writeData(0x33);
    writeData(0x2c);
    writeData(0x29);
    writeData(0x2e);
    writeData(0x30);
    writeData(0x30);
    writeData(0x39);
    writeData(0x3f);
    writeData(0x00);
    writeData(0x07);
    writeData(0x03);
    writeData(0x10);
*/
	writeCmd(0x3A);
	writeData(0x05);
	//writeCmd(0x38);
//writeCmd(0x2a);
    //writeData(0x00);
    //writeData(0x00);
    //writeData(0x00);
    //writeData(0x7f);
    //writeCmd(0x2b);
    //writeData(0x00);
    //writeData(0x00);
    //writeData(0x00);
    //writeData(0x9f);

    //writeCmd(0xF0);
    //writeData(0x01);
    //writeCmd(0xF6);
    //writeData(0x00);
    writeCmd(0x11);       // Sleep out
    HAL_Delay(120);
    writeCmd(0x29);
	


	
    chipDeselect();
	
	
}

void Pixels::scrollCmd() {
    chipSelect();
    // the feature seems to be undocumented in the datasheet
    writeCmd(0x37);
    writeData(highByte(deviceHeight - currentScroll));
    writeData(lowByte(deviceHeight - currentScroll));
    chipDeselect();
}

void Pixels::setFillDirection(uint8_t direction) {
    fillDirection = direction;
}

void Pixels::quickFill (int color, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    chipSelect();

    setRegion(x1, y1, x2, y2);
	uint32_t counter = (uint32_t)(x2 - x1 + 1) * (y2 - y1 + 1);
	//chipDeselect();
	//HAL_Delay(10);
	//chipSelect();
    registerSelect();

	/*
	uint8_t* color_set_ptr = new uint8_t[counter];
	
	for (uint32_t i = 0; i < (counter); i+=2)
	{
		color_set_ptr[i] = hi; 
		color_set_ptr[i+1] = lo; 
	}
	
	HAL_SPI_Transmit_DMA(&hspi1, color_set_ptr, counter);
	delete[] color_set_ptr;
	*/
	complete_dma = 1;
	uint16_t *ass = new uint16_t[counter];
	//uint16_t ass[5];
	//uint8_t ass[5];
	//uint8_t fff[2] = { (uint8_t)highByte(color), (uint8_t)lowByte(color) };
	memset16safe(ass, highByte(color) | lowByte(color) << 8, counter);
	
	//uint8_t *ass1 = (uint8_t *)ass;
	//for(int16_t i = 0 ; i < counter*2 ; i++) {
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)ass, counter*2);
	while (complete_dma) ;
	//setCurrentPixel(color);
	//HAL_SPI_Transmit(&hspi1, &hi, 1,0);
	//HAL_SPI_Transmit(&hspi1, &lo, 1,0);
    //}
     
	//HAL_Delay(10);
	chipDeselect();
	delete[] ass;
	/*
	LL_GPIO_SetOutputPin(LCD_A0_GPIO_Port, LCD_A0_Pin);
	for (int16_t i = 0; i < counter ; i++) {
		//writeData(hi); writeData(lo);
		
		GPIOA->BSRR = (~hi)<<16 | hi;
		GPIOB->BSRR = GPIO_BSRR_BR8;
		GPIOB->BSRR = GPIO_BSRR_BS8;
		GPIOA->BSRR = (~lo) << 16 | lo;
		GPIOB->BSRR = GPIO_BSRR_BR8;
		GPIOB->BSRR = GPIO_BSRR_BS8;
		
	}
	GPIOA->BRR = 0xFFFF;*/
/*	
    for (int16_t i = 0; i < counter / 20; i++) {
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
        writeData(hi);writeData(lo);
    }
    for (int32_t i = 0; i < counter % 20; i++) {
        writeData(hi);writeData(lo);
    }
*/
    
}

void Pixels::setRegion(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    if ( orientation != PORTRAIT ) {
        int16_t buf;
        switch( orientation ) {
        case LANDSCAPE:
            buf = x1;
            x1 = deviceWidth - y1 - 1;
            y1 = buf;
            buf = x2;
            x2 = deviceWidth - y2 - 1;
            y2 = buf;
            break;
        case PORTRAIT_FLIP:
            y1 = deviceHeight - y1 - 1;
            y2 = deviceHeight - y2 - 1;
            x1 = deviceWidth - x1 - 1;
            x2 = deviceWidth - x2 - 1;
            break;
        case LANDSCAPE_FLIP:
            buf = y1;
            y1 = deviceHeight - x1 - 1;
            x1 = buf;
            buf = y2;
            y2 = deviceHeight - x2 - 1;
            x2 = buf;
            break;
        }

        if (x2 < x1) {
            swap(x1, x2);
        }
        if (y2 < y1) {
            swap(y1, y2);
        }
    }

    writeCmd(0x2a);
    writeData(x1>>8);
    writeData(x1);
    writeData(x2>>8);
    writeData(x2);
    writeCmd(0x2b);
    writeData(y1>>8);
    writeData(y1);
    writeData(y2>>8);
    writeData(y2);
    writeCmd(0x2c);
}

void Pixels::deviceWriteData(uint8_t high, uint8_t low) {
    writeData(high);
    writeData(low);
}


void *Pixels::memset16safe(void *m, uint16_t val, size_t count)
{
	char *buf = (char *)m;
	union 
	{
		uint8_t d8[2];
		uint16_t d16;
	}u16 = { .d16 = val };

	while (count--) 
	{
		*buf++ = u16.d8[0];
		*buf++ = u16.d8[1];
	}
	return m;
}




#endif
