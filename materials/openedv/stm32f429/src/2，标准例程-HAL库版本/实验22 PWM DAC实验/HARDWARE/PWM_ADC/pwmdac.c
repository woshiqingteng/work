#include "pwmdac.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//PWM DAC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


TIM_HandleTypeDef TIM9_Handler;         //��ʱ��9 PWM��� 
TIM_OC_InitTypeDef TIM9_CH2Handler;	    //��ʱ��9ͨ��2���

//PWM DAC��ʼ��(Ҳ����TIM9ͨ��2��ʼ��)
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM9_CH2_PWM_Init(u16 arr,u16 psc)
{ 
    TIM9_Handler.Instance=TIM9;            //��ʱ��9
    TIM9_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM9_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM9_Handler);       //��ʼ��PWM
    
    TIM9_CH2Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM9_CH2Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM9_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���Ϊ��
    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CH2Handler,TIM_CHANNEL_2);//����TIM9ͨ��2
	
    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_2);//����PWMͨ��2
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM9_CLK_ENABLE();			//ʹ�ܶ�ʱ��9
	__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_3;           	//PA3
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	GPIO_Initure.Alternate= GPIO_AF3_TIM9;	//PA3����ΪTIM9_CH2
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//����TIMͨ��2��ռ�ձ�
//TIM_TypeDef:��ʱ��
//compare:�Ƚ�ֵ
void TIM_SetTIM9Compare2(u32 compare)
{
	TIM9->CCR2=compare;
}

