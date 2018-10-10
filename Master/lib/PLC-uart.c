#include "PLC-uart.h"

Queue Q_data;

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

/*-----------------------------------------------------*/

// Config UART
void Config_Uart(void)
{
   CLK->CKDIVR = 0;
   CLK->PCKENR1 = 0xff;
   CLK->PCKENR2 = 0xff;
   GPIOD->DDR |=  (1<<5);
   GPIOD->DDR &= ~(1<<6);
   UART1->BRR1 = 0x68;		// Baud 9600bps
   UART1->BRR2 = 0x03;
   UART1->CR1  = (0<<5);
   UART1->CR2  = (1<<2) | (1<<3) | (1<<5);
}

// Send a character
void Fn_UART_PutChar (char _vruc_Data)
{
    UART1->DR = _vruc_Data;
    while(!(UART1->SR & (1<<7)));
}
          
// Send string          
void Fn_UART_Puts (char *_vruc_String)
{
    while(*_vruc_String)
    {
        Fn_UART_PutChar(*_vruc_String);
        _vruc_String++;
    }
}

// Send Integer
void Fn_UART_PutInt(int8_t _vruc_Int)
{
	int8_t i;
	uint8_t j;
	uint8_t _str[5];
	if(_vruc_Int==0)
	{
		Fn_UART_PutChar('0');	
		return;
	}
	
	for(i=0;i<5;i++)
		_str[i] = '\0';
	if(_vruc_Int<0)
	{
		Fn_UART_PutChar('-');
		_vruc_Int *= -1;
	}
	j = 0;
	while(_vruc_Int>0)
	{
		_str[j++] = _vruc_Int%10 + 48;
		_vruc_Int/=10;
	}
	for(i=j-1; i>=0; i--)
		Fn_UART_PutChar(_str[i]);
}

// Rx Interrupt
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  uint8_t tmp = UART1->DR;
  if(tmp>=33 && tmp<=126)
    Queue_Push(tmp);
}

/*-----------------------------------------------*/
// Timer
void Config_Timer()
{
  CLK->CKDIVR = 0;
  CLK->PCKENR1 = 0xff;
  CLK->PCKENR2 = 0xff;
  //16MHZ/1024 = 15625
  TIM2->PSCR = 0x0A; 
  //15625 * 1s = 15625 =0x3D09
  TIM2->ARRH = 0x3D; // 8 bit high
  TIM2->ARRL = 0x09; // 8 bit low
  TIM2->IER = 0x01;  // Update interrupt enabled
  TIM2->CR1 = 0x01;  // Counter enabled
}

