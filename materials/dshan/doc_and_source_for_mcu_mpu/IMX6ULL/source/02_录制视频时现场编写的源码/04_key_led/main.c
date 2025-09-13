
void delay(volatile int d)
{
	while(d--);
}

int main(void)
{
	 volatile unsigned int *pRegLed;
	 volatile unsigned int *pRegKey;
	 
	/* 使能GPIO5: 默认使能 */
	/* 使能GPIO4, CCM_CCGR3, b[13:12]=0b11 */
	pRegKey = (volatile unsigned int *)(0x020C4074);
	*pRegKey |= (3<<12);

	/* 把GPIO5_3设置为GPIO功能 */
	pRegLed = (volatile unsigned int *)(0x02290000 + 0x14);
	*pRegLed |= (0x5);

	/* 把GPIO4_14设置为GPIO功能
	 * IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B   地址：20E_0000h base + 1B0h offset = 0x020E01B0
	 */
	pRegKey = (volatile unsigned int *)(0x020E01B0);
	*pRegLed &= ~(0xf);
	*pRegLed |= (0x5);

	/* 把GPIO5_3设置为输出引脚 */
	pRegLed = (volatile unsigned int *)(0x020AC004);
	*pRegLed |= (1<<3);

	/* 把GPIO4_14设置为输入引脚 
	 * GPIO4_GDIR地址：0x020A8004 
	 */
	pRegKey = (volatile unsigned int *)(0x020A8004);
	*pRegLed &= ~(1<<14);

	/* GPIO5_DR */
	pRegLed = (volatile unsigned int *)(0x020AC000);

	/* GPIO4_DR */
	pRegKey = (volatile unsigned int *)(0x020A8000);

	while (1)
	{
		/* 读取GPIO4_14引脚 */
		if ((*pRegKey & (1<<14)) == 0) /* 被按下 */
		{
			/* 设置GPIO5_3输出0 */
			*pRegLed &= ~(1<<3);
		}
		else
		{		
			/* 设置GPIO5_3输出1 */
			*pRegLed |= (1<<3);
		}
	}

	return 0;
}

