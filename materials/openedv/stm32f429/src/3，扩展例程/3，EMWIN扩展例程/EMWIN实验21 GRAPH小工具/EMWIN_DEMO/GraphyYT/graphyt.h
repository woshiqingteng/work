#ifndef _GRAHPYT_H
#define _GRAHPYT_H
#include "sys.h"
#include "GRAPH.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin GRAPH�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/19
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
typedef struct 
{
	int *buffer;				        //AD���ݴ����
	u8 	adflag;					        //AD�ɼ��Ƿ���ɱ�־
	int dataxsize,dataysize;	        //ʾ����������X,Y��С
	u8 	linecolorindex,backcolorindex;	//������ɫ����ֵ�ͱ�����ɫ����ֵ
	GUI_COLOR linecolor,backcolor;		//������ɫ�ͱ�����ɫ
	GRAPH_DATA_Handle  graphdata; 	    //GRAHP_DATA�����ݾ��
	int div;					        //һ�������
    float resolut;                      //ÿ��ADԭʼֵ������ĵ�ѹ(��λΪmv)
}_oscill_dev;

extern _oscill_dev oscilldev;

void Graphyt_demo(void); 
#endif
