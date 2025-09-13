		ADR		LR, Ret    ; 伪指令，读取Ret标号的地址赋给LR，这是返回地址
		ADR		PC, Delay  ; 伪指令，读取Delay标号的地址赋给PC，直接跳转
Ret
		MOV		R1, #1
Delay
		MOV		R0, #1000
Loop
		SUBS		R0, R0, #1
		BNE		Loop
		MOV		PC, LR    ; 把LR赋给PC，返回
