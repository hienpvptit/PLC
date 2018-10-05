#include "PLC-gpio.h"

void GPIO_Setup()
{
	GPIOB->ODR = 0;
	GPIOB->DDR = (1<<5);
	GPIOB->CR1 = (1<<5);
	GPIOB->CR2 = (1<<5);
}

