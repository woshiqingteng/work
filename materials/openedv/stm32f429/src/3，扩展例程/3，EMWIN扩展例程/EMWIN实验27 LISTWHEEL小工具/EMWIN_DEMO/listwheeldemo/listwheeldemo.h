#ifndef __LISTWHEELDEMO_H
#define __LISTWHEELDEMO_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin LISTWHEEL�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/24
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//���ڽṹ��
typedef struct
{
    char year[5];
    char month[20];
    char day[5];
}DateStruct;

void CreateListWheelDemo(void);
#endif
