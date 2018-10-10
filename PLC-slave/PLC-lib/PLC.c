#include "PLC.h"

Queue Q_data;
Frame frame;

/* Queue Init */
void Queue_Init()
{
    uint8_t i;
	Q_data.MAX = 100;
	for(i=0;i<100;i++)
          Q_data.data[i] = '\0';
	Q_data.front = 0;
	Q_data.rear = -1;
	Q_data.item_count = 0;
}

/* Queue Push */
void Queue_Push(uint8_t data)
{
	if(Q_data.item_count<=Q_data.MAX)
	{
		if(Q_data.rear==Q_data.MAX-1)
			Q_data.rear = -1;
		Q_data.data[++Q_data.rear] = data;
		Q_data.item_count ++;	
	}
}

/* Queue Pop */
uint8_t Queue_Pop()
{
	uint8_t data = Q_data.data[Q_data.front++];
	if(Q_data.front==Q_data.MAX)
		Q_data.front = 0;
	Q_data.item_count--;
	return data;
}

/*----------------------------------------------------*/
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


/*----------------------------------------------------*/
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

