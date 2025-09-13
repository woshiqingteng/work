#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "timer.h"
#include "sdram.h"
#include "malloc.h"
#include "touch.h"
#include "GUI.h"
#include "WM.h"
#include "GUIDEMO.h"

/************************************************
 ALIENTEK STM32������STemWinʵ��
 STemWin�޲���ϵͳ��ֲʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz   
    HAL_Init();                     //��ʼ��HAL��
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //SDRAM��ʼ��
    TFTLCD_Init();  		        //LCD��ʼ��
    TP_Init();				        //��������ʼ��
    TIM3_Init(999,89);              //1KHZ ��ʱ��3����Ϊ1ms
    TIM4_Init(999,899);             //������ɨ���ٶ�,100HZ.
    my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ��
	__HAL_RCC_CRC_CLK_ENABLE();		//ʹ��CRCʱ��
	WM_SetCreateFlags(WM_CF_MEMDEV);//����STemWin�洢�豸
	GUI_Init();
    WM_MULTIBUF_Enable(1);			//����STemWin�໺�壬RGB�����ܻ��õ�
	GUIDEMO_Main();					//STemWin ��ʾdemo
    while(1)
    {
	}
}
