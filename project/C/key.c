#include "global.h"

/*-----------------------------------------------------------
* 函数：void keyscan(void)
* 功能：按键短按检测函数，10ms跑一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void keyscan(void)
{
  P0 &= 0xfb;    //1111 1011   
	P0M &= 0xe3;   //1110 0011
	P0UR |= 0x0c;  //0000 1100
		
	keyNEW =0;
	if(K1==0)
	{keyNEW=keyNEW | 0x01;}	
	if(K2==0)
	{keyNEW=keyNEW | 0x02;}
	if(K3==0)
	{keyNEW=keyNEW | 0x04;}	
	temp=keyNEW ^keyBUF;
	if(keyBUF!=keyNEW)
	{
		DEB+=1;
		if(DEB>=6)
		{
			DEB=0;
			keyBUF=keyNEW;
			if(temp & 0x01)  //电源开关
			{
				if(K1==0)			
				{	
					fk1DOWN=1;
				}
				else
				{
					fk1DOWN=0;
					if(fk1LONG)
					{
						fk1LONG=0;
					}
				}
			}
			if(temp & 0x02)  //振动马达档位切换开关
			{
				if(K2==0)			
				{	
					//K2功能------
					if(F_Power) //开机时按键有效
					{
						F_Turn1 = 1;
						if(Motor1_Mode < 3)
						{
							Motor1_Mode++;
						}
						else
						{
							Motor1_Mode = 0;
						}
				  }
				}
			}
			if(temp & 0x04)  //转动马达档位切换开关
			{
				if(K3==0)			
				{	
					//K3功能------
				}
			}
		}
	}
	else
	{
		if(DEB>=2)
		{DEB=DEB-2;}
		else
		{DEB=0;}
	}
	
  P0 |= 0x04;    //0000 0100 
	P0M |= 0x1c;   //0001 1100
	P0UR &= 0xe7;  //1110 0111
}



/*-----------------------------------------------------------
* 函数：void keylongDEAL(void)
* 功能：按键长按检测函数，10ms跑一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void keylongDEAL(void)
{
	if(fk1DOWN)						
	{
		if(fk1LONG==0)
		{
			cntK1LONG++;
			if(cntK1LONG>=150)
			{
				cntK1LONG=0;
				fk1LONG=1;
				if(F_Power)  //按下响应，总开关
				{
					F_Power = 0;
				}
				else
				{
					F_Power = 1;
					F_kuidian = 0;
				}
			}
		}
	}
	else
	{
		cntK1LONG=0;
		
	}
}