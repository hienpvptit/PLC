#include "PLC-gpio.h"

void GPIO_Setup()
{
	GPIOB->ODR = 0;
	GPIOB->DDR = (1<<5);
	GPIOB->CR1 = (1<<5);
	GPIOB->CR2 = (1<<5);
	
	GPIOC->ODR = 0;
	GPIOC->DDR = (1<<3 | 1<<4 | 1<<5 | 1<<6);
	GPIOC->CR1 = (1<<3 | 1<<4 | 1<<5 | 1<<6);
	GPIOC->CR2 = (1<<3 | 1<<4 | 1<<5 | 1<<6);
}

void digitalWrite(uint8_t _gpio, uint8_t _mode)
{
	if(_gpio>=20 && _gpio<=29)
	{	
		if(_mode)
			GPIOB->ODR |= (1<<_gpio%10);
		if(!_mode)
			GPIOB->ODR &= ~(1<<_gpio%10);
	}
	
	if(_gpio>=30 && _gpio<=39)
	{	
		if(_mode)
			GPIOC->ODR |= (1<<_gpio%10);
		if(!_mode)
			GPIOC->ODR &= ~(1<<_gpio%10);
	}
}

