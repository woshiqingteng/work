#ifndef __RGBCONTROL_H
#define __RGBCONTROL_H
#include "sys.h"
#include "ff.h"
#include "DIALOG.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin RGB�ʵƿ��� 
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//RGB�ƽṹ��
typedef struct
{
    u8 red;                 //��ɫ����
    u8 green;               //��ɫ����
    u8 blue;                //��ɫ����
    u8 *coloroop_buffer;    //��ɫ��ͼ�񻺳���
    u8 image_flag;          //�ж�����Ƿ���IMAGE�ؼ���,0 ���ڣ�1 ��
    FIL ColorLoopFile;      //��ɫ��ͼƬ�ļ�
    GUI_PID_STATE PidData;  //���λ��
    u32 colorloopsize;      //��ɫ��ͼƬ��С
    u8 brightness;          //����ϵ����0~100,��Ӧ0%~100%
    WM_HWIN AppWin;         //APP��ʹ�õĴ���
}rgb_struct;

void RGBLight_DrawCircle(u16 x,u16 y);
float CalculatteDistance(float x0,float y0,float x1,float y1);
void CreateRGBControlDemo(void);
#endif
