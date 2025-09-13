#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "timer.h" 
//ALIENTEK 阿波罗STM32F429开发板 实验7
//定时器中断实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
  
int main(void)
{   
	Stm32_Clock_Init(360,25,2,8);	//设置时钟,180Mhz
	delay_init(180);				//初始化延时函数 
	LED_Init();		  				//初始化与LED连接的硬件接口
 	TIM3_Int_Init(5000-1,9000-1);	//10Khz的计数频率，计数5K次为500ms     
	while(1)
	{
		LED0=!LED0;
		delay_ms(200);
	};
}

















