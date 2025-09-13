#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "timer.h" 
#include "usart.h" 
//ALIENTEK ������STM32F429������ ʵ��9
//���벶��ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
  
extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
int main(void)
{  
	long long temp=0;  
	Stm32_Clock_Init(360,25,2,8);	//����ʱ��,180Mhz
	delay_init(180);				//��ʼ����ʱ���� 
	uart_init(90,115200);			//��ʼ�����ڲ�����Ϊ115200
	LED_Init();		  				//��ʼ����LED���ӵ�Ӳ���ӿ�
 	TIM3_PWM_Init(500-1,96-1);		//1Mhz�ļ���Ƶ��,2Khz��PWM.     
 	TIM5_CH1_Cap_Init(0XFFFFFFFF,90-1);//��1Mhz��Ƶ�ʼ��� 
   	while(1)
	{
 		delay_ms(10);
		LED0_PWM_VAL++;
		if(LED0_PWM_VAL==300)LED0_PWM_VAL=0;	 		 
 		if(TIM5CH1_CAPTURE_STA&0X80)		//�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		//���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%lld us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;			//������һ�β���
		}
	}
}

















