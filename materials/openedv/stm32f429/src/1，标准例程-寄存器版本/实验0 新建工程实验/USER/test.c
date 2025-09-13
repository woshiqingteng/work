#include "sys.h" 
#include "usart.h" 
#include "delay.h" 
//ALIENTEK 阿波罗STM32F429开发板 实验0
//新建工程实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

int main(void)
{ 
	u8 t=0;
	Stm32_Clock_Init(360,25,2,8);	//设置时钟,180Mhz
	delay_init(180);				//初始化延时函数
	uart_init(90,115200);			//串口初始化为115200
	while(1)
	{
		printf("t:%d\r\n",t);
		delay_ms(500);
		t++;
	}
}
