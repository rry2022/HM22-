#include "global.h"

/*-----------------------------------------------------------
* ������void keyscan(void)
* ���ܣ������̰���⺯����10ms��һ��
* ���룺 ��
* ����� ��
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
			if(temp & 0x01)  //��Դ����
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
			if(temp & 0x02)  //����ﵵλ�л�����
			{
				if(K2==0)			
				{	
					//K2����------
					if(F_Power) //����ʱ������Ч
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
			if(temp & 0x04)  //ת����ﵵλ�л�����
			{
				if(K3==0)			
				{	
					//K3����------
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
* ������void keylongDEAL(void)
* ���ܣ�����������⺯����10ms��һ��
* ���룺 ��
* ����� ��
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
				if(F_Power)  //������Ӧ���ܿ���
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