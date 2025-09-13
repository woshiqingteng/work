#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h"    
#include "malloc.h" 
#include "usmart.h"  
#include "w25qxx.h"  
#include "pcf8574.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"  
#include "text.h"
#include "usbh_usr.h" 
/************************************************
 ALIENTEK ������STM32F429������ʵ��57
 USB U��(Host)ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

//�û�����������
//����ֵ:0,����
//       1,������
u8 USH_User_App(void)
{  
	u32 total,free;
	u8 res=0;
	Show_Str(30,140,200,16,"�豸���ӳɹ�!.",16,0);	 
  	f_mount(fs[3],"3:",1); 	//���¹���U��
	res=exf_getfree("3:",&total,&free);
	if(res==0)
	{
		POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
		LCD_ShowString(30,160,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,180,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,200,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,180,total>>10,5,16);//��ʾU�������� MB
		LCD_ShowNum(174,200,free>>10,5,16);	
	} 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//�豸���ӳɹ�
	{	
		LED1=!LED1;
		delay_ms(200);
	}
	LED1=1;				//�ر�LED1 
 	f_mount(0,"3:",1); 	//ж��U��
	POINT_COLOR=RED;//��������Ϊ��ɫ	   
	Show_Str(30,140,200,16,"�豸������...",16,0);
	LCD_Fill(30,160,239,220,WHITE); 
	return res;
} 


int main(void)
{
	u8 t;
	
    Stm32_Clock_Init(384,25,2,8);   //����ʱ��,192Mhz   
    delay_init(192);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    usmart_dev.init(96);
   	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD 
	W25QXX_Init();				//��ʼ��W25Q256 
	PCF8574_Init();				//��ʼ��PCF8574
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����SPI FLASH. 
 	f_mount(fs[2],"2:",1); 		//����NAND FLASH. 
	POINT_COLOR=RED;      
 	while(font_init()) 				//����ֿ�
	{	    
		LCD_ShowString(60,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}
	Show_Str(30,50,200,16,"������STM32F4/F7������",16,0);				    	 
	Show_Str(30,70,200,16,"USB U��ʵ��",16,0);					    	 
	Show_Str(30,90,200,16,"2016��1��24��",16,0);	    	 
	Show_Str(30,110,200,16,"����ԭ��@ALIENTEK",16,0); 
	Show_Str(30,140,200,16,"�豸������...",16,0);			 		
	//��ʼ��USB����
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);  
	while(1)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
		t++;
		if(t==200)
		{
			LED0=!LED0;
			t=0;
		} 
	}	
}
