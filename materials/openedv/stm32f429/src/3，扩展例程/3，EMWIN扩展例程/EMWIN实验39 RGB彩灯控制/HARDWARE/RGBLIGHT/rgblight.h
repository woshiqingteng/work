#ifndef __RGBLIGHT_H
#define __RGBLIGHT_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//RGB����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
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
