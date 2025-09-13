#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tpad.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��10
 ���ݴ�������ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 

int main(void)
{
    u8 t=0; 
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    TPAD_Init(2);                   //��ʼ����������,��90/8=11.25MhzƵ�ʼ���
    while(1)
    {
        if(TPAD_Scan(0))    //�ɹ�������һ��������(�˺���ִ��ʱ������15ms)
        {
            LED1=!LED1;    	//LED1ȡ��
        }
        t++;
        if(t==15)
        {
            t=0;
            LED0=!LED0;
        }
        delay_ms(10);
    }
}

