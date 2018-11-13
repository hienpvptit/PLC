#include "PLC-lib/stm8s.h"
#include "PLC-lib/PLC-slave.h"

int main( void )
{
  Slave_Reset();
  Frame_Reset();
  Queue_Init();
    
  Config_Uart();
  Config_Timer();
  GPIO_Setup();
  enableInterrupts();
  test_count = 0;
  while(1)
  {
    if(Q_data.item_count != 0)
    {
      Frame_Catch(Queue_Pop());
      Slave_Update_2();
    }
    Slave_Update_1();
  }
}
