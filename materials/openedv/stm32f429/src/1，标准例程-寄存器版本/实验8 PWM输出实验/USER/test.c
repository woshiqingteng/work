#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "timer.h" 
//ALIENTEK ������STM32F429������ ʵ��8
//PWM���ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
  
int main(void)
{  
	u16 led0pwmval=0;    
	u8 dir=1; 
	Stm32_Clock_Init(360,25,2,8);//����ʱ��,180Mhz
	delay_init(180);		//��ʼ����ʱ���� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	TIM3_PWM_Init(500-1,90-1);	//1Mhz�ļ���Ƶ��,2Khz��PWM.     
   	while(1)
	{
 		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;	 
 		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;	   					 
		LED0_PWM_VAL=led0pwmval;	   
	}
}

















