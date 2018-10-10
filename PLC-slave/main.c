#include "PLC-lib/stm8s.h"
#include "PLC-lib/PLC-slave.h"

int main( void )
{
  Slave_Reset();
  Frame_Reset();
  Config_Uart();
  Config_Timer();
  
  enableInterrupts();
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
