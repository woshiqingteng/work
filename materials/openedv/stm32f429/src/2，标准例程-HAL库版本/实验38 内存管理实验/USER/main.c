#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "usmart.h"
#include "malloc.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��38
 �ڴ����ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 
int main(void)
{
    u8 paddr[20];				    //���P Addr:+p��ַ��ASCIIֵ
	u16 memused=0;
	u8 key;		 
 	u8 i=0;	    
	u8 *p=0;
	u8 *tp=0;
	u8 sramx=0;					    //Ĭ��Ϊ�ڲ�sram
    
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    usmart_dev.init(90); 		    //��ʼ��USMART	
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    LCD_Init();                     //��ʼ��LCD
    
    my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ��
     
   	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"MALLOC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");	 		
	LCD_ShowString(30,130,200,16,16,"KEY0:Malloc  KEY2:Free");
	LCD_ShowString(30,150,200,16,16,"KEY_UP:SRAMx KEY1:Read"); 
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_ShowString(30,170,200,16,16,"SRAMIN");
	LCD_ShowString(30,190,200,16,16,"SRAMIN  USED:");
	LCD_ShowString(30,210,200,16,16,"SRAMEX  USED:");
	LCD_ShowString(30,230,200,16,16,"SRAMCCM USED:");
 	while(1)
	{	
		key=KEY_Scan(0);//��֧������	
		switch(key)
		{
			case 0://û�а�������	
				break;
			case KEY0_PRES:	//KEY0����
				p=mymalloc(sramx,2048);//����2K�ֽ�
				if(p!=NULL)sprintf((char*)p,"Memory Malloc Test%03d",i);//��pд��һЩ����
				break;
			case KEY1_PRES:	//KEY1����	   
				if(p!=NULL)
				{
					sprintf((char*)p,"Memory Malloc Test%03d",i);//������ʾ���� 	 
					LCD_ShowString(30,270,200,16,16,p);			 //��ʾP������
				}
				break;
			case KEY2_PRES:	//KEY2����	  
				myfree(sramx,p);//�ͷ��ڴ�
				p=0;			//ָ��յ�ַ
				break;
			case WKUP_PRES:	//KEY UP���� 
				sramx++; 
				if(sramx>2)sramx=0;
				if(sramx==0)LCD_ShowString(30,170,200,16,16,"SRAMIN ");
				else if(sramx==1)LCD_ShowString(30,170,200,16,16,"SRAMEX ");
				else LCD_ShowString(30,170,200,16,16,"SRAMCCM");
				break;
		}
		if(tp!=p&&p!=NULL)
		{
			tp=p;
			sprintf((char*)paddr,"P Addr:0X%08X",(u32)tp);
			LCD_ShowString(30,250,200,16,16,paddr);	//��ʾp�ĵ�ַ
			if(p)LCD_ShowString(30,270,200,16,16,p);//��ʾP������
		    else LCD_Fill(30,270,239,266,WHITE);	//p=0,�����ʾ
		}
		delay_ms(10);   
		i++;
		if((i%20)==0)//DS0��˸.
		{
			memused=my_mem_perused(SRAMIN);
			sprintf((char*)paddr,"%d.%01d%%",memused/10,memused%10);
			LCD_ShowString(30+104,190,200,16,16,paddr);	//��ʾ�ڲ��ڴ�ʹ����
			memused=my_mem_perused(SRAMEX);
			sprintf((char*)paddr,"%d.%01d%%",memused/10,memused%10);
			LCD_ShowString(30+104,210,200,16,16,paddr);	//��ʾ�ⲿ�ڴ�ʹ����
			memused=my_mem_perused(SRAMCCM);
			sprintf((char*)paddr,"%d.%01d%%",memused/10,memused%10);
			LCD_ShowString(30+104,230,200,16,16,paddr);	//��ʾCCM�ڴ�ʹ���� 
 			LED0=!LED0;
 		}
	} 
}
