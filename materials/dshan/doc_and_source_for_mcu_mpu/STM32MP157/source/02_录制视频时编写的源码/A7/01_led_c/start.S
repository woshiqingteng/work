.text
.global  _start
_start: 				

	/* 设置sp */
	ldr sp, =0xc0000000 + 0x100000

	/* 调用main函数 */
	bl main

