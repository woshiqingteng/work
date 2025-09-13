#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h" 
#include "wkup.h"  
//ALIENTEK ������STM32F429������ ʵ��18
//�������� ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
   
int main(void)
{   
	Stm32_Clock_Init(360,25,2,8);	//����ʱ��,180Mhz
	delay_init(180);			//��ʼ����ʱ���� 
	uart_init(90,115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
 	WKUP_Init();				//�������ѳ�ʼ��
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"WKUP TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/27");	
	LCD_ShowString(30,130,200,16,16,"WK_UP:Stanby/WK_UP");	 
	while(1)
	{
		LED0=!LED0;
		delay_ms(250);  
	}		
}

















