#ifndef _PLC_uart_h_
#define _PLC_uart_h_

#include "stm8s.h"


/* 
    Struct Queue 
 */
typedef struct Queue{
	uint8_t MAX;            // Max of Queue
	uint8_t data[100];      // Value    	
	uint8_t front;          // Front Index   
	int8_t rear;            // Rear Index
	uint8_t item_count;     // Number of Queue at present
}Queue;

extern Queue Q_data;

// Queue Init
void Queue_Init();

// Queue insert new data
void Queue_Push(uint8_t data);

// Queue get data
uint8_t Queue_Pop();

/*---------------------------------------------------*/


/*
  Function for UART
 */

// Config UART
void Config_Uart(void);

// UART send a character
void Fn_UART_PutChar(char _vruc_Data);

// UART send a string
void Fn_UART_Puts(char *_vruc_String);

// UART send a Integer
void Fn_UART_PutInt(int8_t _vruc_Int);

// Interrupt Timer
void Config_Timer();

#endif
