#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "timer.h"
#include "sdram.h"
#include "math.h"
#include "arm_math.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��49_2
 DSP FFTʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#define FFT_LENGTH		1024 		//FFT���ȣ�Ĭ����1024��FFT

float fft_inputbuf[FFT_LENGTH*2];	//FFT��������
float fft_outputbuf[FFT_LENGTH];	//FFT�������

u8 timeout;
int main(void)
{
    arm_cfft_radix4_instance_f32 scfft;
 	u8 key,t=0;
	float time; 
	u8 buf[50]; 
	u16 i; 
    
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART	
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    LCD_Init();                     //��ʼ��LCD
    TIM3_Init(65535,90-1);          //10Khz����Ƶ��,����ʱ65ms�볬��
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"DSP FFT TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/17");	
	LCD_ShowString(30,130,200,16,16,"KEY0:Run FFT");//��ʾ��ʾ��Ϣ 
	LCD_ShowString(30,160,200,16,16,"FFT runtime:");//��ʾFFTִ��ʱ��
 	POINT_COLOR=BLUE;		//��������Ϊ��ɫ   
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);//��ʼ��scfft�ṹ�壬�趨FFT��ز���
    while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			for(i=0;i<FFT_LENGTH;i++)//�����ź�����
			{
				 fft_inputbuf[2*i]=100+
				                   10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
								   30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
				                   50*arm_cos_f32(2*PI*i*8/FFT_LENGTH);	//���������ź�ʵ��
				 fft_inputbuf[2*i+1]=0;//�鲿ȫ��Ϊ0
			}
			__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//����TIM3��ʱ���ļ�����ֵ
			timeout=0;
			arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT���㣨��4��
            time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)timeout*65536;//��������ʱ��
			sprintf((char*)buf,"%0.3fms\r\n",time/1000);	
			LCD_ShowString(30+12*8,160,100,16,16,buf);	//��ʾ����ʱ��		
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//��������������ģ�÷�ֵ 
			printf("\r\n%d point FFT runtime:%0.3fms\r\n",FFT_LENGTH,time/1000);
			printf("FFT Result:\r\n");
			for(i=0;i<FFT_LENGTH;i++)
			{
				printf("fft_outputbuf[%d]:%f\r\n",i,fft_outputbuf[i]);
			}
		}else delay_ms(10);
		t++;
		if((t%10)==0)LED0=!LED0;		  
	}
}
