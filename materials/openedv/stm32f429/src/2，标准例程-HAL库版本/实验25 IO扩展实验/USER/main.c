#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "pcf8574.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��25
 IO��չʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
    u8 key;
	u16 i=0; 
	u8 beepsta=1;
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
	LCD_ShowString(30,70,200,16,16,"PCF8574 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/13");	 
	LCD_ShowString(30,130,200,16,16,"KEY0:BEEP ON/OFF");	//��ʾ��ʾ��Ϣ	
	LCD_ShowString(30,150,200,16,16,"EXIO:DS1 ON/OFF");		//��ʾ��ʾ��Ϣ		
	while(PCF8574_Init())		//��ⲻ��PCF8574
	{
		LCD_ShowString(30,170,200,16,16,"PCF8574 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,170,200,16,16,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	LCD_ShowString(30,170,200,16,16,"PCF8574 Ready!");    
	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
	while(1)
	{
		key=KEY_Scan(0); 
		if(key==KEY0_PRES)//KEY0����,��ȡ�ַ�������ʾ
		{ 
			beepsta=!beepsta;					//������״̬ȡ��
			PCF8574_WriteBit(BEEP_IO,beepsta);	//���Ʒ�����
		}
		if(PCF8574_INT==0)				//PCF8574���жϵ͵�ƽ��Ч
		{
			key=PCF8574_ReadBit(EX_IO);	//��ȡEXIO״̬,ͬʱ���PCF8574���ж����(INT�ָ��ߵ�ƽ)
			if(key==0)LED1=!LED1;		//LED1״̬ȡ�� 
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}		   
	} 	    
}
