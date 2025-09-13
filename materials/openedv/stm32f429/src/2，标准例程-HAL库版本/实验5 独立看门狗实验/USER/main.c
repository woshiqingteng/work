#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "iwdg.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��5
 �������Ź�ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


int main(void)
{
    HAL_Init();                    	 	//��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   	//����ʱ��,180Mhz
    delay_init(180);                	//��ʼ����ʱ����
    uart_init(115200);              	//��ʼ��USART
    LED_Init();                     	//��ʼ��LED 
    KEY_Init();                     	//��ʼ������
    delay_ms(100);                  	//��ʱ100ms�ٳ�ʼ�����Ź�,LED0�ı仯"�ɼ�"
    IWDG_Init(IWDG_PRESCALER_64,500);  	//��Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
    LED0=0;
    while(1)
    {
        if(KEY_Scan(0)==WKUP_PRES)  //���WK_UP���£�ι��
        {
            IWDG_Feed();    //ι��
        }
        delay_ms(10);  
    }
}

