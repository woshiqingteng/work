
void delay(int d)
{
	while(d--);
}

int main()
{
	unsigned int *pReg;
	
	/* 使能GPIOB */
	pReg = (unsigned int *)(0x40021000 + 0x18);
	*pReg |= (1<<3);
	
	/* 设置GPIOB0为输出引脚 */
	pReg = (unsigned int *)(0x40010C00 + 0x00);
	*pReg |= (1<<0);

	pReg = (unsigned int *)(0x40010C00 + 0x0C);
	
	while (1)
	{
		/* 设置GPIOB0输出1 */
		*pReg |= (1<<0);
		
		delay(100000);

		/* 设置GPIOB0输出0 */
		*pReg &= ~(1<<0);
		
		delay(100000);		
	}
	
	return 0;
}
