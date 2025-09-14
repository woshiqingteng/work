
void delay(int d)
{
	while(d--);
}

int main()
{
	unsigned int *pReg;
	
	/* ʹ��GPIOB */
	pReg = (unsigned int *)(0x40021000 + 0x18);
	*pReg |= (1<<3);
	
	/* ����GPIOB0Ϊ������� */
	pReg = (unsigned int *)(0x40010C00 + 0x00);
	*pReg |= (1<<0);

	pReg = (unsigned int *)(0x40010C00 + 0x0C);
	
	while (1)
	{
		/* ����GPIOB0���1 */
		*pReg |= (1<<0);
		
		delay(100000);

		/* ����GPIOB0���0 */
		*pReg &= ~(1<<0);
		
		delay(100000);		
	}
	
	return 0;
}
