#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "ap3216c.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验26
 APC3216C光照和接近传感器实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
    u16 ir,als,ps;
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
	LCD_ShowString(30,70,200,16,16,"AP3216C TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");	 		
	while(AP3216C_Init())		//检测不到AP3216C
	{
		LCD_ShowString(30,130,200,16,16,"AP3216C Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,130,200,16,16,"Please Check!        ");
		delay_ms(500);
		LED0=!LED0;				//DS0闪烁
	}	
    LCD_ShowString(30,130,200,16,16,"AP3216C Ready!");  
    LCD_ShowString(30,160,200,16,16," IR:");	 
	LCD_ShowString(30,180,200,16,16," PS:");	
	LCD_ShowString(30,200,200,16,16,"ALS:");	 
	POINT_COLOR=BLUE;			//设置字体为蓝色		  
    while(1)
    {
        AP3216C_ReadData(&ir,&ps,&als);	//读取数据 
        LCD_ShowNum(30+32,160,ir,5,16);	//显示IR数据
        LCD_ShowNum(30+32,180,ps,5,16);	//显示PS数据
        LCD_ShowNum(30+32,200,als,5,16);//显示ALS数据 
		LED0=!LED0;	;			        //提示系统正在运行	
        delay_ms(120); 
	}		    
}
