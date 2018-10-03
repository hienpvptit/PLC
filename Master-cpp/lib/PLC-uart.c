#include "PLC-uart.h"

Queue Q_data;

/* Queue Init */
Queue Queue_Init()
{
        uint8_t i;
	Queue que;
	que.MAX = 100;
	for(i=0;i<100;i++)
          que.data[i] = '\0';
	que.front = 0;
	que.rear = -1;
	que.item_count = 0;
	return que;
}

/* Queue Push */
void Queue_Push(Queue *que, uint8_t data)
{
	if(que->item_count<=que->MAX)
	{
		if(que->rear==que->MAX-1)
			que->rear = -1;
		que->data[++que->rear] = data;
		que->item_count ++;
	}
}

/* Queue Pop */
uint8_t Queue_Pop(Queue *que)
{
	uint8_t data = que->data[que->front++];
	if(que->front==que->MAX)
		que->front = 0;
	que->item_count--;
	return data;
}

/*-----------------------------------------------------*/

// Config UART
void Config_uart(void)
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
void Fn_UART_SendChar (char _vruc_Data)
{
    UART1->DR = _vruc_Data;
    while(!(UART1->SR & (1<<7)));
}
          
// Send string          
void Fn_UART_Pustf (char *_vruc_String)
{
    while(*_vruc_String)
    {
        Fn_UART_SendChar(*_vruc_String);
        _vruc_String++;
    }
}

// Rx Interrupt
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  uint8_t tmp = UART1->DR;
  if(tmp>=33 && tmp<=126)
    Queue_Push(&Q_data, tmp);
}


