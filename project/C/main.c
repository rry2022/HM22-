#include <SN8F5762.h>
#include "Init.h"
#include "AD.h"
#include "test.h"
#include "ctr.h"
#include "time.h"
#include "key.h"
#include "uart.h"

#define MyDef		
#include "global.h"


void main(void)
{
	WDTR = 0X5A;  //关看门狗
	Init_IO();
	Init_Time0();
	Init_PWM();
	Init_ADC();
		
	EAL = 1;
	
	while(1)
	{
  	WDTR = 0X5A;  //关看门狗
		if(Ft_2ms)
		{
		  Ft_2ms = 0;
			adcDEAL();
			if(++cnt_10ms >= 5)
			{
				cnt_10ms = 0;
				Scan_chadian();  //插电检测
				keyscan();
				keylongDEAL();
			}
			if(++cnt_125ms >= 63)
			{
				cnt_125ms = 0;
				Ctrl_LED();
			}
			if(++cnt_100ms >= 50)
			{
				cnt_100ms = 0;
				Scan_guoya();    //过压检测
				Scan_Temp();     //温度检测
				Scan_BATVol();   //电池电压检测	
        Ctrl_Charge();	 //充电控制
				Motor1_Ctrl();   //振动电机控制
			}
			if(++cnt_500ms >= 250)
			{
				cnt_500ms = 0;
				Charge_timecounter(); //计时函数
			}
			if(++cnt_4s >= 2000) //串口发送数据--4S发送一次
			{
				cnt_4s = 0;
				Uart_SendByte((Con_ChargeVolt>>8));
				Uart_SendByte((Con_ChargeVolt & 0x00ff));
				Uart_SendByte(Dec_num);
			}
		}
  }
}

void T0_Interrupt(void) interrupt ISRTimer0 
{
	TH0 = 0xf8;    //延时2ms
	TL0 = 0x30;
	Ft_2ms = 1;
}


