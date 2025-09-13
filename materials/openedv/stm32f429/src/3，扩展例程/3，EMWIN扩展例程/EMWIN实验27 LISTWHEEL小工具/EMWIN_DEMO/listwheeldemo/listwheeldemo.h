#ifndef __LISTWHEELDEMO_H
#define __LISTWHEELDEMO_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin LISTWHEEL控件使用   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/3/24
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//日期结构体
typedef struct
{
    char year[5];
    char month[20];
    char day[5];
}DateStruct;

void CreateListWheelDemo(void);
#endif
