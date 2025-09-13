#ifndef __RGBLIGHT_H
#define __RGBLIGHT_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//RGB灯驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/5
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define RGB_DOUT PBout(11)  //P9813 DIN
#define RGB_COUT PBout(10)  //P9813 CIN

void RGBLIGHT_Init(void);
void RGB_FrameSend(u32 rgbvalue);
void RGB_Start(void);
void RGB_WriteData(u8 red,u8 green,u8 blue);
void RGB_SetColor(u8 red,u8 green,u8 blue);
void RGB_Open(void);
void RGB_Close(void);
#endif
