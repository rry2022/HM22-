#include "global.h"

/*-----------------------------------------------------------
* ������void Ctrl_Charge(void)
* ���ܣ������ƺ�����100ms��һ��
* ���룺 ��
* ����� ��
------------------------------------------------------------*/
void Ctrl_Charge(void)
{
	if(F_chadian && F_guoyaover && !F_guoya && !F_TempErro && !F_NTCErro && !F_ChargeErro)
	{
		if(Charge_Mode == Charge_NULL)
		{
			if(AD_BATV >= BAT_4V15) //��ص�ѹ����4.15V�����
			{
				F_ChargeSTA = 1;
			}	
		}	
		
		if(!F_ChargeSTA)  //�����
		{
			if(!F_TempAnom) //10-49��ʱ�ĳ��
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
					case Charge_S: //������ ��ص�ѹ0-3V��
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
						if(AD_BATV >= RealBat_3V)  //���ת����
						{
							Charge_Mode = Charge_M;
							PW2D = 0x01cd;
							Con_Deb = 0;
						}
					}break;
					case Charge_M: //������� ��ص�ѹ3-4V��
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
						if(AD_BATV >= RealBat_4V)  //����ת��ѹ
						{
							Charge_Mode = Charge_H;
							Con_ChargeVolt = RealBat_4V;
							Charge_cnt_9min = 0;
							ChargeFullDeb = 0;
							Dec_num = 0;
							if(AD_ChargeC <= Charge_790mA) //����δ��950mAת��ѹҪ���ں�ѹ��
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
					case Charge_H: //��ѹ��� ��ص�ѹ4.00V��
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
						//ÿ��9���Ӻ�ѹ���һ
						if(++Charge_cnt_9min >= 5400) 
						{
							Charge_cnt_9min = 0;
							Con_ChargeVolt--;
							Dec_num++;  //��ѹ����Ĵ���
						}
						
						if(AD_ChargeC < Charge_90mA) //��ֹ����80-100mA
						{
							ChargeFullDeb++;
							if(ChargeFullDeb >= 600)   //����һ����
							{
								ChargeFullDeb = 0;
								F_ChargeSTA = 1; 
							}
						}
                        else
						{
							if(ChargeFullDeb >= 10)
							{
								ChargeFullDeb -= 10; //ֵ�ϴ�һ�μ�10
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
			else  //0-10,49-54�¶��µ�������
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
			
			CHARGEON;       //�����
			
		}
		else                     //F_ChargeSTA = 1,������
		{
			CHARGEOFF;             //�س��
			if(AD_BATV <= BAT_3V9) //��ѹ��3.9V��������
			{
				BAT3V9Deb++;
				if(BAT3V9Deb >= 20)  //����
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
		CHARGEOFF; //ֹͣ���
	}
}

/*-----------------------------------------------------------
* ������void Ctrl_LED(void)
* ���ܣ�LED���ƺ�����125ms��һ��
* ���룺 ��
* ����� ��
------------------------------------------------------------*/
void Ctrl_LED(void)
{
	if(F_Power) //����
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
				F_Power = 0;  //�쳣��ʾ3S��ػ�
			}			
		}
		else
		{
			if(!F_diya)   //��ѹ3.3-4.2V  ���Ƴ���
			{
				LED_Mode = 2;
				LEDRed_OFF;
				LEDBlue_ON; //���Ƴ���
			}
			else          //��ѹ���2HZ��˸
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
	else        //�ػ�
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
				if(F_ChargeSTA || F_ChargeFull) //������
				{
					LED_Mode = 2;
					LEDRed_OFF;
					LEDBlue_ON; //���Ƴ���
				}
				else
				{    //������
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
							LEDBlueTurn; //��˸
						}
					}  //��������ѹ���
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
							LEDBlueTurn;  //��˸
						}						
					}  //��ѹ���---������
					if(Charge_Mode == Charge_H)
					{
						LED_Mode = 2;
						LEDBlue_OFF;
						LEDRed_ON; //��Ƴ���
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
* ������void Motor1_Ctrl(void)
* ���ܣ��񶯵�����ƺ�����100ms��һ��
* ���룺 ��
* ����� ��
------------------------------------------------------------*/
void Motor1_Ctrl(void)
{
	if(F_Power && !F_NTCErro && !F_TempHigh)
	{	
		switch(Motor1_Mode)
		{
			case Motor1_L:  //50%
			{
				MOTOR1ON; //�����
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
				MOTOR1ON;  //�����
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
				MOTOR1ON;  //�����
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
				MOTOR1OFF; //���񶯵��
			}break;			
		}
	}
	else
	{
		MOTOR1OFF; //���񶯵��
	}
}

///*-----------------------------------------------------------
//* ������void Motor2_Ctrl(void)
//* ���ܣ�ת��������ƺ�����100ms��һ��
//* ���룺 ��
//* ����� ��
//------------------------------------------------------------*/
//void Motor2_Ctrl(void)
//{
//	if(F_Power && !F_NTCErro && !F_TempHigh)
//	{
//		switch(Motor2_Mode)
//		{
//			case Motor2_L:   //80%����
//			{
//				if(F_Turn2)
//				{
//					F_Turn2 = 0;
//					PW2D = 0x00;
//					F_Direction = 1;
//				}
//				if(F_Direction)
//				{
//					MOTOR2ON_POS; //�����ת
//          if(++Motor2_30s >= 300)
//					{
//						Motor2_30s = 0;
//						F_Direction = 0;
//					}						
//				}
//				else
//				{
//					MOTOR2ON_NEG; //�����ת
//          if(++Motor2_30s >= 300)
//					{
//						Motor2_30s = 0;
//						F_Direction = 1;
//					}						
//				}
//			}break;
//			case Motor2_M:   //100%����
//			{
//				if(F_Turn2)
//				{
//					F_Turn2 = 0;
//					PW2D = 0x00;
//					F_Direction = 1;
//				}
//				if(F_Direction)
//				{
//					MOTOR2ON_POS; //�����ת
//          if(++Motor2_30s >= 300)
//					{
//						Motor2_30s = 0;
//						F_Direction = 0;
//					}						
//				}
//				else
//				{
//					MOTOR2ON_NEG; //�����ת
//          if(++Motor2_30s >= 300)
//					{
//						Motor2_30s = 0;
//						F_Direction = 1;
//					}						
//				}
//			}break;
//			case Motor2_H:   //�͸���
//			{
//				if(F_Turn2)
//				{
//					F_Turn2 = 0;
//					PW2D = 0x00;
//					F_Direction = 1;
//				}
//				if(F_Direction)
//				{
//					MOTOR2ON_POS; //�����ת
//          if(++Motor2_5s >= 50)
//					{
//						Motor2_5s = 0;
//						F_Direction = 0;
//						PW2D = 0x00;
//					}						
//				}
//				else
//				{
//					MOTOR2ON_NEG; //�����ת
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
//				MOTOR2OFF; //�ص��
//			}break;
//		}
//	}
//}