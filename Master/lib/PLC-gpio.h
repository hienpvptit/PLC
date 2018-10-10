#ifndef _PLC_GPIO_H_
#define _PLC_GPIO_H_

#include "stm8s.h"

#define B5 25

#define C3 33
#define C4 34
#define C5 35
#define C6 36


// Setup INPUT or OUTPUT 
void GPIO_Setup();
// GPIO Blink test
void digitalWrite(uint8_t _gpio, uint8_t _mode);

#endif
