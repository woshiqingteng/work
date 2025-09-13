
.text
.global  _start
_start: 	
	/* 使能PLL4 */
	/* RCC_PLL4CR地址：0x50000000 + 0x894	*/
	LDR R0, =(0x50000000 + 0x894)
	LDR R1, [R0]
	ORR R1, R1, #(1<<0)
	STR R1, [R0]
NotReady:
	LDR R1, [R0]
	TST R1, #(1<<1)
	BEQ NotReady	

	/* 使能GPIOA */
	/*
	 *	RCC_MP_AHB4ENSETR地址：0x50000000 + 0xA28
	 * RCC_MC_AHB4ENSETR地址：0x50000000 + 0xAA8
	 */
	LDR R0, =(0x50000000 + 0xA28)
	LDR R1, [R0]
	ORR R1, R1, #(1<<0)
	STR R1, [R0]
	
	/* 设置PA10为GPIO模式, 输出模式 */
	/*
	 * GPIOA_MODER地址：0x50002000 + 0x00
	 */
	LDR R0, =(0x50002000 + 0x00)
	LDR R1, [R0]
	MOV R2, #(3<<20)
	BIC R1, R1, R2  /* R1 = R1 & ~R2 */
	ORR R1, R1, #(1<<20)
	STR R1, [R0]
	
	/*
	 * GPIOA_ODR地址： 0x50002000 + 0x14
	 */
	LDR R2, =(0x50002000 + 0x14)
Loop:
		
	/* 设置PA10输出1 */
	LDR R1, [R2]
	ORR R1, R1, #(1<<10)
	STR R1, [R2]

	LDR R0, =1000000
	BL Delay
	 
	/* 设置PA10输出0 */
	LDR R1, [R2]
	BIC R1, R1, #(1<<10)
	STR R1, [R2]
	
	LDR R0, =1000000
	BL Delay

	B Loop

Delay:
	SUBS R0, R0, #1
	BNE Delay
	MOV PC, LR
	