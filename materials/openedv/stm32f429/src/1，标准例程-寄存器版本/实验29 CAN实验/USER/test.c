#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h"  
#include "key.h" 
#include "usmart.h" 
#include "can.h"
//ALIENTEK 阿波罗STM32F429开发板 实验29
//CAN 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
  
int main(void)
{     
	u8 key;
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf[8];
	u8 res;
	u8 mode=1;//CAN工作模式;0,普通模式;1,环回模式
	Stm32_Clock_Init(360,25,2,8);//设置时钟,180Mhz
	delay_init(180);			//初始化延时函数 
	uart_init(90,115200);		//初始化串口波特率为115200
	usmart_dev.init(90);		//初始化USMART
 	LED_Init();					//初始化与LED连接的硬件接口
	SDRAM_Init();				//初始化SDRAM 
	LCD_Init();					//初始化LCD	
	KEY_Init(); 				//按键初始化 
 	CAN1_Mode_Init(1,5,9,6,1);	//CAN初始化,波特率500Kbps    
  	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"CAN TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/28");	 		
	LCD_ShowString(30,130,200,16,16,"LoopBack Mode");	 
	LCD_ShowString(30,150,200,16,16,"KEY0:Send WK_UP:Mode");//显示提示信息		
  	POINT_COLOR=BLUE;//设置字体为蓝色	  
	LCD_ShowString(30,170,200,16,16,"Count:");			//显示当前计数值	
	LCD_ShowString(30,190,200,16,16,"Send Data:");		//提示发送的数据	
	LCD_ShowString(30,250,200,16,16,"Receive Data:");	//提示接收到的数据		
 	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0按下,发送一次数据
		{
			for(i=0;i<8;i++)
			{
				canbuf[i]=cnt+i;//填充发送缓冲区
				if(i<4)LCD_ShowxNum(30+i*32,210,canbuf[i],3,16,0X80);	//显示数据
				else LCD_ShowxNum(30+(i-4)*32,230,canbuf[i],3,16,0X80);	//显示数据
 			}
			res=CAN1_Send_Msg(canbuf,8);//发送8个字节 
			if(res)LCD_ShowString(30+80,190,200,16,16,"Failed");		//提示发送失败
			else LCD_ShowString(30+80,190,200,16,16,"OK    ");	 		//提示发送成功								   
		}else if(key==WKUP_PRES)//WK_UP按下，改变CAN的工作模式
		{	   
			mode=!mode;
			CAN1_Mode_Init(1,5,9,6,mode);	//CAN普通模式初始化,普通模式,波特率500Kbps
  			POINT_COLOR=RED;//设置字体为红色 
			if(mode==0)//普通模式，需要2个开发板
			{
				LCD_ShowString(30,130,200,16,16,"Nnormal Mode ");	    
			}else //回环模式,一个开发板就可以测试了.
			{
 				LCD_ShowString(30,130,200,16,16,"LoopBack Mode");
			}
 			POINT_COLOR=BLUE;//设置字体为蓝色 
		}		 
		key=CAN1_Receive_Msg(canbuf);
		if(key)//接收到有数据
		{			
			LCD_Fill(30,270,160,310,WHITE);//清除之前的显示
 			for(i=0;i<key;i++)
			{									    
				if(i<4)LCD_ShowxNum(30+i*32,270,canbuf[i],3,16,0X80);	//显示数据
				else LCD_ShowxNum(30+(i-4)*32,290,canbuf[i],3,16,0X80);	//显示数据
 			}
		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;//提示系统正在运行	
			t=0;
			cnt++;
			LCD_ShowxNum(30+48,170,cnt,3,16,0X80);	//显示数据
		}		   
	}  
}

















