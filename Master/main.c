#include "lib/stm8s.h"
#include "lib/PLC-gpio.h"
#include "lib/PLC-uart.h"
#include "lib/PLC-master.h"

void test()
{
  unsigned int i;
  for(i=0 ;i< 50000; i++){};
}

int main( void )
{
  Master_Reset();
  Frame_Reset();
  
  Queue_Init();
  
  Config_Uart();
  Config_Timer();
  enableInterrupts();
  
  while(1)
  {
//    if(Q_data.item_count != 0)
//    {
//      Frame_Catch(Queue_Pop());
//      Master_Update_2();
//    }
//    Master_Update_1();
    Fn_UART_Puts("Delete Device! ID = ");
    test();
  }
  
}
