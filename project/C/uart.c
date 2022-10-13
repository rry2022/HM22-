#include "global.h"

//IO模拟UART发送
void Delay104us(void)
{//波特率9600 104 us
  unsigned char i;
//  Test_Port = 1;
  for(i=0;i<162;i++)
 	{;}
//  Test_Port = 0;
}

void Uart_SendByte(unsigned char SendData)
{
  unsigned char i;
  UART_TX=1;
 	UART_TX=0;  
	Delay104us();
	EAL = 0;
  for(i=0;i<8;i++)
	{
    if(SendData & 0x01)
		{
   			UART_TX=1;
    }
		else
		{
   		UART_TX=0;
   	}
    Delay104us();
    SendData >>= 1;
  }
  UART_TX=1;
	EAL = 1;
	     
  Delay104us();
  Delay104us();
}

