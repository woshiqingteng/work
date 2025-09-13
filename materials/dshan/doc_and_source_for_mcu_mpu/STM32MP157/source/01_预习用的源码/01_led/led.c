
#include "led.h"

static volatile unsigned int *GPIOA_MODER                               ;
static volatile unsigned int *GPIOA_OTYPER                               ;
static volatile unsigned int *GPIOA_ODR                               ;
static volatile unsigned int *GPIOA_AFRL                               ;
static volatile unsigned int *GPIOA_AFRH                               ;

void led_init(void)
{
	unsigned int val;
	
	GPIOA_MODER                                = (volatile unsigned int *)(0x50002000);
	GPIOA_OTYPER                               = (volatile unsigned int *)(0x50002004);
	GPIOA_ODR                                  = (volatile unsigned int *)(0x50002014);
	GPIOA_AFRL                                 = (volatile unsigned int *)(0x50002020);
	GPIOA_AFRH                                 = (volatile unsigned int *)(0x50002024);

	//01: General purpose output mode
	val = *GPIOA_MODER;
	val &= ~(3 << 20);
	val |= (1 << 20);
	*GPIOA_MODER = val; 

	val = *GPIOA_ODR;
	val &= ~(1 << 10);
	*GPIOA_ODR = val;

	val = *GPIOA_AFRH;
	val &= ~(0xF << 8);
	*GPIOA_AFRH = val;
}

void led_ctl(int on)
{
	if (on) /* on: output 0*/
	{
		/* d. 设置GPIO5_DR输出低电平
		 * set GPIO5_DR to configure GPIO5_IO03 output 0
		 * GPIO5_DR 0x020AC000 + 0
		 * bit[3] = 0b0
		 */
		*GPIOA_ODR &= ~(1 << 10);
	}
	else  /* off: output 1*/
	{
		/* e. 设置GPIO5_IO3输出高电平
		 * set GPIO5_DR to configure GPIO5_IO03 output 1
		 * GPIO5_DR 0x020AC000 + 0
		 * bit[3] = 0b1
		 */ 
		*GPIOA_ODR |= (1 << 10);
	}
}




	
	
