#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "dht11.h"
#include "pcf8574.h"

/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验33
 DHT11数字温湿度传感器实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
	u8 t=0;			     
	u8 temperature;  	    
	u8 humidity;    
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART	
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
    LCD_Init();                     //初始化LCD
    PCF8574_Init();                 //初始化PCF8574
   	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"DHT11 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");	
    PCF8574_ReadBit(BEEP_IO);       //由于DHT11和PCF8574的中断引脚共用一个IO，
                                    //所以在初始化DHT11之前要先读取一次PCF8574的任意一个IO，
                                    //使其释放掉中断引脚所占用的IO(PB12引脚),否则初始化DS18B20会出问题    
 	while(DHT11_Init())	//DHT11初始化	
	{
		LCD_ShowString(30,130,200,16,16,"DHT11 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}								   
	LCD_ShowString(30,130,200,16,16,"DHT11 OK");
	POINT_COLOR=BLUE;//设置字体为蓝色 
 	LCD_ShowString(30,150,200,16,16,"Temp:  C");	 
 	LCD_ShowString(30,170,200,16,16,"Humi:  %");
	while(1)
	{	    	    
 		if(t%10==0)//每100ms读取一次
		{	
            PCF8574_ReadBit(BEEP_IO);   //读取一次PCF8574的任意一个IO，使其释放掉PB12引脚，
                                        //否则读取DHT11可能会出问题            
			DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值					    
			LCD_ShowNum(30+40,150,temperature,2,16);		//显示温度	   		   
			LCD_ShowNum(30+40,170,humidity,2,16);			//显示湿度	 	   
		}				   
	 	delay_ms(10);
		t++;
		if(t==20)
		{
			t=0;
			LED0=!LED0;
		}
	}						    
}
