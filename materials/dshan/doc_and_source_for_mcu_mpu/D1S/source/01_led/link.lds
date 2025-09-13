
SECTIONS
{
	 /*
	  * 设置sbi的加载入口地址为0x20000
	  */
	. = 0x20000;
	text_begin = .;

	.text : { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
	PROVIDE(__bin_size = bss_begin - text_begin);
}
	
