#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "lan8720.h"
#include "timer.h"
#include "pcf8574.h"
#include "ov2640.h"
#include "dcmi.h"
#include "usmart.h"
#include "malloc.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "netcamera_demo.h"
/************************************************
 ALIENTEK ������STM32F429����ʵ��11
 ��������ͷʵ��(OV2640�汾)
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//KEY����
#define KEY_TASK_PRIO 		8
//�����ջ��С
#define KEY_STK_SIZE		128		
//�����ջ
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//������
void key_task(void *pdata);  

//��LCD����ʾ��ַ��Ϣ����
//�������ȼ�
#define DISPLAY_TASK_PRIO	9
//�����ջ��С
#define DISPLAY_STK_SIZE	128
//�����ջ
OS_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//������
void display_task(void *pdata);


//LED����
//�������ȼ�
#define LED_TASK_PRIO		10
//�����ջ��С
#define LED_STK_SIZE		64
//�����ջ
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *pdata);  


//START����
//�������ȼ�
#define START_TASK_PRIO		11
//�����ջ��С
#define START_STK_SIZE		128
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata); 

//��LCD����ʾ��Ϣ
//mode:1 ��ʾDHCP��ȡ���ĵ�ַ
//	  ���� ��ʾ��̬��ַ
void show_inform(u8 mode)
{
   
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",\
                lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]); //��ӡ��̬IP��ַ
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",\
                lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]); //��ӡ��̬IP��ַ 
	} 
    LCD_ShowString(30,110,210,16,16,"KEY_UP:Focus,KEY0:Resol"); 
    LCD_ShowString(30,130,210,16,16,buf); 
    LCD_ShowString(30,150,210,16,16,"Port:8088!"); 
    POINT_COLOR = BLUE; 		        //��ɫ��ɫ   
    LCD_ShowString(30,170,210,16,16,"Resol:1280*800");

}

int main(void)
{   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz   
    HAL_Init();                     //��ʼ��HAL��
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    usmart_dev.init(90); 		    //��ʼ��USMART	
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    LCD_Init();                     //��ʼ��LCD
    PCF8574_Init();                 //��ʼ��PCF8574
    my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ��
        
	POINT_COLOR = RED; 		        //��ɫ����
	LCD_ShowString(30,30,200,20,16,"Apollo STM32F4/F7");
	LCD_ShowString(30,50,200,20,16,"NetCamera Test");
	LCD_ShowString(30,70,200,20,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,20,16,"2016/2/24");
    
	OSInit(); 					    //UCOS��ʼ��
	while(lwip_comm_init()) 	    //lwip��ʼ��
	{
		LCD_ShowString(30,110,200,20,16,"Lwip Init failed! "); 	//lwip��ʼ��ʧ��
		delay_ms(500);
		LCD_Fill(30,110,230,150,WHITE);
		delay_ms(500);
	}
    netcamera_init();
	LCD_ShowString(30,110,200,20,16,"Lwip Success!     "); 	//��������ͷ�����ɹ�
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart(); //����UCOS
}

//start����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OSStatInit();  			//��ʼ��ͳ������
	OS_ENTER_CRITICAL();  	//���ж�
#if LWIP_DHCP
	lwip_comm_dhcp_creat(); //����DHCP����
#endif
	
	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);//����LED����
    OSTaskCreate(key_task,(void*)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);//��������
	OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO); //��ʾ����
	OSTaskSuspend(OS_PRIO_SELF); //����start_task����
	OS_EXIT_CRITICAL();  		//���ж�
}

//��ʾ��ַ����Ϣ
void display_task(void *pdata)
{
	while(1)
	{ 
#if LWIP_DHCP									//������DHCP��ʱ��
		if(lwipdev.dhcpstatus != 0) 			//����DHCP
		{
			show_inform(lwipdev.dhcpstatus );	//��ʾ��ַ��Ϣ
			OSTaskSuspend(OS_PRIO_SELF); 		//��ʾ���ַ��Ϣ�������������
		}
#else
		show_inform(0); 						//��ʾ��̬��ַ
		OSTaskSuspend(OS_PRIO_SELF); 		 	//��ʾ���ַ��Ϣ�������������
#endif //LWIP_DHCP
		OSTimeDlyHMSM(0,0,0,500);
	}
}

//key����
void key_task(void *pdata)
{
    u8 resolut=0;
	u8 key=0; 
	while(1)
	{
        key=KEY_Scan(0);
        if(key==KEY0_PRES)//�����ֱ���
        {
            resolut++;
            if(resolut==5) resolut=0;
            netcam_resolu_set(resolut);//���÷ֱ���
            switch(resolut)            //��ʾ�����õķֱ���
            {
                case 0:
                    LCD_ShowString(78,170,210,16,16,"320*240 ");
                    break;
                case 1:
                    LCD_ShowString(78,170,210,16,16,"640*480 ");
                    break;
                case 2:
                    LCD_ShowString(78,170,210,16,16,"1024*768");
                    break;
                case 3:
                    LCD_ShowString(78,170,210,16,16,"1280*800");
                    break;
                case 4:
                    LCD_ShowString(78,170,210,16,16,"1600*1200");
                    break;
            }     
        }
		OSTimeDlyHMSM(0,0,0,10);  //��ʱ10ms
	}
}

//led����
void led_task(void *pdata)
{
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500);  //��ʱ500ms
 	}
}
