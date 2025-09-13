		BL		Delay    ; 跳转前把返回地址保持在LR寄存器里
		MOV		R1, #1
Delay
		MOV		R0, #1000
Loop
		SUBS		R0, R0, #1
		BNE		Loop
		MOV		PC, LR    ; 把LR赋给PC，返回
