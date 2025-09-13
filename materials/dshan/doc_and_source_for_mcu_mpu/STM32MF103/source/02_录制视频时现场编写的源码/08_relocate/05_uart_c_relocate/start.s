
                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
				EXPORT  __Vectors
					
					
__Vectors       DCD     0                  
                DCD     0x08000009; //Reset_Handler              ; Reset Handler

				AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler   PROC
				EXPORT  Reset_Handler             [WEAK]
                IMPORT  mymain

				IMPORT SystemInit

				LDR SP, =(0x20000000+0x10000)
				
				BL SystemInit

				;BL mymain
				LDR R0, =mymain
				BLX R0

                ENDP
                
                 END


