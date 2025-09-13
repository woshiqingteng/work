#include "sys.h"
#include "delay.h" 
#include "led.h" 
#include "key.h" 
#include "wdg.h" 
//ALIENTEK ������STM32F429������ ʵ��5
//�������Ź�ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
  
int main(void)
{   
	Stm32_Clock_Init(360,25,2,8);//����ʱ��,180Mhz
	delay_init(180);		//��ʼ����ʱ���� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();		  		//��ʼ������
 	delay_ms(100);			//��ʱ100ms�ٳ�ʼ�����Ź�,LED0�ı仯"�ɼ�"
	IWDG_Init(4,500);    	//Ԥ��Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	   
	LED0=0;				 	//����LED0
	while(1)
	{ 
		if(KEY_Scan(0)==WKUP_PRES)//���WK_UP����,��ι��
		{
			IWDG_Feed();//ι��
		}
		delay_ms(10);
	};
}

















