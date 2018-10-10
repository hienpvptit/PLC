#include "PLC-master.h"

/*--------------------------------*/
/* Extern variable */
Frame frame;
Master master;

/*--------------------------------*/

/* Frame */
// Frame :: Frame Reset
void Frame_Reset()
{
	uint8_t i, j;
	
	// Reset check variable
	frame.check_start = 0;
	frame.check_stop  = 0;
	
	// Reset buff
	for(i=0; i<3; i++)
		frame.buff[i] = '?';
	frame.buff[3] = '\0';
	
	// Reset dataframe
	for(i=0; i<30; i++)
		frame.data_frame[i] = '\0';
	
	// Reset nm_split
	frame.num_split = 0;
	
	// Reset data_split
	for(i=0; i<7; i++)
		for(j=0; j<5; j++)
		frame.frame_split[i][j] = '\0';		
}

// Frame :: Catch Frame
void Frame_Catch(uint8_t ch)
{
	/*
        buff    0 1 2
           <-   a b c   <- new character
        compare buff to catch "MS:", "SL:", ":MS", ":SL"
   */
   	char tmp[2];
	tmp[0] = '\0'; tmp[1] = '\0';
	
	frame.buff[0] = frame.buff[1];
	frame.buff[1] = frame.buff[2];
	frame.buff[2] = ch;
	
	// Catch Start Frame
	if(frame.check_start==0)
	{
		if(strcmp(frame.buff, "FR:")==0)
		{
			frame.check_start = 1;
		}
	}
	else
	{
		tmp[0] = ch;
		strcat(frame.data_frame, tmp);
		if(strcmp(frame.buff, ":FR")==0)
		{
			frame.check_stop = 1;
			frame.data_frame[strlen(frame.data_frame)-3] = '\0';
		}
	}
}

// Frame :: Split Frame
void Frame_Split()
{
	/*
		Split data_frame by '/'
		Input: Frame -> data_frame[]
		Output: Frame -> frame_split[][]
				Frame -> num_split
	*/
	uint8_t i = 0;
	uint8_t tmp = 0;
	uint8_t len = strlen(frame.data_frame);
	
	for(i=0; i<len; i++)
	{
		if(frame.data_frame[i]=='/')
		{
			frame.num_split++;
			String_nCopy(frame.frame_split[frame.num_split-1], frame.data_frame, tmp, i-1);
			tmp = i+1;
		}
	}
	if(tmp<len)
	{
		frame.num_split++;
		String_nCopy(frame.frame_split[frame.num_split-1], frame.data_frame, tmp, len-1);
	}
}

// Frame :: Frame Infomation
void Frame_Info()
{
	uint8_t i;
	Fn_UART_Puts("Frame Info\n------\n");
	Fn_UART_Puts("check_start: ");	Fn_UART_PutInt(frame.check_start); 	Fn_UART_PutChar('\n');
	Fn_UART_Puts("check_stop : ");	Fn_UART_PutInt(frame.check_stop);  	Fn_UART_PutChar('\n');
	Fn_UART_Puts("buff: "); 	 	Fn_UART_Puts(frame.buff); 			Fn_UART_PutChar('\n');
	Fn_UART_Puts("dataframe : "); 	Fn_UART_Puts(frame.data_frame);		Fn_UART_PutChar('\n');
	Fn_UART_Puts("num_split : "); 	Fn_UART_PutInt(frame.num_split);	Fn_UART_PutChar('\n');
	Fn_UART_Puts("frame_split: ");
	for(i=0; i<frame.num_split; i++)
	{
		Fn_UART_Puts(frame.frame_split[i]);
		Fn_UART_Puts("  ");
	}
	Fn_UART_Puts("\n------\n\n");
}

/*--------------------------------*/
/* Master */
/*--------------------------------------------------------------------------------*/
Master master;

void Master_Reset()
{
	uint8_t i;
	master.num_devices = 0;
	for(i=0; i<10; i++)
	{
		master.id_devices[i]  = 0;
		master.time_remaining[i] = 0;
	}	
}

// Master :: Insert new device
void Master_InsertDevice(uint8_t id_device)
{
	if(master.num_devices==10 || id_device>9 || id_device<1)
		return;
	master.num_devices++;
	master.id_devices[master.num_devices-1] = id_device;
	master.time_remaining[id_device] = TIME_REMAIN;
}

