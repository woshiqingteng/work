		MOV		R1, #1
		MOV		R2, #2
		MOV		R3, #3
		MOV		R0, #0x20000
		STMIA	R0,		{R1-R3} ; R1,R2,R3分别存入R0,R0+4,R0+8地址处
		ADD		R0, R0, #0x10
		STMIA	R0!, {R1-R3} ; R1,R2,R3分别存入R0,R0+4,R0+8地址处, R0=R0+3*4
