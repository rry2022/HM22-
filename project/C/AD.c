#include "global.h"


/*-----------------------------------------------------------
* 函数：void adcDEAL(void)
* 功能：AD转换函数
* 输入： 无
* 输出： 无
------------------------------------------------------------*/
void adcDEAL(void) // Vector @  0x9B
{
	if(fad_ing)   
	{ // check ADC status // Get ADC value
		if(_EOC)
		{
   			//将ADM中的数据取出赋予adDATA数组
   			ADM=ADM & 0xDF;
   			adDATA = ADB<<2;
			  adDATA = adDATA |((ADR&0x0c)>>2);
   			fad_ing=0; 
   			if(adN==0)
   			{
    				adMAX=adDATA;
    				adMIN=adDATA;
    				adSUM=0;
   			}
   			else
   			{
    				//比较得出6组AD数据中的最大最小值
    				if(adDATA<adMIN)
    				{adMIN=adDATA;}
    				else if(adDATA>adMAX)
    				{adMAX=adDATA;}
   			}
   			adSUM=adSUM+adDATA;
   			if(adN<5)
   			{adN++;} 
   			else
   			{
    				adN=0;
    				adSUM=adSUM-adMAX;
    				adSUM=adSUM-adMIN;
    				adVAL=adSUM>>2;
    				switch(adcCH)
    				{
     					case 0:
     					{//0通道AD数据处理
      					adcCH = 1;
								AD_BATV = adVAL;   //电池电压
								if(AD_BATV >= 3)
								{
									AD_BATV -= 3;
								}
								VREFH = CLR_VREF | ADCRef2_5V;
    		 			}break;
     					case 1:
     					{//1通道AD数据处理
      					adcCH=2;
     						AD_USBV = adVAL;   //USB输入电压
								VREFH = CLR_VREF | ADCRef2_5V;
     					}break;
     					case 2:
     					{//2通道AD数据处理
      					adcCH=3;
      					AD_ChargeC = adVAL;//USB充电电流
								if(AD_ChargeC >= 4)
								{
									AD_ChargeC -= 4;
								}
								VREFH = CLR_VREF | ADCRef2_5V;
    		 			}break;
     					case 3:
     					{//3通道AD数据处理
      					adcCH=4;
     						AD_M1V = adVAL;    //振动马达工作电压
								VREFH = CLR_VREF | ADCRefVDD;
     					}break;
     					case 4:
     					{//4通道AD数据处理
      					adcCH=5;
      				  AD_NTCV = adVAL;  //NTC测温值
								VREFH = CLR_VREF | ADCRefVDD; 
    		 			}break;
     					case 5:
     					{//5通道AD数据处理
      					adcCH=6;
     						AD_M2V = adVAL;    //转动马达工作电压
								VREFH = CLR_VREF | ADCRefVDD;  
     					}break;
     					case 6:
     					{//6通道AD数据处理
      					adcCH=7;
      					AD_M2C = adVAL;    //转动马达工作电流
								VREFH = CLR_VREF | ADCRef2_5V; 
    		 			}break;
     					case 7:
     					{//7通道AD数据处理
      				  adcCH=0;
     						AD_M1C = adVAL;    //振动马达工作电流
								VREFH = CLR_VREF | ADCRef2_5V;
     					}break;
     					default :
     					break;
   				}
   			}
  		}     
 	}
 	if(fad_ing==0)
 	{
  		switch(adcCH)	//切换AD通道
  		{
   			case 0:
   			{
    				ADM = CLR_ADCCH |0x00;  // AIN0  ADC-3.7V
   			}break;
   			case 1:
   			{
    				ADM = CLR_ADCCH |0x02;  // AIN2  Detect-6V
   			}break;
   			case 2:
   			{
    				ADM = CLR_ADCCH |0x03;  // AIN3  ADC-CHARGEC
   			}break;
   			case 3:
   			{
    				ADM = CLR_ADCCH |0x04;  // AIN4  ADC-M1
   			}break;
   			case 4:
   			{
    				ADM = CLR_ADCCH |0x06;  // AIN6  NTC
   			}break;
   			case 5:
   			{
    				ADM = CLR_ADCCH |0x07;  // AIN7  ADC-M2
   			}break;
   			case 6:
   			{
    				ADM = CLR_ADCCH |0x08;  // AIN8  ADC-MTCTR2
   			}break;
   			case 7:
   			{
    				ADM = CLR_ADCCH |0x0b;  // AIN11 ADC-MTCTR
   			}break;
   			default:
   			{ 
    				ADM= CLR_ADCCH |0x00;  
    				adcCH=0;
   			}break;
  		}
  		ADC_START;
  		fad_ing=1;
 	}
}