#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "oled.h" 
//ALIENTEK 阿波罗STM32F429开发板 实验11
//OLED显示实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
   
int main(void)
{  
	u8 t=0;
	Stm32_Clock_Init(360,25,2,8);//设置时钟,180Mhz
	delay_init(180);			//初始化延时函数 
	uart_init(90,115200);		//初始化串口波特率为115200
	LED_Init();					//初始化与LED连接的硬件接口
	OLED_Init();				//初始化OLED
  	OLED_ShowString(0,0,"ALIENTEK",24);  
	OLED_ShowString(0,24, "0.96' OLED TEST",16);  
 	OLED_ShowString(0,40,"ATOM 2015/12/5",12);  
 	OLED_ShowString(0,52,"ASCII:",12);  
 	OLED_ShowString(64,52,"CODE:",12);  
	OLED_Refresh_Gram();//更新显示到OLED	 
	t=' ';  
	while(1) 
	{		
		OLED_ShowChar(36,52,t,12,1);//显示ASCII字符	
		OLED_ShowNum(94,52,t,3,12);	//显示ASCII字符的码值    
		OLED_Refresh_Gram();//更新显示到OLED
		t++;
		if(t>'~')t=' ';  
		delay_ms(500);
		LED0=!LED0;
	}
}

















