
void delay(volatile int n)
{
	while (n--);
}

int main(void)
{
	volatile unsigned int *p;
	unsigned int val;
	
	/* 1. 配置PE1为output 
     * PE_CFG0 : PE Configure Register 0
     * Offset: 0x00C0
     * Base addr: 0x02000000
     */
	p = (volatile unsigned int *)(0x02000000+0x00C0);
	val = *p;
	val &= ~(0xf<<4);
	val |= (1<<4);
	*p = val;

	/* 
     * PE_DAT : PE1 data register
     * Offset: 0x00D0
     * Base addr: 0x02000000
     */
	p = (volatile unsigned int *)(0x02000000+0x00D0);

	while (1)
	{
		/* 让PE1输出1 */
		*p |= (1<<1);

		/* delay */
		delay(1000000);

		/* 让PE1输出0 */
		*p &= ~(1<<1);

		/* delay */
		delay(1000000);
	}

	return 0;
}

