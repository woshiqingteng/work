#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "rng.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验17
 RNG(随机数发生器)实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
    u32 random;
    u8 t=0,key;
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART	
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
    LCD_Init();                     //初始化LCD
    
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"RNG TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/27");	
    while(RNG_Init())	 		    //初始化随机数发生器
	{
		LCD_ShowString(30,130,200,16,16,"  RNG Error! ");	 
		delay_ms(200);
		LCD_ShowString(30,130,200,16,16,"RNG Trying...");	 
	}                                 
	LCD_ShowString(30,130,200,16,16,"RNG Ready!   ");	 
	LCD_ShowString(30,150,200,16,16,"KEY0:Get Random Num");	 
	LCD_ShowString(30,180,200,16,16,"Random Num:");	 
	LCD_ShowString(30,210,200,16,16,"Random Num[0-9]:");    
    while(1)
    {
        delay_ms(10);
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			random=RNG_Get_RandomNum(); //获得随机数
			LCD_ShowNum(30+8*11,180,random,10,16); //显示随机数

		} 
		if((t%20)==0)
		{ 
			LED0=!LED0;	                    //每200ms,翻转一次LED0 
			random=RNG_Get_RandomRange(0,9);//获取[0,9]区间的随机数
			LCD_ShowNum(30+8*16,210,random,1,16); //显示随机数
		 }
		delay_ms(10);
		t++;
	}   
}
