
void delay(int d)
{
	while(d--);
}

int main() 
{
	unsigned int *pReg;
	unsigned int *pRegA;
	unsigned int *pRegB;
	
	/* 使能GPIOB, GPIOA */
	pReg = (unsigned int *)(0x40021000 + 0x18);
	*pReg |= (1<<3) | (1<<2);
	
	/* 设置GPIOB0为输出引脚 */
	pRegB = (unsigned int *)(0x40010C00 + 0x00);
	*pRegB |= (1<<0);

	/* 设置GPIOA0为输入引脚 */
	pRegA = (unsigned int *)(0x40010800 + 0x00);
	*pRegA &= ~(3);     /* mode0 = 0b00 */
	*pRegA &= ~(3<<2);  /* cnf0 = 0b00 */
	*pRegA |= (1<<2);   /* cnf0 = 0b01 */

	/* GPIOB output data register */
	pRegB = (unsigned int *)(0x40010C00 + 0x0C);

	/* GPIOA input data register */
	pRegA = (unsigned int *)(0x40010800 + 0x08);

	while (1)
	{
		/* 读取GPIOA input data register */
		
		if ((*pRegA & (1<<0)) == 0) /* KEY1被按下 */
		{
			/* 设置GPIOB0输出0 */
			*pRegB &= ~(1<<0);
		}
		else
		{
			/* 设置GPIOB0输出1 */
			*pRegB |= (1<<0);
		}
	}
	
	return 0;
}
