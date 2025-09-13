#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h"    
#include "key.h" 
#include "malloc.h" 
#include "usmart.h"  
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"  
#include "text.h"	   
#include "wm8978.h"	 
#include "audioplay.h"	
//ALIENTEK ������STM32F429������ ʵ��45
//���ֲ����� ʵ�� 
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

 
int main(void)
{      
	Stm32_Clock_Init(360,25,2,8);//����ʱ��,180Mhz
	delay_init(180);			//��ʼ����ʱ���� 
	uart_init(90,115200);		//��ʼ�����ڲ�����Ϊ115200 
	usmart_dev.init(90);
   	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD
	KEY_Init();					//��ʼ������ 
	W25QXX_Init();				//��ʼ��W25Q256
	WM8978_Init();				//��ʼ��WM8978
	WM8978_HPvol_Set(40,40);	//������������
	WM8978_SPKvol_Set(50);		//������������
	
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
  	f_mount(fs[0],"0:",1); 		//����SD��  
 	f_mount(fs[1],"1:",1); 		//����SPI FLASH
 	POINT_COLOR=RED;      
	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}  	 
	POINT_COLOR=RED;      
 	Show_Str(60,50,200,16,"������STM32F4/F7������",16,0);				    	 
	Show_Str(60,70,200,16,"���ֲ�����ʵ��",16,0);				    	 
	Show_Str(60,90,200,16,"����ԭ��@ALIENTEK",16,0);				    	 
	Show_Str(60,110,200,16,"2016��1��11��",16,0);
	Show_Str(60,130,200,16,"KEY0:NEXT   KEY2:PREV",16,0); 
	Show_Str(60,150,200,16,"KEY_UP:PAUSE/PLAY",16,0);
	
	while(1)
	{ 
		audio_play();
	} 
}






