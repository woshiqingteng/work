#include "rgblight.h"
#include "delay.h"
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

//RGB�Ƴ�ʼ��
void RGBLIGHT_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();               //����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;   //PB10,11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //�������
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);	//PB11��1 
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);	//PB10��1 
    RGB_SetColor(0,0,0);                               //Ĭ�Ϲر�
}

//��������֡
//data:Ҫ���͵�����ֵ
void RGB_FrameSend(u32 rgbvalue)
{
    u8 i=0;
    u32 data=rgbvalue;
    for(i=0;i<32;i++)
    {
        RGB_COUT=0;
        delay_us(2);
        if(data&0X80000000) RGB_DOUT=1;
        else RGB_DOUT=0;
        delay_us(2);
        RGB_COUT=1;     //������д������
        delay_us(2);
        data=data<<1;   //����һλ
    }
}

//��ʼ�źţ���������32λ0
void RGB_Start(void)
{
    u32 start=0X00000000;
    RGB_FrameSend(start);
}

//������ɫֵ����
//red:��ɫ�Ҷ�ֵ��0~255
//green:��ɫ�Ҷ�ֵ,0~255
//blue:��ɫ�Ҷ�ֵ��0~255
void RGB_WriteData(u8 red,u8 green,u8 blue)
{
    u8 check_byte=0XC0;//У��ֵ�������λΪ11��Ϊ��־λ
    u32 data=0;
    
    check_byte|=(((~blue)>>2)&0x30);    // B7,B6
    check_byte|=(((~green)>>4)&0x0C);   // G7,G6
    check_byte|=(((~red)>>6)&0x03);     // R7,R6
    
    data=(check_byte<<24)|(blue<<16)|(green<<8)|red;//�齨����֡
    RGB_FrameSend(data);                //��������֡
}

//����RGB�Ƶ���ɫֵ
//red:��ɫ�Ҷ�ֵ��0~255
//green:��ɫ�Ҷ�ֵ,0~255
//blue:��ɫ�Ҷ�ֵ��0~255
void RGB_SetColor(u8 red,u8 green,u8 blue)
{
    RGB_Start();
    RGB_WriteData(red,green,blue);
    RGB_Start();
}

//��RGB�ƣ���ɫ��50%����
void RGB_Open(void)
{
    RGBLIGHT_Init();
    RGB_SetColor(128,128,128);
}

//�ر�RGB��
void RGB_Close(void)
{
    RGB_SetColor(0,0,0);
    HAL_GPIO_DeInit(GPIOB,GPIO_PIN_10);//��λPB10
    HAL_GPIO_DeInit(GPIOB,GPIO_PIN_11);//��λPB11
}
