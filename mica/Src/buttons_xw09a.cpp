
#include "main.h"
#include "buttons_xw09a.h"

extern I2C_HandleTypeDef hi2c2;

void I2C_ClearBusyFlagErratum(I2C_HandleTypeDef*);
uint8_t mReceive(uint16_t);



button::button(uint16_t button_assy, uint16_t _long_press_time)
{
	button_mask = button_assy;
	button_state = 0;
	pressed_time = 0;
	short_press_time = 10;
	continious_press_time = 100;
	long_press_time = _long_press_time;
	button_short_pressed = 0;
	button_continious_pressed = 0;
	button_long_pressed = 0;
	button_last_state = 0;
	click_count = 0;
	clicks = 0;
}
void button::init()
{

}

void button::check_button_state()
{		
	//static uint8_t clicks = 0;
	button_state = mReceive(button_mask);
		
	if ((button_state != button_last_state) && button_state)
	{
		pressed_time = osKernelGetTickCount();
	}
	
	/*short press button*/
	if (((osKernelGetTickCount() - pressed_time) == short_press_time) && (!button_short_pressed) && button_state)
	{
		button_short_pressed = true;
	}

	/*hold button*/
	if ((osKernelGetTickCount() - pressed_time) > continious_press_time && button_state) 
	{
		click_count++;
	}
	else 
	{
		clicks = 0;
		click_count = 0;
		button_continious_pressed = false;
	}
		
	if ((click_count == 300) && (clicks < 6))
	{			
		button_continious_pressed = true;
		click_count = 0;
		clicks++;
	}
	if ((click_count == 120) && (clicks > 5))
	{			
		button_continious_pressed = true;
		click_count = 0;
			
	}
		
	/*long pressed button*/
	if (((osKernelGetTickCount() - pressed_time) == long_press_time) && (!button_long_pressed) && button_state)
	{
		button_long_pressed = true;
	}		
		
	button_last_state = button_state;
			
}

bool button::button_short_is_pressed()
{
	if (button_short_pressed) 
	{
		button_short_pressed = false;
		return true;
	}
	return false;		
}
	
bool button::button_continious_is_pressed()
{
	if (button_continious_pressed) 
	{
		button_continious_pressed = false;
		return true;
	}
	return false;		
}
	
bool button::button_long_is_pressed()
{
	if (button_long_pressed) 
	{
		button_long_pressed = false;
		return true;
	}
	return false;		
}	

uint8_t mReceive(uint16_t button_mask)
{
	uint32_t cnt = 0;
	uint8_t btn_state[2] = { 0, };
	uint16_t temp_btn_state = 0;
	//Проверяем флаг I2C_FLAG_BUSY, Если завил вызываем I2C_ClearBusyFlagErratum
	//Число 100000 взял с потолка. Его нужно подбирать опытным путём.
	//Подобную проверку нужно делать при проверки всех необходимых флагов в рамках алгоритма. Если задержка больше, чем нужно, вызываем I2C_ClearBusyFlagErratum
	while(__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_BUSY) == SET)
	{
		if (cnt++ > 10000)
		{
			I2C_ClearBusyFlagErratum(&hi2c2);
			//HAL_Delay(2000);
			return 0;
		}
	}
		
	CLEAR_BIT(hi2c2.Instance->CR1, I2C_CR1_POS);
	
	SET_BIT(hi2c2.Instance->CR1, I2C_CR1_ACK);
		
	SET_BIT(hi2c2.Instance->CR1, I2C_CR1_START);
	cnt = 0;
	while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_SB) == RESET)
	{
		if (cnt++ > 10)
		{
			I2C_ClearBusyFlagErratum(&hi2c2);
			return 0;
		}
	}
	(void) hi2c2.Instance->SR1;
	hi2c2.Instance->DR = I2C_7BIT_ADD_READ(0x81);     /////////////0x81 Адрес микросхемы
		
	cnt = 0;
	while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_ADDR) == RESET)
	{
		if (cnt++ > 10000)
		{
			I2C_ClearBusyFlagErratum(&hi2c2);
			return 0;
		}
	}
		

	/* Clear ADDR flag */
	(void) hi2c2.Instance->SR1;
	(void) hi2c2.Instance->SR2;

	/* Generate Stop */
	SET_BIT(hi2c2.Instance->CR1, I2C_CR1_STOP);
		
	///////////////////////////////////Reading//////////////////////////////////////////
		cnt = 0;
	while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_RXNE) == RESET)
	{
		if (cnt++ > 10000)
		{
			I2C_ClearBusyFlagErratum(&hi2c2);
			return 0;
		}
	}
	;
	btn_state[0] = (uint8_t)hi2c2.Instance->DR;
	//=======================================================
		
		CLEAR_BIT(hi2c2.Instance->CR1, I2C_CR1_ACK);	
	cnt = 0;
	while (__HAL_I2C_GET_FLAG(&hi2c2, I2C_FLAG_RXNE) == RESET)
	{
		if (cnt++ > 10000)
		{
			I2C_ClearBusyFlagErratum(&hi2c2);
			return 0;
		}
	}
	;
	btn_state[1] = (uint8_t)hi2c2.Instance->DR;
	
	SET_BIT(hi2c2.Instance->CR1, I2C_CR1_STOP);
	
	
	temp_btn_state = (btn_state[0] << 8) | btn_state[1];
	
	
	if (temp_btn_state & button_mask) 
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}



void I2C_ClearBusyFlagErratum(I2C_HandleTypeDef* i2c)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	// 1. Clear PE bit.
	  CLEAR_BIT(i2c->Instance->CR1, I2C_CR1_PE);

	//  2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
	GPIO_InitStructure.Mode         = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pull         = GPIO_PULLUP;
	GPIO_InitStructure.Speed        = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStructure.Pin 					= GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

	GPIO_InitStructure.Pin          = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);

	// 3. Check SCL and SDA High level in GPIOx_IDR.
	while(GPIO_PIN_SET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10))
	{
		__asm("nop");
	}

	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11))
	{
		__asm("nop");
	}

	// 4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);

	//  5. Check SDA Low level in GPIOx_IDR.
	while(GPIO_PIN_RESET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11))
	{
		__asm("nop");
	}

	// 6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

	//  7. Check SCL Low level in GPIOx_IDR.
	while(GPIO_PIN_RESET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10))
	{
		__asm("nop");
	}

	// 8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

	// 9. Check SCL High level in GPIOx_IDR.
	while(GPIO_PIN_SET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10))
	{
		__asm("nop");
	}

	// 10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to GPIOx_ODR).
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);

	// 11. Check SDA High level in GPIOx_IDR.
	while(GPIO_PIN_SET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11))
	{
		__asm("nop");
	}

	// 12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
	GPIO_InitStructure.Mode         = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Pin          = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.Pin          = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 13. Set SWRST bit in I2Cx_CR1 register.
	  SET_BIT(i2c->Instance->CR1, I2C_CR1_SWRST);

	__asm("nop");

	// 14. Clear SWRST bit in I2Cx_CR1 register.
	  CLEAR_BIT(i2c->Instance->CR1, I2C_CR1_SWRST);

	__asm("nop");

	// 15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register
	  SET_BIT(i2c->Instance->CR1, I2C_CR1_PE);

	// Call initialization function.
	HAL_I2C_Init(i2c);
}