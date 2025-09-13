
#include "led.h"

void delay(volatile unsigned int d)
{
	while(d--);
}

static volatile unsigned int *RCC_PLL4CR = (volatile unsigned int *)(0x50000894);
static volatile unsigned int *RCC_MP_AHB4LPENSETR = (volatile unsigned int *)(0x50000A28);

int  main()
{
	//enable PLL4
	*RCC_PLL4CR |= 1;
	//wait PLL4 ready
	while(!(*RCC_PLL4CR & 0x2));
	//enable GPIOA clk
	*RCC_MP_AHB4LPENSETR |= 1;
	led_init();

	while(1)
	{
		led_ctl(1);
		delay(1000000);
		led_ctl(0);
		delay(1000000);
	}
					
	return 0;
}


