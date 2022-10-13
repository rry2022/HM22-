#include "global.h"

/*-----------------------------------------------------------
* 函数： void Scan_chadian(void)
* 功能： 插电检测函数,10ms执行一次,实时检测
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void Scan_chadian(void)
{
	if(!F_chadian)
	{
		if(USB_Port)
		{
			USBDeb++;
			if(USBDeb >= 5) //消抖50ms
			{
				USBDeb = 0;
				F_chadian = 1;
				F_guoyaover = 0;
				F_Power = 0;  //插电后关机
			}
		}
		else
		{
			USBDeb = 0;
		}
	}
	else
	{
		if(!USB_Port)
		{
			USBDeb++;
			if(USBDeb >= 5) //消抖50ms
			{
				USBDeb = 0;
				F_chadian = 0;
				F_ChargeSTA = 0;
				Charge_Mode = Charge_NULL;
				F_ChargeErro = 0;
				ChargeFullDeb = 0;
				Charge_cnt_9min = 0;
		    BAT3V9Deb = 0;
			}
		}
		else
		{
			USBDeb = 0;
		}
	}
}



/*-----------------------------------------------------------
* 函数： void Scan_guoya(void)
* 功能： 过压检测函数，插电后实时检测，100ms跑一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void Scan_guoya(void)
{
	if(F_chadian && !F_guoya)
	{
		if(AD_USBV >= USB_OverVolt)
		{
			guoyaDeb++;
			if(guoyaDeb >= 4) //消抖400ms
			{
				guoyaDeb = 0;
				F_guoyaover = 1;
				F_guoya = 1;
			}
			guoyaDeb1 = 0;
		}
		else
		{
			if(!F_guoyaover)
			{
				guoyaDeb1++;
				if(guoyaDeb1 >= 4) //消抖400ms
				{
					guoyaDeb1 = 0;
					F_guoyaover = 1;
					F_guoya = 0;
				}
		  }
			guoyaDeb = 0;
		}
  }
	else
	{
		guoyaDeb = 0;
		guoyaDeb1 = 0;
		F_guoya = 0;  //拔掉USB后过压标志位清零
	}
}


/*-----------------------------------------------------------
* 函数： void Scan_Temp(void)
* 功能： 温度检测函数,100ms跑一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void Scan_Temp(void)
{ 	
	if(!F_NTCErro)
	{
		//NTC开短路检测
		if((AD_NTCV <= NTC_ErroL) || (AD_NTCV >= NTC_ErroH))
		{
			NTCErroDeb++;
			if(NTCErroDeb >= 30)  //消抖3S
			{
				NTCErroDeb = 0;
				F_NTCErro = 1;      //不清零
				F_Power = 0;
			}
		}
		else
		{
			if(NTCErroDeb)
			{
				NTCErroDeb--;
			}
		}	

		//测温
		if(Temp_Level == 0)
		{
			if(AD_NTCV <= NTC_Temp0C)
			{
				Temp_Level = 1;
			}
			if(((AD_NTCV > NTC_Temp0C) && (AD_NTCV <= NTC_Temp10C)))
			{
				Temp_Level = 2;
			}
			else if(((AD_NTCV > NTC_Temp10C) && (AD_NTCV <= NTC_Temp49C)))
			{
				Temp_Level = 3;
			}
			else if(((AD_NTCV > NTC_Temp49C) && (AD_NTCV <= NTC_Temp54C)))
			{
				Temp_Level = 4;
			}
			else if(((AD_NTCV > NTC_Temp54C) && (AD_NTCV <= NTC_Temp60C)))
			{
				Temp_Level = 5;
			}
			else if(AD_NTCV > NTC_Temp60C)
			{
				Temp_Level = 6;
			}
		}
		else
		{
			if(TempVal <= (NTC_Temp0C-32))
			{
				Temp_Level = 1;
			}
			if(((TempVal > (NTC_Temp0C+32)) && (TempVal <= (NTC_Temp10C))))
			{
				Temp_Level = 2;
			}
			else if(((TempVal > (NTC_Temp10C+32)) && (TempVal <= (NTC_Temp49C-32))))
			{
				Temp_Level = 3;
			}
			else if(((TempVal > (NTC_Temp49C+32)) && (TempVal <= (NTC_Temp54C-32))))
			{
				Temp_Level = 4;
			}
			else if(((TempVal > (NTC_Temp54C)) && (TempVal <= (NTC_Temp60C-32))))
			{
				Temp_Level = 5;
			}
			else if(TempVal > (NTC_Temp60C+32))
			{
				Temp_Level = 6;
			}
		}
		
		if(AD_NTCV != TempVal)
		{
			if(++TempDeb >= 4) //消抖400ms
			{
				TempDeb = 0;
				TempVal = AD_NTCV;
			}
		}
		else
		{
			TempDeb = 0;
		}
		
		//根据不同温度设标志位
		if(Temp_Level==0 || Temp_Level==5 || Temp_Level==6)
		{
			F_TempErro = 1;
			if(Temp_Level==6)
			{
				F_TempHigh = 1;
			}
			else
			{
				F_TempHigh = 0;
			}
		}
		else
		{
			F_TempErro = 0;
			F_TempHigh = 0;
			if(Temp_Level==4 || Temp_Level==2)
			{
				F_TempAnom = 1;
			}
			if(Temp_Level==3)
			{
				F_TempAnom = 0;
			}
	  }
  }
}




/*-----------------------------------------------------------
* 函数： void Scan_BATVol(void)
* 功能： 电池电压检测函数,100ms跑一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void Scan_BATVol(void)
{
	if(F_chadian)
	{
		//电池过压4.15V保护
		if((AD_BATV >= RealBat_4V15) && (AD_ChargeC <= Charge_250mA)) 
		{
			BATDeb1++;
			if(BATDeb1 >= 5)    //消抖500ms
			{
				BATDeb1 = 0;
				F_ChargeSTA = 1;
			}
		}
		else
		{
			BATDeb1 = 0;
		}
	}
	else
	{
		BATDeb1 = 0;
		
		//电池低压检测
		if(!F_diya)
		{
			if((AD_BATV >= BAT_3V) && (AD_BATV <= BAT_3V3))
			{
				if(++BATDeb2 >= 10)  //消抖1s
				{
					BATDeb2 = 0;
					F_diya = 1;
				}
			}
			else
			{
				if(BATDeb2)
				{
					BATDeb2--;
				}
			}
		}
		else
		{
			if(AD_BATV >= BAT_3V3 + 55)
			{
				if(++BATDeb2 >= 10)  //消抖1s
				{
					BATDeb2 = 0;
					F_diya = 0;
				}
			}
			else
			{
				if(BATDeb2)
				{
					BATDeb2--;
				}
			}
		}
		
		//亏电检测
		if(!F_kuidian)
		{
			if(AD_BATV <= BAT_3V)
			{
				if(++BATDeb3 >= 10) //消抖1s
				{
					BATDeb3 = 0;
					F_kuidian = 1;
					F_Power = 0;
				}
			}
			else
			{
				if(BATDeb3)
				{
					BATDeb3 = 0;
				}
			}
		}
		
	}	
}

///*-----------------------------------------------------------
//* 函数： void Motor1_guozai(void)
//* 功能： 振动电机过载检测函数,100ms跑一次
//* 输入： 无
//* 输出： 无
//------------------------------------------------------------*/
//void Motor1_guozai(void)
//{
//	
//}
