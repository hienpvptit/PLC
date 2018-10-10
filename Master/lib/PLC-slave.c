#include "PLC-slave.h"

Frame frame;
Slave slave;
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
        compare buff to catch "FR:", ":FR"
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
							digitalWrite(C1, status);
							break;
						case 2:
							digitalWrite(C2, status);
							break;
						case 3:
							digitalWrite(C3, status);
							break;
						case 4:
							digitalWrite(C4, status);
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

// Interrupt Slave each 1 second
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
    uint8_t i;
    if(slave.time_remaining>=0)
    	slave.time_remaining--;
    TIM2->SR1 = (0<<0);
}
