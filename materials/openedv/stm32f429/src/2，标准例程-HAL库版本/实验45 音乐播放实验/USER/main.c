#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "sdio_sdcard.h"
#include "fontupd.h"
#include "text.h"
#include "wm8978.h"	 
#include "audioplay.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验45
 音乐播放器实验-HAL库函数版
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
    SDRAM_Init();                   //SDRAM初始化
    LCD_Init();                     //LCD初始化
	W25QXX_Init();				    //初始化W25Q256
    WM8978_Init();				    //初始化WM8978
	WM8978_HPvol_Set(40,40);	    //耳机音量设置
	WM8978_SPKvol_Set(40);		    //喇叭音量设置
    my_mem_init(SRAMIN);            //初始化内部内存池
    my_mem_init(SRAMEX);            //初始化外部SDRAM内存池
    my_mem_init(SRAMCCM);           //初始化内部CCM内存池
    exfuns_init();		            //为fatfs相关变量申请内存  
    f_mount(fs[0],"0:",1);          //挂载SD卡 
 	f_mount(fs[1],"1:",1);          //挂载SPI FLASH.  
	POINT_COLOR=RED;      
	while(font_init()) 			    //检查字库
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//清除显示	     
		delay_ms(200);				  
	}  	 
	POINT_COLOR=RED;      
 	Show_Str(60,50,200,16,"阿波罗STM32F4/F7开发板",16,0);				    	 
	Show_Str(60,70,200,16,"音乐播放器实验",16,0);				    	 
	Show_Str(60,90,200,16,"正点原子@ALIENTEK",16,0);				    	 
	Show_Str(60,110,200,16,"2016年1月18日",16,0);
	Show_Str(60,130,200,16,"KEY0:NEXT   KEY2:PREV",16,0); 
	Show_Str(60,150,200,16,"KEY_UP:PAUSE/PLAY",16,0);
	while(1)
	{ 
		audio_play();
	} 
}
