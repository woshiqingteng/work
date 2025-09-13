
.text
.global  _start
_start: 	

	/* 使能GPIO模块 */

	/* 把GPIO5_3设置为GPIO功能 */
	LDR R0, =(0x02290000 + 0x14)
	LDR R1, [R0]
	ORR R1, R1, #0x5
	STR R1, [R0]
	
	/* 设置引脚为输出 */

	/* 把GPIO5_3设置为输出引脚 */
	LDR R0, =0x020AC004
	LDR R1, [R0]
	ORR R1, R1, #(1<<3)
	STR R1, [R0]

	/* DATA out register */
	LDR R2, =(0x020AC000)
	
Loop:
		
	/* 设置GPIO5_3输出1 */
	LDR R1, [R2]
	ORR R1, R1, #(1<<3)
	STR R1, [R2]

	LDR R0, =1000000
	BL Delay
	 
	/* 设置引脚输出低电平 */
	LDR R1, [R2]
	BIC R1, R1, #(1<<3)
	STR R1, [R2]
	
	LDR R0, =1000000
	BL Delay

	b Loop

Delay:
	SUBS R0, R0, #1
	BNE Delay
	MOV PC, LR
	