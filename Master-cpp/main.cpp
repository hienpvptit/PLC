#include "stdint.h"
#include "lib/stm8s_conf.h"
#include "lib/stm8s.h"
#include "lib/stm8s_gpio.h"
#include "lib/PLC-master.h"
#include "lib/PLC-uart.h"

#define LED_GPIO_PORT  (GPIOB)
#define LED_GPIO_PINS  (GPIO_PIN_5 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0)

void delay_ms(int a)
{
  for(int i=0;i<a;i++)
    for (int index = 0; index < 400; index++);
}

void blink()
{
  uint8_t i;
  for(i=0; i<3; i++)
  {
    GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    delay_ms(300);
  }
}

int main()
{
  // Queue Data Init
  Q_data = Queue_Init();
  
  // Config Uart baud 9600bps
  Config_uart();
  enableInterrupts();
  
  // Config GPIO
  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  
  // Main
  while(1)
  {    
    if(Q_data.item_count!=0)
    {  
        Fn_UART_SendChar(Queue_Pop(&Q_data));
        blink();
    }
  }

}