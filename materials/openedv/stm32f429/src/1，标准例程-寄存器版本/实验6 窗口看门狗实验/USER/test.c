#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "wdg.h" 
//ALIENTEK ������STM32F429������ ʵ��6
//���ڿ��Ź�ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
  
int main(void)
{   
	Stm32_Clock_Init(360,25,2,8);//����ʱ��,180Mhz
	delay_init(180);		//��ʼ����ʱ���� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	LED0=0;				 	//����LED0
	delay_ms(300);			//��ʱ300ms�ٳ�ʼ�����Ź�,LED0�ı仯"�ɼ�"
	WWDG_Init(0X7F,0X5F,3);	//������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8	   
	while(1)
	{
		LED0=1;				//�ر�LED0
	};
}

















