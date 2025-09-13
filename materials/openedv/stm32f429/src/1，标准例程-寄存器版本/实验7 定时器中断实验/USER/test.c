#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "timer.h" 
//ALIENTEK ������STM32F429������ ʵ��7
//��ʱ���ж�ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
  
int main(void)
{   
	Stm32_Clock_Init(360,25,2,8);	//����ʱ��,180Mhz
	delay_init(180);				//��ʼ����ʱ���� 
	LED_Init();		  				//��ʼ����LED���ӵ�Ӳ���ӿ�
 	TIM3_Int_Init(5000-1,9000-1);	//10Khz�ļ���Ƶ�ʣ�����5K��Ϊ500ms     
	while(1)
	{
		LED0=!LED0;
		delay_ms(200);
	};
}

















