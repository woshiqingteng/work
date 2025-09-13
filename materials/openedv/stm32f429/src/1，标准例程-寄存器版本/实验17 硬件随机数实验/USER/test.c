#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h" 
#include "usmart.h" 
#include "key.h" 
#include "rng.h"  
//ALIENTEK ������STM32F429������ ʵ��17
//Ӳ������� ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
   
int main(void)
{  
	u32 random;
	u8 t=0,key;
	Stm32_Clock_Init(360,25,2,8);	//����ʱ��,180Mhz
	delay_init(180);			//��ʼ����ʱ���� 
	uart_init(90,115200);		//��ʼ�����ڲ�����Ϊ115200
	usmart_dev.init(90); 		//��ʼ��USMART	
	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//������ʼ��
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"RNG TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/27");	 
	while(RNG_Init())	 		//��ʼ�������������
	{
		LCD_ShowString(30,130,200,16,16,"  RNG Error! ");	 
		delay_ms(200);
		LCD_ShowString(30,130,200,16,16,"RNG Trying...");	 
	}                                 
	LCD_ShowString(30,130,200,16,16,"RNG Ready!   ");	 
	LCD_ShowString(30,150,200,16,16,"KEY0:Get Random Num");	 
	LCD_ShowString(30,180,200,16,16,"Random Num:");	 
 	LCD_ShowString(30,210,200,16,16,"Random Num[0-9]:");
 	POINT_COLOR=BLUE;
	while(1) 
	{		
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			random=RNG_Get_RandomNum(); 
			LCD_ShowNum(30+8*11,180,random,10,16); 
		} 
		if((t%20)==0)
		{
			LED0=!LED0;	//ÿ200ms,��תһ��LED0 
			random=RNG_Get_RandomRange(0,9);		//��ȡ[0,9]����������
			LCD_ShowNum(30+8*16,210,random,1,16); 	//��ʾ�����
		}
		delay_ms(10);
		t++;
	}	 
}

















