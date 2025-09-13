#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h"    
#include "key.h" 
#include "malloc.h" 
#include "usmart.h"  
#include "sdio_sdcard.h"    
//ALIENTEK ������STM32F429������ ʵ��39
//SD�� ʵ�� 
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
}
//����SD���Ķ�ȡ
//��secaddr��ַ��ʼ,��ȡseccnt������������
//secaddr:������ַ
//seccnt:������
void sd_test_read(u32 secaddr,u32 seccnt)
{
	u32 i;
	u8 *buf;
	u8 sta=0;
	buf=mymalloc(SRAMEX,seccnt*512);	//�����ڴ�,��SDRAM�����ڴ�
	sta=SD_ReadDisk(buf,secaddr,seccnt);//��ȡsecaddr������ʼ������
	if(sta==0)			
	{	 
		printf("SECTOR %d DATA:\r\n",secaddr);
		for(i=0;i<seccnt*512;i++)printf("%x ",buf[i]);//��ӡsecaddr��ʼ����������    	   
		printf("\r\nDATA ENDED\r\n"); 
	}else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);	//�ͷ��ڴ�	   
}

//����SD����д��(����,���дȫ��0XFF������,���������SD��.)
//��secaddr��ַ��ʼ,д��seccnt������������
//secaddr:������ַ
//seccnt:������
void sd_test_write(u32 secaddr,u32 seccnt)
{
	u32 i;
	u8 *buf;
	u8 sta=0;
	buf=mymalloc(SRAMEX,seccnt*512);	//��SDRAM�����ڴ�
	for(i=0;i<seccnt*512;i++) 			//��ʼ��д�������,��3�ı���.
	{
		buf[i]=i*3;
	}
	sta=SD_WriteDisk(buf,secaddr,seccnt);//��secaddr������ʼд��seccnt����������
	if(sta==0)							
	{	 
		printf("Write over!\r\n"); 
	}else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);					//�ͷ��ڴ�	   
}

int main(void)
{     
	u8 key;		 
	u32 sd_size;
	u8 t=0;	
	u8 *buf;	
	Stm32_Clock_Init(360,25,2,8);//����ʱ��,180Mhz
	delay_init(180);			//��ʼ����ʱ���� 
	uart_init(90,115200);		//��ʼ�����ڲ�����Ϊ115200 
	usmart_dev.init(90);
   	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD
	KEY_Init();					//��ʼ������
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
   	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"SD CARD TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/6");	 	 
	LCD_ShowString(30,130,200,16,16,"KEY0:Read Sector 0");	   
 	while(SD_Init())//��ⲻ��SD��
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	show_sdcard_info();	//��ӡSD�������Ϣ
 	POINT_COLOR=BLUE;	//��������Ϊ��ɫ 
	//���SD���ɹ� 											    
	LCD_ShowString(30,150,200,16,16,"SD Card OK    ");
	LCD_ShowString(30,170,200,16,16,"SD Card Size:     MB");
	LCD_ShowNum(30+13*8,170,SDCardInfo.CardCapacity>>20,5,16);//��ʾSD������
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0������
		{
			buf=mymalloc(0,512);		//�����ڴ�
			key=SD_ReadDisk(buf,0,1);
			if(key==0)	//��ȡ0����������
			{	
				LCD_ShowString(30,190,200,16,16,"USART1 Sending Data...");
				printf("SECTOR 0 DATA:\r\n");
				for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf[sd_size]);//��ӡ0��������    	   
				printf("\r\nDATA ENDED\r\n");
				LCD_ShowString(30,190,200,16,16,"USART1 Send Data Over!");
			}else printf("err:%d\r\n",key);
			myfree(0,buf);//�ͷ��ڴ�	   
		}  
		t++;
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;
			t=0;
		}
	} 
}






