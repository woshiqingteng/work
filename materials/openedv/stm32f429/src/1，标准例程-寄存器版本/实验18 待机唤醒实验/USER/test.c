#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h" 
#include "wkup.h"  
//ALIENTEK 阿波罗STM32F429开发板 实验18
//待机唤醒 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
   
int main(void)
{   
	Stm32_Clock_Init(360,25,2,8);	//设置时钟,180Mhz
	delay_init(180);			//初始化延时函数 
	uart_init(90,115200);		//初始化串口波特率为115200
	LED_Init();					//初始化与LED连接的硬件接口
 	WKUP_Init();				//待机唤醒初始化
	SDRAM_Init();				//初始化SDRAM 
	LCD_Init();					//初始化LCD
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"WKUP TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/27");	
	LCD_ShowString(30,130,200,16,16,"WK_UP:Stanby/WK_UP");	 
	while(1)
	{
		LED0=!LED0;
		delay_ms(250);  
	}		
}

















