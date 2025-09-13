	PRESERVE8
	THUMB

; Vector Table Mapped to Address 0 at Reset
	AREA    RESET, DATA, READONLY

__Vectors       DCD     0
	DCD     Reset_Handler              ; Reset Handler

	AREA    |.text|, CODE, READONLY
                
; Reset handler
Reset_Handler   PROC
				
	; 使能GPIOB 
	LDR R0, =(0x40021000 + 0x18)  ; RCC_APB2ENR
	LDR R1, [R0]
	ORR R1, R1, #(1<<3)
	STR R1, [R0]

	; 设置GPIOB0为输出引脚
	LDR R0, =(0x40010C00 + 0)  ; GPIOB_ODR
	LDR R1, [R0]
	ORR R1, R1, #(1<<0)
	STR R1, [R0]

MainLoop
	; 设置GPIIOB0输出高电平
	LDR R0, =(0x40010C00 + 0xc)  ; GPIOB_ODR
	LDR R1, [R0]
	ORR R1, R1, #(1<<0)
	STR R1, [R0]
	BL Delay

	; 设置GPIIOB0输出低电平
	LDR R0, =(0x40010C00 + 0xc)  ; GPIOB_ODR
	LDR R1, [R0]
	BIC R1, R1, #(1<<0)
	STR R1, [R0]
	BL Delay
	B MainLoop

Delay
	LDR R0, =300000
DelayLoop
	SUBS R0, #1
	BNE DelayLoop
	BX LR
	NOP
	ENDP
	END