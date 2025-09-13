#ifndef __LISTVIEWDEMO_H
#define __LISTVIEWDEMO_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin LISTVIEW控件使用   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/3/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//学生信息结构体
typedef struct
{
    u8 name[20];    //名字
    u8 num[20];     //学号
    u8 score[4];    //分数
    u8 grade;       //等级
}student_info;

void CreateListviewdemo(void); 
#endif
