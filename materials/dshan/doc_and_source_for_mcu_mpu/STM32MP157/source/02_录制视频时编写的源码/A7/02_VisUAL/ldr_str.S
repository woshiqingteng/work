		MOV		R0, #0x20000
		MOV		R1, #0x10
		MOV		R2, #0x12
		STR		R2, [R0]              ; R2的值存到R0所示地址
		STR		R2, [R0, #4]          ; R2的值存到R0+4所示地址
		STR		R2, [R0, #8]!         ; R2的值存到R0+8所示地址, R0=R0+8
		STR		R2, [R0, R1]          ; R2的值存到R0+R1所示地址
		STR		R2, [R0, R1, LSL #4]  ; R2的值存到R0+(R1<<4)所示地址
		STR		R2, [R0], #0X20       ; R2的值存到R0所示地址, R0=R0+0x20
		MOV		R2, #0x34
		STR		R2, [R0]              ; R2的值存到R0所示地址
		LDR		R3, [R0], +R1, LSL #1 ; R3的值等于R0+(R1<<1)所示地址上的值