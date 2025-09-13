#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32429������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:201/1/6
//�汾��V1.3
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 
extern TIM_OC_InitTypeDef TIM3_CH4Handler;  //��ʱ��3ͨ��4���
extern TIM_HandleTypeDef TIM5_Handler;      //��ʱ��5���
extern TIM_HandleTypeDef TIM6_Handler;      //��ʱ��6��� 

void TIM3_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM_SetCompare4(TIM_TypeDef *TIMx,u32 compare);
u32 TIM_GetCapture4(TIM_TypeDef *TIMx);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
void TIM6_Init(u16 arr,u16 psc);
#endif

