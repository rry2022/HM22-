#include "global.h"

/*-----------------------------------------------------------
* ������void Charge_timecounter(void)
* ���ܣ�����ʱ����,500msִ��һ��
* ���룺 ��
* ����� ��
------------------------------------------------------------*/
void Charge_timecounter(void)
{
	if(!F_ChargeSTA)
	{
		Charge_time++;  //����ʱ
		if(Charge_time >= 21600) //3H
		{
			F_ChargeFull = 1;
		}
		if(Charge_time >= 36000) //5H
		{
			Charge_time = 0;
			F_ChargeSTA = 1; //�س��
		}
		
		if(Charge_Mode == Charge_S) //��ѹ�����粻�ܳ���1H
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
		
		if((Charge_Mode == Charge_Other) && (Temp_Level==4)) //���������粻�ܳ���1H
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