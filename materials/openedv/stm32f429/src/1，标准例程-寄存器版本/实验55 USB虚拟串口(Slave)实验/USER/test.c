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
//ALIENTEK 阿波罗STM32F429开发板 实验55
//USB虚拟串口(Slave) 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

USB_OTG_CORE_HANDLE    USB_OTG_dev;
extern vu8 bDeviceState;			//USB连接 情况
 
int main(void)
{       
 	u16 t;
	u16 len;	
	u16 times=0;    
	u8 usbstatus=0;	
	Stm32_Clock_Init(384,25,2,8);	//设置时钟,192Mhz
	delay_init(192);			//初始化延时函数 
	uart_init(96,115200);		//初始化串口波特率为115200
  	LED_Init();					//初始化与LED连接的硬件接口
	SDRAM_Init();				//初始化SDRAM 
	LCD_Init();					//初始化LCD
	PCF8574_Init();				//初始化PCF8574
  	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"USB Virtual USART TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/2/24");	 
 	LCD_ShowString(30,130,200,16,16,"USB Connecting...");//提示USB开始连接
 	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
 	while(1)
	{
		if(usbstatus!=bDeviceState)//USB连接状态发生了改变.
		{
			usbstatus=bDeviceState;//记录新的状态
			if(usbstatus==1)
			{
				POINT_COLOR=BLUE;
				LCD_ShowString(30,130,200,16,16,"USB Connected    ");//提示USB连接成功
				LED1=0;//DS1亮
			}else
			{
				POINT_COLOR=RED;
				LCD_ShowString(30,130,200,16,16,"USB disConnected ");//提示USB断开
				LED1=1;//DS1灭
			}
		}
		if(USB_USART_RX_STA&0x8000)
		{					   
			len=USB_USART_RX_STA&0x3FFF;//得到此次接收到的数据长度
			usb_printf("\r\n您发送的消息为:%d\r\n\r\n",len);
			for(t=0;t<len;t++)
			{
				VCP_DataTx(USB_USART_RX_BUF[t]);//以字节方式,发送给USB 
			}
			usb_printf("\r\n\r\n");//插入换行
			USB_USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				usb_printf("\r\n阿波罗STM32F4/F7开发板USB虚拟串口实验\r\n");
				usb_printf("正点原子@ALIENTEK\r\n\r\n");
			}
			if(times%200==0)usb_printf("请输入数据,以回车键结束\r\n");  
			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		}
	}    
}

















