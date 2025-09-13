#ifndef __RGBCONTROL_H
#define __RGBCONTROL_H
#include "sys.h"
#include "ff.h"
#include "DIALOG.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin RGB彩灯控制 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//RGB灯结构体
typedef struct
{
    u8 red;                 //红色分量
    u8 green;               //绿色分量
    u8 blue;                //蓝色分量
    u8 *coloroop_buffer;    //颜色环图像缓冲区
    u8 image_flag;          //判断鼠标是否在IMAGE控件上,0 不在，1 在
    FIL ColorLoopFile;      //颜色环图片文件
    GUI_PID_STATE PidData;  //鼠标位置
    u32 colorloopsize;      //颜色环图片大小
    u8 brightness;          //亮度系数，0~100,对应0%~100%
    WM_HWIN AppWin;         //APP所使用的窗口
}rgb_struct;

void RGBLight_DrawCircle(u16 x,u16 y);
float CalculatteDistance(float x0,float y0,float x1,float y1);
void CreateRGBControlDemo(void);
#endif
