#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "ap3216c.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��26
 APC3216C���պͽӽ�������ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
    u16 ir,als,ps;
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    LCD_Init();                     //��ʼ��LCD
  	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"AP3216C TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");	 		
	while(AP3216C_Init())		//��ⲻ��AP3216C
	{
		LCD_ShowString(30,130,200,16,16,"AP3216C Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,130,200,16,16,"Please Check!        ");
		delay_ms(500);
		LED0=!LED0;				//DS0��˸
	}	
    LCD_ShowString(30,130,200,16,16,"AP3216C Ready!");  
    LCD_ShowString(30,160,200,16,16," IR:");	 
	LCD_ShowString(30,180,200,16,16," PS:");	
	LCD_ShowString(30,200,200,16,16,"ALS:");	 
	POINT_COLOR=BLUE;			//��������Ϊ��ɫ		  
    while(1)
    {
        AP3216C_ReadData(&ir,&ps,&als);	//��ȡ���� 
        LCD_ShowNum(30+32,160,ir,5,16);	//��ʾIR����
        LCD_ShowNum(30+32,180,ps,5,16);	//��ʾPS����
        LCD_ShowNum(30+32,200,als,5,16);//��ʾALS���� 
		LED0=!LED0;	;			        //��ʾϵͳ��������	
        delay_ms(120); 
	}		    
}
