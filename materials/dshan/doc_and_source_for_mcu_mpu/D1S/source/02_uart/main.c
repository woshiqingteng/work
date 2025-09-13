
#include <uart.h>
#include <clock.h>


void delay(volatile int n)
{
	while (n--);
}

int main(void)
{
	int i = 0;

	//sys_clock_init();
	uart_init();
	
	while (1)
	{
		sys_uart_printf("%02d www.100ask.net\r\n", i++);
		/* delay */
		delay(1000000);
	}

	return 0;
}

