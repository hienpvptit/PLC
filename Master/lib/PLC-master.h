#ifndef _PLC_MASTER_H_
#define _PLC_MASTER_H_

#include "stm8s.h"
#include "string.h"
#include "PLC-uart.h"

#define TIME_REMAIN 30
/*------------------------------*/
typedef struct Master
{
	uint8_t num_devices;
	uint8_t id_devices[10];	
	uint8_t time_remaining[10];
	/*
		num_devices:
			Number devices are online
		id_devices:
			Id devices are online
		time_remaining:
			Time remaining devices. 
			If time remaining == 0 -> Devices is offline
			Time remaining is being refresh if slave response
			time_remaining is weight array with id_devices: time_remaining(id_devices[])
	*/
} Master;

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
							2/1/d1/on  -> Control d1 in Slave have ID = 1 to on
							2/3/d2/off -> Control d2 in Slave have ID = 3 to off
							
					3 - Slave response Master
						Example dataFrame:
							3/1 -> Slave have ID = 01 response online
							
					4 - Slave send data to Master
						Example dataFrame:
							4/1/1/27		-> Salve have ID = 1 send Temperature = 27
							4/1/2/60		-> Salve have ID = 1 send Humidity = 60
							4/1/3/27/60	-> Salve have ID = 1 send Temp/Hum = 27/60
		 */
} Frame;

void Frame_Reset();
void Frame_Catch(uint8_t ch);
void Frame_Split();
void Frame_Info();
/* */
void Master_Reset();
void Master_InsertDevice(uint8_t id_device);
int8_t Master_FindDevice(uint8_t id_device);
void Master_DeleteDevice(uint8_t id_device);
void Master_Update();
void Master_Info();
/**/
void String_nCopy(char *str_des, char *str_src, uint8_t start, uint8_t finish);
uint8_t String_Str2Int(char *str);

extern Frame frame;
extern Master master;
#endif
