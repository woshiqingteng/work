#include "sys.h"
#include "delay.h"
#include "usart.h"

/************************************************
 ALIENTEK ������STM32F429������ʵ��0-2
 Template����ģ��-�����½�ʹ��-HAL��汾
 ����֧����̳��www.openedv.com
 �Ա����̣�    http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

/*ע�⣺�����̺ͽ̳�3.4С�ڳ������ص��Թ��̶�Ӧ*/

int main(void)
{
    u8 t=0;
     
    HAL_Init();                     //��ʼ��HAL��    
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
	  while(1)
	{
	 	printf("t:%d\r\n",t);
		delay_ms(500);
		t++;
	}
}


