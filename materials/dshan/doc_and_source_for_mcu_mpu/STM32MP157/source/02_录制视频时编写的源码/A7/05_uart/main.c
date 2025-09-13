#include "uart.h"

#define A7

void delay(volatile unsigned int d)
{
	while (d--);
}

int main(void)
{
	char c;
	
	uart_init();
	putchar('1');
	putchar('0');
	putchar('0');
	putchar('a');
	putchar('s');
	putchar('k');
	putchar('\r');
	putchar('\n');

	while (1)
	{
		c = getchar();
		putchar(c);
		putchar(c+1);
	}
	
	return 0;
}

