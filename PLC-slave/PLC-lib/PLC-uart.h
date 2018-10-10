#ifndef _PLC_uart_h_
#define _PLC_uart_h_

#include "stm8s.h"
#include "PLC.h"

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
