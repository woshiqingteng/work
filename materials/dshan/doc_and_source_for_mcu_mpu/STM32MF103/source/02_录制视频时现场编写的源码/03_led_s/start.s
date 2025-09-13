
                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
				EXPORT  __Vectors
					
__Vectors       DCD     0                  
                DCD     Reset_Handler              ; Reset Handler

				AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler   PROC
				EXPORT  Reset_Handler             [WEAK]

				; enable GPIOB
				LDR R0, =(0x40021000 + 0x18)
				LDR R1, [R0]
				ORR R1, R1, #(1<<3)
				STR R1, [R0]
				
				; set GPIOB0 as output
				LDR R0, =(0x40010C00 + 0x00)
				LDR R1, [R0]
				ORR R1, R1, #(1<<0)
				STR R1, [R0]
				
				LDR R2, =(0x40010C00 + 0x0C)
Loop				
				; set GPIOB0 output hight
				LDR R1, [R2]
				ORR R1, R1, #(1<<0)
				STR R1, [R2]
				
				LDR R0, =100000
				BL delay
				
				; set GPIOB0 output low
				LDR R1, [R2]
				BIC R1, R1, #(1<<0)
				STR R1, [R2]
				
				LDR R0, =100000
				BL delay
				
				B Loop
                ENDP

delay
				SUBS R0, R0, #1
				BNE delay
				MOV PC, LR

                
                 END

