#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h"   
#include "pcf8574.h"  
#include "usbd_cdc_vcp.h"
#include "usbd_usr.h" 
#include "usbd_desc.h" 
//ALIENTEK ������STM32F429������ ʵ��55
//USB���⴮��(Slave) ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

USB_OTG_CORE_HANDLE    USB_OTG_dev;
extern vu8 bDeviceState;			//USB���� ���
 
int main(void)
{       
 	u16 t;
	u16 len;	
	u16 times=0;    
	u8 usbstatus=0;	
	Stm32_Clock_Init(384,25,2,8);	//����ʱ��,192Mhz
	delay_init(192);			//��ʼ����ʱ���� 
	uart_init(96,115200);		//��ʼ�����ڲ�����Ϊ115200
  	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD
	PCF8574_Init();				//��ʼ��PCF8574
  	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"USB Virtual USART TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/2/24");	 
 	LCD_ShowString(30,130,200,16,16,"USB Connecting...");//��ʾUSB��ʼ����
 	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
 	while(1)
	{
		if(usbstatus!=bDeviceState)//USB����״̬�����˸ı�.
		{
			usbstatus=bDeviceState;//��¼�µ�״̬
			if(usbstatus==1)
			{
				POINT_COLOR=BLUE;
				LCD_ShowString(30,130,200,16,16,"USB Connected    ");//��ʾUSB���ӳɹ�
				LED1=0;//DS1��
			}else
			{
				POINT_COLOR=RED;
				LCD_ShowString(30,130,200,16,16,"USB disConnected ");//��ʾUSB�Ͽ�
				LED1=1;//DS1��
			}
		}
		if(USB_USART_RX_STA&0x8000)
		{					   
			len=USB_USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
			usb_printf("\r\n�����͵���ϢΪ:%d\r\n\r\n",len);
			for(t=0;t<len;t++)
			{
				VCP_DataTx(USB_USART_RX_BUF[t]);//���ֽڷ�ʽ,���͸�USB 
			}
			usb_printf("\r\n\r\n");//���뻻��
			USB_USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				usb_printf("\r\n������STM32F4/F7������USB���⴮��ʵ��\r\n");
				usb_printf("����ԭ��@ALIENTEK\r\n\r\n");
			}
			if(times%200==0)usb_printf("����������,�Իس�������\r\n");  
			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		}
	}    
}

















