
#define A7

void key_init(void)
{
	/* 157里GPIO控制器里没有中断相关的寄存器 */
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

	/* 设置PG3为GPIO模式, 输入模式 
	 * GPIOG_MODER地址：0x50008000 + 0x00
	 */
	pRegKey = (volatile unsigned int *)(0x50008000 + 0x00);
	*pRegKey &= ~(3<<6);
}


