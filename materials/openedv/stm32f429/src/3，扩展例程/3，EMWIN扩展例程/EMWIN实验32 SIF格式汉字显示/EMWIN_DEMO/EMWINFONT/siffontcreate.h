#ifndef _SIFFONTCREATE_H
#define _SIFFONTCREATE_H
#include "sys.h"
#include "GUI.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin SIF��ʽ������ʾ   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern GUI_FONT SIF12_Font;
extern GUI_FONT SIF16_Font;
extern GUI_FONT SIF24_Font;
extern GUI_FONT SIF36_Font;

void Create_SIF12(u8 *fxpath);
void Create_SIF16(u8 *fxpath);
void Create_SIF24(u8 *fxpath);
void Create_SIF36(u8 *fxpath);
#endif
