#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��9
 ���벶��ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ 

int main(void)
{
    long long temp=0;  
    HAL_Init();                     	//��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   	//����ʱ��,180Mhz
    delay_init(180);               	 	//��ʼ����ʱ����
    uart_init(115200);             		//��ʼ��USART
    LED_Init();                     	//��ʼ��LED 
    TIM3_PWM_Init(500-1,90-1);      	//90M/90=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ500����ôPWMƵ��Ϊ1M/500=2kHZ
    TIM5_CH1_Cap_Init(0XFFFFFFFF,90-1); //��1MHZ��Ƶ�ʼ���
    while(1)
    {
        delay_ms(10);
		TIM_SetTIM3Compare4(TIM_GetTIM3Capture4()+1); 
		if(TIM_GetTIM3Capture4()==300)TIM_SetTIM3Compare4(0);        
        if(TIM5CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 	    //���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%lld us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;          //������һ�β���
		}
    }
}

