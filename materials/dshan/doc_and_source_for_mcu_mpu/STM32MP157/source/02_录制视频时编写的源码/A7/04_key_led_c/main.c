
#define A7

void delay(volatile unsigned int d)
{
	while (d--);
}

int main(void)
{
	volatile unsigned int *pReg;
	volatile unsigned int *pRegLed;
	volatile unsigned int *pRegKey;
	
	/* 使能PLL4 */
	/* RCC_PLL4CR地址：0x50000000 + 0x894	*/
	pReg = (volatile unsigned int *)(0x50000000 + 0x894);
	*pReg |= (1<<0);
	while((*pReg & (1<<1)) == 0);

	/* 使能GPIOA, GPIOG */
	/*
	 *	RCC_MP_AHB4ENSETR地址：0x50000000 + 0xA28
	 * RCC_MC_AHB4ENSETR地址：0x50000000 + 0xAA8
	 */
#ifdef A7	 
	/* for A7 */
	pReg = (volatile unsigned int *)(0x50000000 + 0xA28);
	*pReg |= (1<<0) | (1<<6);
#else
	/* for M4 */
	pReg = (volatile unsigned int *)(0x50000000 + 0xAA8);
	*pReg |= (1<<0) | (1<<6);
#endif
	/* 设置PA10为GPIO模式, 输出模式 */
	/*
	 * GPIOA_MODER地址：0x50002000 + 0x00
	 */
	pRegLed = (volatile unsigned int *)(0x50002000 + 0x00);
	*pRegLed &= ~(3<<20);
	*pRegLed |= (1<<20);

	/* 设置PG3为GPIO模式, 输入模式 
	 * GPIOG_MODER地址：0x50008000 + 0x00
	 */
	pRegKey = (volatile unsigned int *)(0x50008000 + 0x00);
	*pRegKey &= ~(3<<6);
	
	/*
	 * GPIOA_ODR地址： 0x50002000 + 0x14
	 */
	pRegLed = (volatile unsigned int *)(0x50002000 + 0x14);

	/*
	 * GPIOG_IDR地址：0x50008000 + 0x10
	 */
	pRegKey = (volatile unsigned int *)(0x50008000 + 0x10);
	
	while (1)
	{
		/* 读取PG3电平 */
		if ((*pRegKey & (1<<3)) == 0) /* 被按下 */
		{
			/* 设置PA10输出0 */
			*pRegLed &= ~(1<<10);
		}
		else
		{
			/* 设置PA10输出1 */
			*pRegLed |= (1<<10);
		}		
	}

	return 0;
}

