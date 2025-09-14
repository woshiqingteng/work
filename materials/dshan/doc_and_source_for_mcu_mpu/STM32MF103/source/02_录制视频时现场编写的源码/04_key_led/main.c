
void delay(int d)
{
	while(d--);
}

int main() 
{
	unsigned int *pReg;
	unsigned int *pRegA;
	unsigned int *pRegB;
	
	/* ʹ��GPIOB, GPIOA */
	pReg = (unsigned int *)(0x40021000 + 0x18);
	*pReg |= (1<<3) | (1<<2);
	
	/* ����GPIOB0Ϊ������� */
	pRegB = (unsigned int *)(0x40010C00 + 0x00);
	*pRegB |= (1<<0);

	/* ����GPIOA0Ϊ�������� */
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
		/* ��ȡGPIOA input data register */
		
		if ((*pRegA & (1<<0)) == 0) /* KEY1������ */
		{
			/* ����GPIOB0���0 */
			*pRegB &= ~(1<<0);
		}
		else
		{
			/* ����GPIOB0���1 */
			*pRegB |= (1<<0);
		}
	}
	
	return 0;
}
