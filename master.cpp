#include<iostream>
using namespace std;
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"

/*
 * typedef struct MASTER
 * 			uint8_t num_devices: Number of devices are working at present
 *			uint8_t *id_devices: Id of each device are working
 *
 */
 
typedef struct MASTER
{
	uint8_t num_devices;
	uint8_t *id_devices;
}MASTER;



/*
 * MASTER Master_Init()
 * 	To create new Master
 *	Example: MASTER master = Master_Init();
 *
 */

MASTER Master_Init()
{
	MASTER master;
	master.num_devices = 0;
	master.id_devices = (uint8_t *)malloc(sizeof(uint8_t*)*0);
	return master;
}


/*
 * void Master_InsertDevice(MASTER &master, uint8_t id_device)
 *		To Insert 1 device to Master Manger with ID=id_device
 *		Example: Master_InsertDevice(master, 1);
 * 
 */

void Master_InsertDevice(MASTER &master, uint8_t id_device)
{
	master.num_devices++;
	master.id_devices = (uint8_t *)realloc(master.id_devices, master.num_devices);
	master.id_devices[master.num_devices-1] = id_device;
}


/*
 * int8_t Master_FindDevice(MASTER master, uint8_t id_device)
 * 		To Find 1 device in Master Manager
 *		-> Return index of device
 *		-> Return -1 if device does not exist in Master Manager
 *		Example: Master_FindDevice(master, 1);
 * 
 */

int8_t Master_FindDevice(MASTER master, uint8_t id_device)
{
	uint8_t i;
	if(master.num_devices==0)
		return false;
	for(i=0; i<master.num_devices; i++)
		if(master.id_devices[i]==id_device)
			return i;
	return -1;
}


/*
 * void Master_DeleteDevice(MASTER &master, uint8_t id_device)
 *		To Delete 1 device exist in Master Manager
 *		
 *		i				0	1	2	3	4
 *		id_devices[i]	2	3	1	7	8
 *		
 *		Example: Master_DeleteDevice(master, 3);
 *
 *		i				0	1	2	3
 *		id_devices[i]	2	1	7	8		
 *
 */

void Master_DeleteDevice(MASTER &master, uint8_t id_device)
{
	uint8_t i, j;
	uint8_t index_id = Master_FindDevice(master, id_device);
	if(index_id==-1)
		return;
	for(i=index_id; i<master.num_devices-1; i++)
		master.id_devices[i] = master.id_devices[i+1];
	master.num_devices--;
	master.id_devices = (uint8_t *)realloc(master.id_devices, master.num_devices);
}

/*----------------------------------------*/
void String_nCopy(char *str_des, char *str_src, uint8_t start, uint8_t finish)
{
	uint8_t i, j=0;
	for(i=start; i<=finish; i++)
		str_des[j++] = str_src[i];
	str_des[j] = '\0';
}

void String_Split(char *str_src, uint8_t ch_split, char str_res[][20], uint8_t &num_component)
{
	uint8_t i = 0;
	uint8_t num = 0;
	uint8_t tmp = 0;
	uint8_t len = strlen(str_src);
	for(i=0; i<len; i++)
	{
		if(str_src[i]==ch_split)
		{
			num++;
			String_nCopy(str_res[num-1], str_src, tmp, i-1);
			tmp = i+1;
		}
	}
	if(tmp!=len-1)
	{
		num++;
		String_nCopy(str_res[num-1], str_src, tmp, len-1);
	}
	num_component = num;
}

/*---------------------------------------*/
void InforMaster(MASTER master)
{
	int i;
	cout<<"Num Devices: "<<(uint16_t)master.num_devices<<endl;
	for(i=0; i<master.num_devices; i++)
	{
		cout<<"ID "<<i<<" : "<<(uint16_t)master.id_devices[i]<<endl;
	}
}


int main()
{
	char str_res[10][20];
	uint8_t num_component;
	char str[] = "hello:hi:kaka";
	String_Split(str, ':', str_res, num_component);
	for(int i=0; i<3; i++)
		cout<<str_res[i]<<endl;
	cout<<"Num Component: "<<(uint16_t)num_component<<endl;
	return 0;
}
