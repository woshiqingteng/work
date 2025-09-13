#ifndef _SDRAM_H
#define _SDRAM_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//SDRAM��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/9
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//����˵��
//V1.1 20180724
//1,�޸�CAS�ӳ�Ϊ2������߷����ٶ�
//2,�޸�SDRAMͻ�����ʳ���Ϊ1,����NAND ��������������
////////////////////////////////////////////////////////////////////////////////// 	

#define Bank5_SDRAM_ADDR    ((u32)(0XC0000000)) 			//SDRAM��ʼ��ַ
 
u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval);
void SDRAM_Init(void);
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);
#endif



































