.text
.global  _start
_start: 				

	/* 设置sp */
	/* 对于STM32MP157设置链接地址为0xC0200000, 对于IMX6ULL设为0x80200000 */
	ldr sp, =0xc0000000 + 0x100000

	adr r0, _start
	bl SystemInit
	
	/* 调用main函数 */
	//bl main
	ldr pc, =main

