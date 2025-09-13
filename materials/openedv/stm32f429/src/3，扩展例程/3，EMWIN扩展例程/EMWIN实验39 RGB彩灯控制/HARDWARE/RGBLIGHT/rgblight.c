#include "rgblight.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//RGB灯驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/5
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//RGB灯初始化
void RGBLIGHT_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();               //开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;   //PB10,11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);	//PB11置1 
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);	//PB10置1 
    RGB_SetColor(0,0,0);                               //默认关闭
}

//发生数据帧
//data:要发送的数据值
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
        RGB_COUT=1;     //上升沿写入数据
        delay_us(2);
        data=data<<1;   //左移一位
    }
}

//开始信号，连续发送32位0
void RGB_Start(void)
{
    u32 start=0X00000000;
    RGB_FrameSend(start);
}

//发送颜色值数据
//red:红色灰度值，0~255
//green:绿色灰度值,0~255
//blue:蓝色灰度值，0~255
void RGB_WriteData(u8 red,u8 green,u8 blue)
{
    u8 check_byte=0XC0;//校验值，最高两位为11，为标志位
    u32 data=0;
    
    check_byte|=(((~blue)>>2)&0x30);    // B7,B6
    check_byte|=(((~green)>>4)&0x0C);   // G7,G6
    check_byte|=(((~red)>>6)&0x03);     // R7,R6
    
    data=(check_byte<<24)|(blue<<16)|(green<<8)|red;//组建数据帧
    RGB_FrameSend(data);                //发送数据帧
}

//设置RGB灯的颜色值
//red:红色灰度值，0~255
//green:绿色灰度值,0~255
//blue:蓝色灰度值，0~255
void RGB_SetColor(u8 red,u8 green,u8 blue)
{
    RGB_Start();
    RGB_WriteData(red,green,blue);
    RGB_Start();
}

//打开RGB灯，白色，50%亮度
void RGB_Open(void)
{
    RGBLIGHT_Init();
    RGB_SetColor(128,128,128);
}

//关闭RGB灯
void RGB_Close(void)
{
    RGB_SetColor(0,0,0);
    HAL_GPIO_DeInit(GPIOB,GPIO_PIN_10);//复位PB10
    HAL_GPIO_DeInit(GPIOB,GPIO_PIN_11);//复位PB11
}
