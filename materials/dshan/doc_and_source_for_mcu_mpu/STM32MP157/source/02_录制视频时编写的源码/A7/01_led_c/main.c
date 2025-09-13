
#define A7

void delay(volatile unsigned int d)
{
	while (d--);
}

int main(void)
{
	volatile unsigned int *pReg;
	
	/* 使能PLL4 */
	/* RCC_PLL4CR地址：0x50000000 + 0x894	*/
	pReg = (volatile unsigned int *)(0x50000000 + 0x894);
	*pReg |= (1<<0);
	while(*pReg & (1<<1) == 0);

	/* 使能GPIOA */
	/*
	 *	RCC_MP_AHB4ENSETR地址：0x50000000 + 0xA28
	 * RCC_MC_AHB4ENSETR地址：0x50000000 + 0xAA8
	 */
#ifdef A7	 
	/* for A7 */
	pReg = (volatile unsigned int *)(0x50000000 + 0xA28);
	*pReg |= (1<<0);
#else
	/* for M4 */
	pReg = (volatile unsigned int *)(0x50000000 + 0xAA8);
	*pReg |= (1<<0);
#endif
	/* 设置PA10为GPIO模式, 输出模式 */
	/*
	 * GPIOA_MODER地址：0x50002000 + 0x00
	 */
	pReg = (volatile unsigned int *)(0x50002000 + 0x00);
	*pReg &= ~(3<<20);
	*pReg |= (1<<20);
	
	/*
	 * GPIOA_ODR地址： 0x50002000 + 0x14
	 */
	pReg = (volatile unsigned int *)(0x50002000 + 0x14);
	while (1)
	{
		/* 设置PA10输出1 */
		*pReg |= (1<<10);
		delay(1000000);

		/* 设置PA10输出0 */
		*pReg &= ~(1<<10);
		delay(1000000);
	}

	return 0;
}

