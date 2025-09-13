
void delay(volatile int d)
{
	while(d--);
}

int main(void)
{
	 volatile unsigned int *pReg;
	 
	/* 使能GPIO5: 默认使能 */

	/* 把GPIO5_3设置为GPIO功能 */
	pReg = (volatile unsigned int *)(0x02290000 + 0x14);
	*pReg |= (0x5);

	/* 把GPIO5_3设置为输出引脚 */
	pReg = (volatile unsigned int *)(0x020AC004);
	*pReg |= (1<<3);

	pReg = (volatile unsigned int *)(0x020AC000);

	while (1)
	{
		/* 设置GPIO5_3输出1 */
		*pReg |= (1<<3);
		delay(1000000);

		/* 设置GPIO5_3输出0 */
		*pReg &= ~(1<<3);
		delay(1000000);		
	}

	return 0;
}

