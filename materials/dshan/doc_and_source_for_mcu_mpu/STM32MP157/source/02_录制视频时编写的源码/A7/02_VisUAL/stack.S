		MOV		R1, #1
		MOV		R2, #2
		MOV		R3, #3
		MOV		SP, #0x20000
		STMFD	SP!, {R1-R3} ; R3,R2,R1分别存入SP-4,SP-8,SP-12地址处, SP=SP-12
		MOV		R1, #0
		MOV		R2, #0
		MOV		R3, #0
		LDMFD	SP!, {R1-R3} ; R1,R2,R3分别得到SP,SP+4,SP+8地址处的值, SP=SP+12
