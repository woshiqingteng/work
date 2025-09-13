#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h"  
#include "key.h" 
#include "usmart.h"
#include "pcf8574.h"
//ALIENTEK 阿波罗STM32F429开发板 实验25
//PCF8574 IO扩展 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 
int main(void)
{     
	u8 key;
	u16 i=0; 
	u8 beepsta=1;
	Stm32_Clock_Init(360,25,2,8);	//设置时钟,180Mhz
	delay_init(180);			//初始化延时函数 
	uart_init(90,115200);		//初始化串口波特率为115200
	usmart_dev.init(90);		//初始化USMART
 	LED_Init();					//初始化与LED连接的硬件接口
	SDRAM_Init();				//初始化SDRAM 
	LCD_Init();					//初始化LCD
 	KEY_Init(); 				//按键初始化	 
 	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"PCF8574 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/28");	 
	LCD_ShowString(30,130,200,16,16,"KEY0:BEEP ON/OFF");	//显示提示信息	
	LCD_ShowString(30,150,200,16,16,"EXIO:DS1 ON/OFF");		//显示提示信息		
	while(PCF8574_Init())		//检测不到PCF8574
	{
		LCD_ShowString(30,170,200,16,16,"PCF8574 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,170,200,16,16,"Please Check!        ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	LCD_ShowString(30,170,200,16,16,"PCF8574 Ready!");    
 	POINT_COLOR=BLUE;//设置字体为蓝色	  
	while(1)
	{
		key=KEY_Scan(0); 
		if(key==KEY0_PRES)//KEY0按下,读取字符串并显示
		{ 
			beepsta=!beepsta;					//蜂鸣器状态取反
			PCF8574_WriteBit(BEEP_IO,beepsta);	//控制蜂鸣器
		}
		if(PCF8574_INT==0)				//PCF8574的中断低电平有效
		{
			key=PCF8574_ReadBit(EX_IO);	//读取EXIO状态,同时清除PCF8574的中断输出(INT恢复高电平)
			if(key==0)LED1=!LED1;		//LED1状态取反 
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//提示系统正在运行	
			i=0;
		}		   
	} 	    
}

















