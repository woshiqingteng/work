#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h" 
#include "adc.h"
//ALIENTEK 阿波罗STM32F429开发板 实验19
//ADC 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
   
int main(void)
{   
 	u16 adcx;
	float temp;
	Stm32_Clock_Init(360,25,2,8);	//设置时钟,180Mhz
	delay_init(180);			//初始化延时函数 
	uart_init(90,115200);		//初始化串口波特率为115200
	LED_Init();					//初始化与LED连接的硬件接口
	SDRAM_Init();				//初始化SDRAM 
	LCD_Init();					//初始化LCD
	Adc_Init(); 				//初始化ADC
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"ADC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/27");	  
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");	      
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");	      
	while(1)
	{
		adcx=Get_Adc_Average(ADC_CH5,20);
		LCD_ShowxNum(134,130,adcx,4,16,0);//显示ADC的值
		temp=(float)adcx*(3.3/4096); 
		adcx=temp;
		LCD_ShowxNum(134,150,adcx,1,16,0);//显示电压值
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(150,150,temp,3,16,0X80);
		LED0=!LED0;
		delay_ms(250);	
	}	
}

















