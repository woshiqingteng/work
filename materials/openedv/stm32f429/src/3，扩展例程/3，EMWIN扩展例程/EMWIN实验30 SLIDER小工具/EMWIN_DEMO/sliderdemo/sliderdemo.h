#ifndef _GRAHPYT_H
#define _GRAHPYT_H
#include "sys.h"
#include "WM.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin SLIDER�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//slider������
typedef struct
{
    WM_HWIN slider;
    u16      slider_value[4]; //SLIDER����ֵ
    
}slider_structer;

void CreateSliderDemo(void); 
#endif
