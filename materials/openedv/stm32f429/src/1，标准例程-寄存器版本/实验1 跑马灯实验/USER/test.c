#include "sys.h"
#include "delay.h" 
#include "led.h"
//ALIENTEK ������STM32F429������ ʵ��1
//�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
  
int main(void)
{ 
	Stm32_Clock_Init(360,25,2,8);//����ʱ��,180Mhz
	delay_init(180);		//��ʼ����ʱ����
	LED_Init();				//��ʼ��LEDʱ��  
	while(1)
	{
		LED0=0;				//DS0��
		LED1=1;				//DS1��
		delay_ms(500);
		LED0=1;				//DS0��
		LED1=0;				//DS1��
		delay_ms(500);
	}
}

















