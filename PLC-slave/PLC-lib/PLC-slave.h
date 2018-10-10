#ifndef _PLC_SLAVE_H_
#define _PLC_SLAVE_H_


#include "stm8s.h"
#include "PLC.h"
#include "PLC-gpio.h"
#include "PLC-uart.h"

typedef struct Slave
{
	uint8_t ID;
	int8_t time_remaining;
}Slave;

/*-----------------------*/
void Slave_Reset();
void Slave_Update_1();
void Slave_Update_2();

extern Slave slave;


#endif