// Master :: Find a device in list available
int8_t Master_FindDevice(uint8_t id_device)
{
	uint8_t i;
	if(master.num_devices==0 || id_device>9 || id_device<1)
		return -1;
	for(i=0; i<master.num_devices; i++)
		if(master.id_devices[i]==id_device)
			return i;
	return -1;
}

// Master :: Delete a device in list available
void Master_DeleteDevice(uint8_t id_device)
{
	uint8_t i;
	int8_t index_id = Master_FindDevice(id_device);
	if(index_id==-1)
		return;
	for(i=index_id; i<master.num_devices-1; i++)
		master.id_devices[i] = master.id_devices[i+1];
	master.id_devices[master.num_devices-1] = 0;
	master.num_devices--;
	master.time_remaining[id_device] = 0;
}

// Master :: Update_1
void Master_Update_1()
{
	uint8_t i;
	if(master.num_devices==0)
		return;
	for(i=0; i<master.num_devices; i++)
	{
		if(master.time_remaining[master.id_devices[i]]<=0)
		{
			Fn_UART_Puts("Delete Device! ID = ");
			Fn_UART_PutInt(master.id_devices[i]);
			Fn_UART_PutChar('\n');
			Master_DeleteDevice(master.id_devices[i]);
		}
	}
}

// Master :: Update_2
void Master_Update_2()
{
	uint8_t cmd;
	uint8_t ID;
	uint8_t mode;
	uint8_t temp = 0;
	uint8_t hum = 0;
	
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
				break;
			case 3:	// Slave response Master
				if(ID>0 && ID<10)
				{
					if(Master_FindDevice(ID) == -1)
					{
						Master_InsertDevice(ID);
						Fn_UART_Puts("Insert New Device! ID = ");
						Fn_UART_PutInt(ID);
						Fn_UART_PutChar('\n');
					}
					else
					{
						master.time_remaining[ID] = 30;
						Fn_UART_Puts("Refresh Device! ID = ");
						Fn_UART_PutInt(ID);
						Fn_UART_PutChar('\n');	
					}
					break;
				}
			case 4:	// Slave send data to Master
				if(ID>0 && ID<10)
				{
					mode = 	String_Str2Int(frame.frame_split[2]);
					switch(mode)
					{
						case 1:
							temp = String_Str2Int(frame.frame_split[3]);
							break;
						case 2:
							hum = String_Str2Int(frame.frame_split[3]);
							break;
						case 3:
							temp = String_Str2Int(frame.frame_split[3]);
							hum = String_Str2Int(frame.frame_split[4]);
							break;
					}
				}
				break;		
		}
		Frame_Reset();
    }
}

// Master Info
void Master_Info()
{
	uint8_t i;
	Fn_UART_Puts("Master Info\n------------------\n");
	Fn_UART_Puts("num_devices: ");	Fn_UART_PutInt(master.num_devices); Fn_UART_PutChar('\n');
	Fn_UART_Puts("id_devices : ");
	for(i=0; i<master.num_devices; i++)
	{
		Fn_UART_PutInt(master.id_devices[i]);
		Fn_UART_Puts(" ");
	}
	Fn_UART_PutChar('\n');
	Fn_UART_Puts("time_remaining: ");
	for(i=0; i<10; i++)
	{
		Fn_UART_PutInt(master.time_remaining[i]);
		Fn_UART_PutChar('\n');
	}
	Fn_UART_Puts("\n---------------------\n\n");
}

/*--------------------------------*/
/* String */

// String :: Copy n character
void String_nCopy(char *str_des, char *str_src, uint8_t start, uint8_t finish)
{
	uint8_t i, j=0;
	for(i=start; i<=finish; i++)
		str_des[j++] = str_src[i];
	str_des[j] = '\0';
}

// String :: Str2Int
uint8_t String_Str2Int(char *str)
{
	uint8_t i;
	uint8_t tmp = 0;
	for(i=0; i<strlen(str); i++)
		tmp = tmp*10 + str[i]-48;
	return tmp;
}

// Interrupt Master each 1 second
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
    uint8_t i;
    if(master.num_devices != 0)
      for(i=0; i<master.num_devices; i++)
      	if(master.time_remaining[master.id_devices[i]]>=0)
        	master.time_remaining[master.id_devices[i]]--; 
    TIM2->SR1 = (0<<0);
}


