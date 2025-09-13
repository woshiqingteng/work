#ifndef _GRAHPYT_H
#define _GRAHPYT_H
#include "sys.h"
#include "WM.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin SLIDER控件使用   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/3/26
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//slider参数集
typedef struct
{
    WM_HWIN slider;
    u16      slider_value[4]; //SLIDER滑块值
    
}slider_structer;

void CreateSliderDemo(void); 
#endif
