#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "usmart.h"
#include "nand.h"    
#include "ftl.h"  
/************************************************
 ALIENTEK ������STM32F429������ʵ��40
 NAND FLASHʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
    
int main(void)
{
	u8 key,t=0;
	u16 i;
	u8 *buf;
	u8 *backbuf;	
    
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    usmart_dev.init(90); 		    //��ʼ��USMART	
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //SDRAM��ʼ��
    LCD_Init();                     //LCD��ʼ��
    my_mem_init(SRAMIN);            //��ʼ���ڲ��ڴ��
    my_mem_init(SRAMEX);            //��ʼ���ⲿSDRAM�ڴ��
    my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ�� 
   	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"NAND TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/2/16");	 	 
	LCD_ShowString(30,130,200,16,16,"KEY0:Read Sector 2");
	LCD_ShowString(30,150,200,16,16,"KEY1:Write Sector 2");
	LCD_ShowString(30,170,200,16,16,"KEY2:Recover Sector 2");   
 	while(FTL_Init())			    //���NAND FLASH,����ʼ��FTL
	{
		LCD_ShowString(30,190,200,16,16,"NAND Error!");
		delay_ms(500);				 
		LCD_ShowString(30,190,200,16,16,"Please Check");
		delay_ms(500);				 
		LED0=!LED0;//DS0��˸
	}
	backbuf=mymalloc(SRAMIN,NAND_ECC_SECTOR_SIZE);	//����һ�������Ļ���
	buf=mymalloc(SRAMIN,NAND_ECC_SECTOR_SIZE);		//����һ�������Ļ���
 	POINT_COLOR=BLUE;						//��������Ϊ��ɫ 
	sprintf((char*)buf,"NAND Size:%dMB",(nand_dev.block_totalnum/1024)*(nand_dev.page_mainsize/1024)*nand_dev.block_pagenum);
	LCD_ShowString(30,190,200,16,16,buf);	//��ʾNAND����  
	FTL_ReadSectors(backbuf,2,NAND_ECC_SECTOR_SIZE,1);//Ԥ�ȶ�ȡ����0����������,��ֹ��д�����ļ�ϵͳ��.
	while(1)
	{
		key=KEY_Scan(0);
		switch(key)
		{
			case KEY0_PRES://KEY0����,��ȡsector
				key=FTL_ReadSectors(buf,2,NAND_ECC_SECTOR_SIZE,1);//��ȡ����
				if(key==0)//��ȡ�ɹ�
				{
					LCD_ShowString(30,210,200,16,16,"USART1 Sending Data...  ");
					printf("Sector 2 data is:\r\n");
					for(i=0;i<NAND_ECC_SECTOR_SIZE;i++)
					{
						printf("%x ",buf[i]);//�������
					}
					printf("\r\ndata end.\r\n");
					LCD_ShowString(30,210,200,16,16,"USART1 Send Data Over!  "); 
				}
				break;
			case KEY1_PRES://KEY1����,д��sector
				for(i=0;i<NAND_ECC_SECTOR_SIZE;i++)buf[i]=i+t;	//�������(�����,����t��ֵ��ȷ��) 
				LCD_ShowString(30,210,210,16,16,"Writing data to sector..");
				key=FTL_WriteSectors(buf,2,NAND_ECC_SECTOR_SIZE,1);//д������
				if(key==0)LCD_ShowString(30,210,200,16,16,"Write data successed    ");//д��ɹ�
				else LCD_ShowString(30,210,200,16,16,"Write data failed       ");//д��ʧ��
				break;
			case KEY2_PRES://KEY2����,�ָ�sector������
				LCD_ShowString(30,210,210,16,16,"Recovering data...      ");
				key=FTL_WriteSectors(backbuf,2,NAND_ECC_SECTOR_SIZE,1);//д������
				if(key==0)LCD_ShowString(30,210,200,16,16,"Recovering data OK      ");//�ָ��ɹ�
				else LCD_ShowString(30,210,200,16,16,"Recovering data failed  ");//�ָ�ʧ��
				break;
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
