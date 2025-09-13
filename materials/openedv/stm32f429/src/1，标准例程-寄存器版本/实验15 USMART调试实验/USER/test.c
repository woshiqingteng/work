#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h" 
#include "key.h" 
#include "timer.h"
#include "sdram.h" 
#include "usmart.h" 
//ALIENTEK 阿波罗STM32F429开发板 实验15
//USMART调试 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 
//LED状态设置函数
void led_set(u8 sta)
{
	LED1=sta;
} 
//函数参数调用测试函数
void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}   
int main(void)
{    
	Stm32_Clock_Init(360,25,2,8);	//设置时钟,180Mhz
	delay_init(180);			//初始化延时函数 
	uart_init(90,115200);		//初始化串口波特率为115200
	usmart_dev.init(90); 		//初始化USMART	
	LED_Init();					//初始化与LED连接的硬件接口
	KEY_Init();					//初始化LED
	SDRAM_Init();				//初始化SDRAM 
	LCD_Init();					//初始化LCD
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"USMART TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/10");	   
  	while(1) 
	{		 	  
		LED0=!LED0;					 
		delay_ms(500);	
	} 
}

















