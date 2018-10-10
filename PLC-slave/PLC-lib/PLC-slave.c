#include "PLC-slave.h"

Slave slave;

/*--------------------------------*/
/* Slave */
void Slave_Reset()
{
	slave.ID = 1;
	slave.time_remaining = 0;	
}

// Slave :: Update 1
void Slave_Update_1()
{
	if(slave.time_remaining<=0)	// Response Master
	{
		Fn_UART_Puts("FR:3/");	
		Fn_UART_PutInt(slave.ID);
		Fn_UART_Puts(":FR");
		slave.time_remaining = 15;
	}	
}

// Slave :: Update 2
void Slave_Update_2()
{
	uint8_t cmd;
	uint8_t ID;
	uint8_t device;
	uint8_t status;
	
	if(frame.check_stop==1)
	{
		Frame_Split();
		//Frame_Info();
		cmd = String_Str2Int(frame.frame_split[0]);
		ID = String_Str2Int(frame.frame_split[1]);
		switch(cmd)
		{
			case 1:
				break;
			case 2:
				if(ID==slave.ID)
				{
					device = String_Str2Int(frame.frame_split[2]);
					status = String_Str2Int(frame.frame_split[3]);
					switch(device)
					{
						case 1:
							digitalWrite(C3, status);
							break;
						case 2:
							digitalWrite(C4, status);
							break;
						case 3:
							digitalWrite(C5, status);
							break;
						case 4:
							digitalWrite(C6, status);
							break;
					}
				}
				break;
			case 3:	// Slave Update 1
				break;
			case 4:
				break;		
		}
		Frame_Reset();
    }	
}

INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
    if(slave.time_remaining>=0)
    	slave.time_remaining--;
    TIM2->SR1 = (0<<0);
}

