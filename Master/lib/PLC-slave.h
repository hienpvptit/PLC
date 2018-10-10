#ifndef _PLC_SLAVE_H_
#define _PLC_SLAVE_H_

#include "stm8s.h"
#include "string.h"
#include "PLC-gpio.h"
#include "PLC-uart.h"

/*------------------------------*/
typedef struct Frame
{
	uint8_t check_start;
	uint8_t check_stop;
	char buff[4];
	char data_frame[30];
	char frame_split[7][5];
	uint8_t num_split;
	/*
			Data Frame
			-----
			Construction Frame
				Code/MAC/cmd
			-----
			Component
				Code:
					1 - Master Request Status of Slave
						Example dataFrame:
							1/1 -> Request status of Slave have ID = 1
							1/0 -> Request status of all Slaves
							
					2 - Master Control
						Example dataFrame:
							2/1/1/1  -> Control C1 in Slave have ID = 1 to on
							2/3/2/0  -> Control C2 in Slave have ID = 3 to off
							
					3 - Slave response Master
						Example dataFrame:
							3/1 -> Slave have ID = 01 response online
							
					4 - Slave send data to Master
						Example dataFrame:
							4/1/1/27		-> Slave have ID = 1 send Temperature = 27
							4/1/2/60		-> Slave have ID = 1 send Humidity = 60
							4/1/3/27/60		-> Slave have ID = 1 send Temp/Hum = 27/60
		 */
} Frame;
/*-----------------------*/
typedef struct Slave
{
	uint8_t ID;
	int8_t time_remaining;
}Slave;

/*-----------------------*/
void Frame_Reset();
void Frame_Catch(uint8_t ch);
void Frame_Split();
void Frame_Info();
/*-----------------------*/
void Slave_Reset();
void Slave_Update_1();

void String_nCopy(char *str_des, char *str_src, uint8_t start, uint8_t finish);
uint8_t String_Str2Int(char *str);

extern Frame frame;
extern Slave slave;

#endif
