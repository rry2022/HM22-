#include "global.h"

/*-----------------------------------------------------------
* 函数：void Charge_timecounter(void)
* 功能：充电计时函数,500ms执行一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void Charge_timecounter(void)
{
	if(!F_ChargeSTA)
	{
		Charge_time++;  //充电计时
		if(Charge_time >= 21600) //3H
		{
			F_ChargeFull = 1;
		}
		if(Charge_time >= 36000) //5H
		{
			Charge_time = 0;
			F_ChargeSTA = 1; //关充电
		}
		
		if(Charge_Mode == Charge_S) //低压涓流充电不能超过1H
		{
			if(++cnt_1hLowVol >= 7200)//1H
			{
				cnt_1hLowVol = 0;
				F_ChargeErro = 1;
			}
		}
		else 
		{
			if(cnt_1hLowVol > 72)
			{
				cnt_1hLowVol -= 72;
			}
			else
			{
				cnt_1hLowVol = 0;
			}
		}
		
		if((Charge_Mode == Charge_Other) && (Temp_Level==4)) //高温涓流充电不能超过1H
		{
			if(++cnt_1hHightemp >= 7200)//1H
			{
				cnt_1hHightemp = 0;
				F_ChargeErro = 1;
			}
		}
		else
		{
			if(cnt_1hHightemp > 72)
			{
				cnt_1hHightemp -= 72;
			}
			else
			{
				cnt_1hHightemp = 0;
			}
		}
	}
	else
	{
		cnt_1hHightemp = 0;
		cnt_1hLowVol = 0;
		Charge_time = 0;
	}
}