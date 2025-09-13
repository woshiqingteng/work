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
//ALIENTEK ������STM32F429������ ʵ��25
//PCF8574 IO��չ ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
 
int main(void)
{     
	u8 key;
	u16 i=0; 
	u8 beepsta=1;
	Stm32_Clock_Init(360,25,2,8);	//����ʱ��,180Mhz
	delay_init(180);			//��ʼ����ʱ���� 
	uart_init(90,115200);		//��ʼ�����ڲ�����Ϊ115200
	usmart_dev.init(90);		//��ʼ��USMART
 	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD
 	KEY_Init(); 				//������ʼ��	 
 	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"PCF8574 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/28");	 
	LCD_ShowString(30,130,200,16,16,"KEY0:BEEP ON/OFF");	//��ʾ��ʾ��Ϣ	
	LCD_ShowString(30,150,200,16,16,"EXIO:DS1 ON/OFF");		//��ʾ��ʾ��Ϣ		
	while(PCF8574_Init())		//��ⲻ��PCF8574
	{
		LCD_ShowString(30,170,200,16,16,"PCF8574 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,170,200,16,16,"Please Check!        ");
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

















