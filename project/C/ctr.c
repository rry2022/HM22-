#include "global.h"

/*-----------------------------------------------------------
* 函数：void Ctrl_Charge(void)
* 功能：充电控制函数，100ms跑一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void Ctrl_Charge(void)
{
	if(F_chadian && F_guoyaover && !F_guoya && !F_TempErro && !F_NTCErro && !F_ChargeErro)
	{
		if(Charge_Mode == Charge_NULL)
		{
			if(AD_BATV >= BAT_4V15) //电池电压大于4.15V不充电
			{
				F_ChargeSTA = 1;
			}	
		}	
		
		if(!F_ChargeSTA)  //充电中
		{
			if(!F_TempAnom) //10-49度时的充电
			{
				if(Charge_Mode == Charge_NULL)
				{
					if(AD_BATV && (AD_BATV < BAT_3V))
					{
						Charge_Mode = Charge_S;
						PW2D = 0x0042;
					}
					else if((AD_BATV >= BAT_3V) && (AD_BATV < BAT_4V15))
					{
						Charge_Mode = Charge_M;
						PW2D = 0x01cd;
					}
				}
				else if(Charge_Mode == Charge_Other)
				{
					Charge_Mode = Charge_S;
					PW2D = 0x0042;
				}

				
				switch(Charge_Mode)
				{
					case Charge_S: //涓流充电 电池电压0-3V进
					{
						if(AD_ChargeC > (Charge_250mA + 4) )
						{
							if(PW2D)
							{
								PW2D--;
							}
						}
						else if(AD_ChargeC < (Charge_250mA - 4))
						{
							if(PW2D < 1020)
							{
								PW2D++;
							}
						}
						if(AD_BATV >= RealBat_3V)  //涓流转恒流
						{
							Charge_Mode = Charge_M;
							PW2D = 0x01cd;
							Con_Deb = 0;
						}
					}break;
					case Charge_M: //恒流充电 电池电压3-4V进
					{
						if(AD_ChargeC > Charge_950mA)
						{
							if(PW2D)
							{
								PW2D--;
							}
						}
						else if(AD_ChargeC < (Charge_950mA - 4))
						{
							if(PW2D < 1020)
							{
								PW2D++;
							}
						}
						if(AD_BATV >= RealBat_4V)  //恒流转恒压
						{
							Charge_Mode = Charge_H;
							Con_ChargeVolt = RealBat_4V;
							Charge_cnt_9min = 0;
							ChargeFullDeb = 0;
							Dec_num = 0;
							if(AD_ChargeC <= Charge_790mA) //电流未到950mA转恒压要调节恒压点
							{
								Con_ChargeVolt--;
							}
							if(AD_ChargeC <= Charge_650mA)
							{
								Con_ChargeVolt--;
							}
							if(AD_ChargeC <= Charge_520mA)
							{
								Con_ChargeVolt--;
							}
							if(AD_ChargeC <= Charge_430mA)
							{
								Con_ChargeVolt--;
							}
						}
					}break;
					case Charge_H: //恒压充电 电池电压4.00V进
					{
						if(AD_BATV > Con_ChargeVolt)
						{
							if(PW2D)
							{
								PW2D--;
							}
						}
						else if(AD_BATV < (Con_ChargeVolt-20))
						{
							if(PW2D < 1020)
							{
								PW2D++;
							}
						}
						//每隔9分钟恒压点减一
						if(++Charge_cnt_9min >= 5400) 
						{
							Charge_cnt_9min = 0;
							Con_ChargeVolt--;
							Dec_num++;  //恒压点减的次数
						}
						
						if(AD_ChargeC < Charge_90mA) //截止电流80-100mA
						{
							ChargeFullDeb++;
							if(ChargeFullDeb >= 600)   //消抖一分钟
							{
								ChargeFullDeb = 0;
								F_ChargeSTA = 1; 
							}
						}
                        else
						{
							if(ChargeFullDeb >= 10)
							{
								ChargeFullDeb -= 10; //值较大一次减10
							}
							else
							{
								ChargeFullDeb = 0;
							}
						}
					}break;
					default: break;
				}
			}
			else  //0-10,49-54温度下的涓流充电
			{
				Charge_Mode = Charge_Other;
				if(AD_ChargeC > (Charge_250mA + 4))
				{
					if(PW2D)
					{
						PW2D--;
					}
				}
				else if(AD_ChargeC < (Charge_250mA - 4))
				{
					if(PW2D < 1020)
					{
						PW2D++;
					}
				}
			}
			
			CHARGEON;       //开充电
			
		}
		else                     //F_ChargeSTA = 1,充满电
		{
			CHARGEOFF;             //关充电
			if(AD_BATV <= BAT_3V9) //电压降3.9V后继续充电
			{
				BAT3V9Deb++;
				if(BAT3V9Deb >= 20)  //消抖
				{
					BAT3V9Deb = 0;
					F_ChargeSTA = 0;
					Charge_Mode = Charge_NULL;
				}
			}
			else
			{
				BAT3V9Deb = 0;
			}
		}
	}
	else
	{
		CHARGEOFF; //停止充电
	}
}

/*-----------------------------------------------------------
* 函数：void Ctrl_LED(void)
* 功能：LED控制函数，125ms跑一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void Ctrl_LED(void)
{
	if(F_Power) //开机
	{
		if(F_NTCErro || F_TempHigh)
		{
			LEDBlue_OFF;  //4HZ
			LEDRedTurn;
			if(LED_Mode != 1)
			{
				LED_Mode = 1;
				cnt_3s = 0;
			}			
			if(++cnt_3s >= 24)
			{
				cnt_3s = 0;
				F_Power = 0;  //异常显示3S后关机
			}			
		}
		else
		{
			if(!F_diya)   //电压3.3-4.2V  蓝灯长亮
			{
				LED_Mode = 2;
				LEDRed_OFF;
				LEDBlue_ON; //蓝灯常亮
			}
			else          //低压红灯2HZ闪烁
			{
				if(LED_Mode != 3)
				{
					LED_Mode = 3;
					LEDDeb = 0;
				}
				if(++LEDDeb >= 2)
				{
					LEDDeb = 0;
					LEDBlue_OFF;
					LEDRedTurn;
				}					
			}
		}
	}
	else        //关机
	{
		if(F_guoya || F_NTCErro || F_TempErro || F_ChargeErro)
		{
			LEDBlue_OFF;
			LEDRedTurn;
			if(LED_Mode != 1)
			{
				LED_Mode = 1;
				cnt_3s = 0;
			}
			if(++cnt_3s >= 24)
			{
				cnt_3s = 0;
			}			
		}
		else
		{
			if(F_chadian)
			{
				if(F_ChargeSTA || F_ChargeFull) //充满电
				{
					LED_Mode = 2;
					LEDRed_OFF;
					LEDBlue_ON; //蓝灯常亮
				}
				else
				{    //涓流充电
					if((Charge_Mode == Charge_S) || (Charge_Mode == Charge_Other)) 
					{
						if(LED_Mode != 4)
						{
							LED_Mode = 4;
							LEDDeb = 0;
						}					
						if(++LEDDeb >= 16) //0.5HZ
						{
							LEDDeb = 0;
							LEDRed_OFF;
							LEDBlueTurn; //闪烁
						}
					}  //恒流、恒压充电
					if(Charge_Mode == Charge_M)
					{
						if(LED_Mode != 5)
						{
							LED_Mode = 5;
							LEDDeb = 0;
						}
						if(++LEDDeb >= 4) //1HZ
						{
							LEDDeb = 0;
							LEDRed_OFF;
							LEDBlueTurn;  //闪烁
						}						
					}  //恒压充电---测试用
					if(Charge_Mode == Charge_H)
					{
						LED_Mode = 2;
						LEDBlue_OFF;
						LEDRed_ON; //红灯常亮
					}
				}
			}
			else
			{
				LED_Mode = 0;
				LEDBlue_OFF;
				LEDRed_OFF;
			}
		}
	}
}


/*-----------------------------------------------------------
* 函数：void Motor1_Ctrl(void)
* 功能：振动电机控制函数，100ms跑一次
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void Motor1_Ctrl(void)
{
	if(F_Power && !F_NTCErro && !F_TempHigh)
	{	
		switch(Motor1_Mode)
		{
			case Motor1_L:  //50%
			{
				MOTOR1ON; //开马达
				if(F_Turn1)
				{
					F_Turn1 = 0;
					if((AD_BATV > BAT_3V) && (AD_BATV <= BAT_3V3))
					{
						PW12D = 381;
					}
					else if((AD_BATV > BAT_3V3) && (AD_BATV <= BAT_3V6))
					{
						PW12D = 348;
					}
					else if((AD_BATV > BAT_3V6) && (AD_BATV <= BAT_3V9))
					{
						PW12D = 320;
					}
					else if((AD_BATV > BAT_3V9) && (AD_BATV <= BAT_4V2))
					{
						PW12D = 296;
					}
				}
				if((AD_BATV - AD_M1V) <= M1_1V5)
				{
					if(PW12D < 790)
					{
//						PW12D++;
						PW12D += 2;
					}
				}
				else
				{
//					if(PW12D)
//					{
//						PW12D--;
//					}
					if(PW12D > 2)
					{
						PW12D -= 2;
					}
				}
			}break;
			case Motor1_M:  //75%
			{
				MOTOR1ON;  //开马达
				if(F_Turn1)
				{
					F_Turn1 = 0;
					if((AD_BATV > BAT_3V) && (AD_BATV <= BAT_3V3))
					{
						PW12D = 571;
					}
					else if((AD_BATV > BAT_3V3) && (AD_BATV <= BAT_3V6))
					{
						PW12D = 522;
					}
					else if((AD_BATV > BAT_3V6) && (AD_BATV <= BAT_3V9))
					{
						PW12D = 480;
					}
					else if((AD_BATV > BAT_3V9) && (AD_BATV <= BAT_4V2))
					{
						PW12D = 444;
					}
				}
				if((AD_BATV - AD_M1V) <= M1_2V25)
				{
					if(PW12D < 790)
					{
//						PW12D++;
						PW12D += 2;
					}
				}
				else
				{
//					if(PW12D > 2)
//					{
//						PW12D--;
//					}
					if(PW12D > 2)
					{
						PW12D -= 2;
					}
				}
			}break;
			case Motor1_H:  //100%
			{
				MOTOR1ON;  //开马达
				if(F_Turn1)
				{
					F_Turn1 = 0;
					if((AD_BATV > BAT_3V) && (AD_BATV <= BAT_3V3))
					{
						PW12D = 762;
					}
					else if((AD_BATV > BAT_3V3) && (AD_BATV <= BAT_3V6))
					{
						PW12D = 696;
					}
					else if((AD_BATV > BAT_3V6) && (AD_BATV <= BAT_3V9))
					{
						PW12D = 640;
					}
					else if(AD_BATV > BAT_3V9)
					{
						PW12D = 593;
					}
				}
				if((AD_BATV - AD_M1V) <= M1_3V)
				{
					if(PW12D < 790)
					{
//						PW12D++;
						PW12D += 2;
					}
				}
				else
				{
//					if(PW12D > 2)
//					{
//						PW12D--;
//					}
					if(PW12D > 2)
					{
						PW12D -= 2;
					}
				}
			}break;
      default: 
			{
				MOTOR1OFF; //关振动电机
			}break;			
		}
	}
	else
	{
		MOTOR1OFF; //关振动电机
	}
}

///*-----------------------------------------------------------
//* 函数：void Motor2_Ctrl(void)
//* 功能：转动电机控制函数，100ms跑一次
//* 输入： 无
//* 输出： 无
//------------------------------------------------------------*/
//void Motor2_Ctrl(void)
//{
//	if(F_Power && !F_NTCErro && !F_TempHigh)
//	{
//		switch(Motor2_Mode)
//		{
//			case Motor2_L:   //80%低速
//			{
//				if(F_Turn2)
//				{
//					F_Turn2 = 0;
//					PW2D = 0x00;
//					F_Direction = 1;
//				}
//				if(F_Direction)
//				{
//					MOTOR2ON_POS; //电机正转
//          if(++Motor2_30s >= 300)
//					{
//						Motor2_30s = 0;
//						F_Direction = 0;
//					}						
//				}
//				else
//				{
//					MOTOR2ON_NEG; //电机反转
//          if(++Motor2_30s >= 300)
//					{
//						Motor2_30s = 0;
//						F_Direction = 1;
//					}						
//				}
//			}break;
//			case Motor2_M:   //100%高速
//			{
//				if(F_Turn2)
//				{
//					F_Turn2 = 0;
//					PW2D = 0x00;
//					F_Direction = 1;
//				}
//				if(F_Direction)
//				{
//					MOTOR2ON_POS; //电机正转
//          if(++Motor2_30s >= 300)
//					{
//						Motor2_30s = 0;
//						F_Direction = 0;
//					}						
//				}
//				else
//				{
//					MOTOR2ON_NEG; //电机反转
//          if(++Motor2_30s >= 300)
//					{
//						Motor2_30s = 0;
//						F_Direction = 1;
//					}						
//				}
//			}break;
//			case Motor2_H:   //低高速
//			{
//				if(F_Turn2)
//				{
//					F_Turn2 = 0;
//					PW2D = 0x00;
//					F_Direction = 1;
//				}
//				if(F_Direction)
//				{
//					MOTOR2ON_POS; //电机正转
//          if(++Motor2_5s >= 50)
//					{
//						Motor2_5s = 0;
//						F_Direction = 0;
//						PW2D = 0x00;
//					}						
//				}
//				else
//				{
//					MOTOR2ON_NEG; //电机反转
//          if(++Motor2_5s >= 50)
//					{
//						Motor2_5s = 0;
//						F_Direction = 1;
//						PW2D = 0x00;
//					}						
//				}
//			}break;
//			default:
//			{
//				MOTOR2OFF; //关电机
//			}break;
//		}
//	}
//}