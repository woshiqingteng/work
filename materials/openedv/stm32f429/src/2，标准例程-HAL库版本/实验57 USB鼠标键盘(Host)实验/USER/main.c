#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "pcf8574.h"
#include "string.h"
#include "sdio_sdcard.h"
#include "ftl.h"
#include "nand.h"
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
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;
extern HID_Machine_TypeDef HID_Machine;	

//USB��Ϣ��ʾ
//msgx:0,USB������
//     1,USB����
//     2,USB���
//     3,��֧�ֵ�USB�豸
void USBH_Msg_Show(u8 msgx)
{
	POINT_COLOR=RED;
	switch(msgx)
	{
		case 0:	//USB������
			LCD_ShowString(30,130,200,16,16,"USB Connecting...");	
			LCD_Fill(0,150,lcddev.width,lcddev.height,WHITE);
			break;
		case 1:	//USB����
			LCD_ShowString(30,130,200,16,16,"USB Connected    ");	
			LCD_ShowString(30,150,200,16,16,"USB KeyBoard");	 
			LCD_ShowString(30,180,210,16,16,"KEYVAL:");	
			LCD_ShowString(30,200,210,16,16,"INPUT STRING:");	
			break;
		case 2:	//USB���
			LCD_ShowString(30,130,200,16,16,"USB Connected    ");	
			LCD_ShowString(30,150,200,16,16,"USB Mouse");	 
			LCD_ShowString(30,180,210,16,16,"BUTTON:");	
			LCD_ShowString(30,200,210,16,16,"X POS:");	
			LCD_ShowString(30,220,210,16,16,"Y POS:");	
			LCD_ShowString(30,240,210,16,16,"Z POS:");	
			break; 		
		case 3:	//��֧�ֵ�USB�豸
			LCD_ShowString(30,130,200,16,16,"USB Connected    ");	
			LCD_ShowString(30,150,200,16,16,"Unknow Device");	 
			break; 	 
	} 
}   
//HID��������
void USBH_HID_Reconnect(void)
{
	//�ر�֮ǰ������
	USBH_DeInit(&USB_OTG_Core_dev,&USB_Host);	//��λUSB HOST
	USB_OTG_StopHost(&USB_OTG_Core_dev);		//ֹͣUSBhost
	if(USB_Host.usr_cb->DeviceDisconnected)		//����,�Ž�ֹ
	{
		USB_Host.usr_cb->DeviceDisconnected(); 	//�ر�USB����
		USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);
		USB_Host.usr_cb->DeInit();
		USB_Host.class_cb->DeInit(&USB_OTG_Core_dev,&USB_Host.device_prop);
	}
	USB_OTG_DisableGlobalInt(&USB_OTG_Core_dev);//�ر������ж�
	//���¸�λUSB
    __HAL_RCC_USB_OTG_FS_FORCE_RESET();//USB OTG FS ��λ
	delay_ms(5);
    __HAL_RCC_USB_OTG_FS_RELEASE_RESET();//��λ����
	memset(&USB_OTG_Core_dev,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));
	//��������USB HID�豸
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  
}

int main(void)
{
	u32 t; 
	
    Stm32_Clock_Init(384,25,2,8);   //����ʱ��,192Mhz   
    delay_init(192);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //SDRAM��ʼ��
    LCD_Init();                     //LCD��ʼ��
	W25QXX_Init();				    //��ʼ��W25Q256
    PCF8574_Init();				    //��ʼ��PCF8574 
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"USB MOUSE/KEYBOARD TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/2/20");	 
	LCD_ShowString(30,130,200,16,16,"USB Connecting...");	   
 	//��ʼ��USB����
  	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks); 		    
	while(1)
	{
		USBH_Process(&USB_OTG_Core_dev, &USB_Host);
		if(bDeviceState==1)//���ӽ�����
		{ 
			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//���USB HIDͨ��,�Ƿ�����? 
			{ 	    
				USBH_HID_Reconnect();//����
			}				
			
		}else	//����δ������ʱ��,���
		{
			if(USBH_Check_EnumeDead(&USB_Host))	//���USB HOST ö���Ƿ�������?������,�����³�ʼ�� 
			{ 	    
				USBH_HID_Reconnect();//����
			}			
		}
		t++;
		if(t==200000)
		{
			LED0=!LED0;
			t=0;
		}
	} 
}
