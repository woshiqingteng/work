#ifndef __SAI_H
#define __SAI_H
#include "sys.h"    									
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//SAI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/11
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									   
////////////////////////////////////////////////////////////////////////////////// 	
 

extern void (*sai_tx_callback)(void);		//sai tx�ص�����ָ��  

void SAIA_Init(u8 mode,u8 cpol,u8 datalen);
u8 SAIA_SampleRate_Set(u32 samplerate);
void SAIA_TX_DMA_Init(u8* buf0,u8 *buf1,u16 num,u8 width);
void SAI_Play_Start(void); 
void SAI_Play_Stop(void);  
#endif





















