#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "tpad.h" 
//ALIENTEK ������STM32F429������ ʵ��10
//���ݴ�������ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
   
int main(void)
{  
	u8 t=0;
	Stm32_Clock_Init(360,25,2,8);	//����ʱ��,180Mhz
	delay_init(180);			//��ʼ����ʱ���� 
	uart_init(90,115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
 	TPAD_Init(2);				//��ʼ����������,��90/2=45MhzƵ�ʼ���
   	while(1)
	{					  						  		 
 		if(TPAD_Scan(0))	//�ɹ�������һ��������(�˺���ִ��ʱ������15ms)
		{
			LED1=!LED1;		//LED1ȡ��
		}
		t++;
		if(t==15)		 
		{
			t=0;
			LED0=!LED0;		//LED0ȡ��,��ʾ������������
		}
		delay_ms(10);
	}
}

















