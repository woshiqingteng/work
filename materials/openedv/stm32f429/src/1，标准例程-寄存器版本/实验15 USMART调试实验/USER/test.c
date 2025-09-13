#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h" 
#include "key.h" 
#include "timer.h"
#include "sdram.h" 
#include "usmart.h" 
//ALIENTEK ������STM32F429������ ʵ��15
//USMART���� ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
 
//LED״̬���ú���
void led_set(u8 sta)
{
	LED1=sta;
} 
//�����������ò��Ժ���
void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}   
int main(void)
{    
	Stm32_Clock_Init(360,25,2,8);	//����ʱ��,180Mhz
	delay_init(180);			//��ʼ����ʱ���� 
	uart_init(90,115200);		//��ʼ�����ڲ�����Ϊ115200
	usmart_dev.init(90); 		//��ʼ��USMART	
	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ��LED
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"USMART TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/10");	   
  	while(1) 
	{		 	  
		LED0=!LED0;					 
		delay_ms(500);	
	} 
}

















