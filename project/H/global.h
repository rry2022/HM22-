#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include <SN8F5762.h>


#ifdef MyDef
#define EXTERN    //定义变量
#else
#define EXTERN extern  //声明变量
#endif

#define uchar unsigned char
#define uint unsigned int
  
//=====================================================================================================
//数字宏定义
//AD.c
#define ADCRefVDD   0x04
#define ADCRef4_5V  0x02
#define ADCRef3_5V  0x01
#define ADCRef2_5V  0x00
#define CLR_VREF    (VREFH & 0xf8)     // clear VHS[2:0]
#define _EOC        ADM & 0x20         // ADC status (EOC)
#define ADC_START   ADM |= 0x40        // ADC converting start (ADS)
#define CLR_ADCCH   (ADM & 0xB0)       // clear CHS[3:0]
//test.c
#define USB_OverVolt 509   //USB过压边界值

#define NTC_ErroL    5     //NTC短路对应的AD值
#define NTC_ErroH    1020   //NTC开路对应的AD值
#define NTC_Temp0C   117    //0度对应的AD值
#define NTC_Temp10C  179    //10度对应的AD值
#define NTC_Temp49C  547    //49度对应的AD值
#define NTC_Temp54C  595    //54度对应的AD值
#define NTC_Temp60C  648   //60度对应的AD值

#define BAT_0V       0      
#define BAT_3V       415
#define BAT_3V3      456
#define BAT_3V6      498
#define BAT_3V9      539
#define BAT_4V       553
#define BAT_4V15       574
#define BAT_4V2      581   //未充电时的电池电压

#define RealBat_3V   423    //已实测
#define RealBat_4V   586    //已实测
#define RealBat_4V1  573   //已实测
#define RealBat_4V15 592   //恒压点上加6个AD值
//ctr.c
#define Charge_90mA  7-4
#define Charge_250mA 20
#define Charge_430mA 34
#define Charge_520mA 42
#define Charge_650mA 52
#define Charge_790mA 63
#define Charge_950mA 76

#define M1_3V        415
#define M1_2V25      311
#define M1_1V5       207

#define BAT_L        1
#define BAT_M        2
#define BAT_H        3 
#define Charge_NULL  0
#define Charge_S     1
#define Charge_M     2
#define Charge_H     3
#define Charge_Other 4
#define Motor1_L     1
#define Motor1_M     2
#define Motor1_H     3
#define Motor2_L     1
#define Motor2_M     2
#define Motor2_H     3
//=====================================================================================================
//IO口宏定义
#define USB_Port    P00
//#define Test_Port   P15
#define UART_TX     P15
#define K1          P01  //总开关
#define K2          P03  //振动马达档位切换开关
#define K3          P04  //转动马达档位切换开关
//IO口操作宏定义
//#define NTCOFF      P20=0
//#define NTCON       P20=1
#define LEDBlue_ON   P03=0
#define LEDBlue_OFF  P03=1
#define LEDBlueTurn  P03=~P03
#define LEDRed_ON    P04=0
#define LEDRed_OFF   P04=1
#define LEDRedTurn   P04=~P04
//=====================================================================================================
//寄存器操作宏定义
#define CHARGEON    PW2CH=0x04;PW2M|=0x80
#define CHARGEOFF   PW2CH&=0xfb;PW2M&=0x7f
#define MOTOR1ON    PW1M|=0x80
#define MOTOR1OFF   PW1M&=0x7f
#define MOTOR2ON_POS    PW2CH=0x10;PW2M|=0x80
#define MOTOR2ON_NEG    PW2CH=0x20;PW2M|=0x80
#define MOTOR2OFF   PW2CH&=0xcf;PW2M&=0x7f 
//=====================================================================================================
//main.c
EXTERN bit idata Ft_2ms;
EXTERN unsigned char idata cnt_10ms, cnt_100ms, cnt_125ms, cnt_500ms;
EXTERN unsigned int cnt_4s;
//AD.c
EXTERN unsigned char idata adcCH, adN;
EXTERN unsigned int idata adDATA, adMAX, adMIN, adVAL, adSUM, AD_NTCV, AD_BATV, AD_USBV, AD_M1V, AD_M2V,
	AD_M1C, AD_M2C, AD_ChargeC; 
EXTERN bit idata fad_ing;
//test.c
EXTERN unsigned char idata USBDeb, guoyaDeb, guoyaDeb1, TempErroDeb, NTCErroDeb, TempAnomDeb, TempnomDeb,
	TempHighDeb, BATDeb, BATDeb1, TempDeb, Temp_Level, BATV_Level, BATDeb2, BATDeb3;
EXTERN unsigned int idata BATVal, BATValBuff, TempVal; 
EXTERN bit idata F_kuidian, F_diya, F_chadian, F_guoyaover, F_guoya, F_NTCErro, F_TempErro, F_TempAnom, F_TempHigh;
//ctr.c
EXTERN unsigned char idata Con_Deb, BAT3V9Deb, Charge_Mode, LED_Mode, LEDDeb, cnt_3s, Motor1_Mode, Motor2_Mode, Dec_num;
EXTERN unsigned int idata ChargeFullDeb, Con_ChargeVolt, Charge_cnt_9min;
EXTERN bit idata F_ChargeSTA, F_Power;
//time.c
EXTERN bit idata F_ChargeErro, F_ChargeFull;
EXTERN unsigned int idata Charge_time, cnt_1hLowVol, cnt_1hHightemp;
//key.c
EXTERN unsigned char idata keyBUF,keyNEW,DEB,temp, cntK1LONG;
EXTERN bit idata fk1DOWN, fk1LONG, F_Turn1;
#endif