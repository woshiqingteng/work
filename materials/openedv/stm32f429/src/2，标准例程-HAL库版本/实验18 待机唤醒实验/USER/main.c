#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "wkup.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验18
 待机唤醒实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
    WKUP_Init();	                //待机唤醒初始化
    SDRAM_Init();                   //初始化SDRAM
    LCD_Init();                     //初始化LCD
    
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Apollos STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"WKUP TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/27");	
	LCD_ShowString(30,130,200,16,16,"WK_UP:Stanby/WK_UP");	  
	while(1)
	{
        LED0=!LED0;
		delay_ms(250);              //延时250ms
	}  
}
