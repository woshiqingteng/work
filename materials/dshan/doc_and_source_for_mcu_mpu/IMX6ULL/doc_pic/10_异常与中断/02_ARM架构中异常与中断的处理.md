# ARM架构中异常与中断的处理
## 1.1 处理流程是一样的

* 每执行完一条指令都会检查有无中断/异常产生
* 发现有中断/异常产生，开始处理：
  * 保存现场
  * 分辨异常/中断，调用对应的异常/中断处理函数
  * 恢复现场



不同的芯片，不同的架构，在这方面的处理稍有差别：

* CPU中止当前执行，跳转去执行处理异常的代码：也有差异
  * cortex M3/M4在向量表上放置的是函数地址
  * cortex A7在向量表上放置的是跳转指令

* 保存/恢复现场：cortex M3/M4是硬件实现的，cortex A7是软件实现的

  

## 1.2 cortex M3/M4

参考资料：`DDI0403E_B_armv7m_arm.pdf`、`ARM Cortex-M3与Cortex-M4权威指南.pdf`、`PM0056.pdf`

要想理解这个处理流程，需要从向量表说起。
向量，在数学定义里是**有方向的量**，在程序里可以认为向量就是一个数组，里面有多个项。
在ARM架构里，对于异常/中断，它们的**处理入口**会整齐地排放在一起。

### 1.2.1 M3/M4的向量表

M3/M4的向量表中，放置的是具体异常/中断的处理函数的地址。
比如发生`Reset`异常时，CPU就会从向量表里找到第1项，得到Reset_Handler函数的地址，跳转去执行。
比如发生`EXTI Line 0`中断时，CPU就会从向量表里找到第22项，得到EXTI0_IRQHandler函数的地址，跳转去执行。

* 跳转之前，硬件会保存现场
* 函数执行完毕，返回之后，硬件会恢复现场

```
; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler            ; Window Watchdog
                DCD     PVD_IRQHandler             ; PVD through EXTI Line detect
                DCD     TAMPER_IRQHandler          ; Tamper
                DCD     RTC_IRQHandler             ; RTC
                DCD     FLASH_IRQHandler           ; Flash
                DCD     RCC_IRQHandler             ; RCC
                DCD     EXTI0_IRQHandler           ; EXTI Line 0
                DCD     EXTI1_IRQHandler           ; EXTI Line 1
                DCD     EXTI2_IRQHandler           ; EXTI Line 2
                DCD     EXTI3_IRQHandler           ; EXTI Line 3
                DCD     EXTI4_IRQHandler           ; EXTI Line 4
                DCD     DMA1_Channel1_IRQHandler   ; DMA1 Channel 1
                DCD     DMA1_Channel2_IRQHandler   ; DMA1 Channel 2
                DCD     DMA1_Channel3_IRQHandler   ; DMA1 Channel 3
                DCD     DMA1_Channel4_IRQHandler   ; DMA1 Channel 4
                DCD     DMA1_Channel5_IRQHandler   ; DMA1 Channel 5
                DCD     DMA1_Channel6_IRQHandler   ; DMA1 Channel 6
                DCD     DMA1_Channel7_IRQHandler   ; DMA1 Channel 7
                DCD     ADC1_2_IRQHandler          ; ADC1 & ADC2
                DCD     USB_HP_CAN1_TX_IRQHandler  ; USB High Priority or CAN1 TX
                DCD     USB_LP_CAN1_RX0_IRQHandler ; USB Low  Priority or CAN1 RX0
                DCD     CAN1_RX1_IRQHandler        ; CAN1 RX1
                DCD     CAN1_SCE_IRQHandler        ; CAN1 SCE
                DCD     EXTI9_5_IRQHandler         ; EXTI Line 9..5
                DCD     TIM1_BRK_IRQHandler        ; TIM1 Break
                DCD     TIM1_UP_IRQHandler         ; TIM1 Update
                DCD     TIM1_TRG_COM_IRQHandler    ; TIM1 Trigger and Commutation
                DCD     TIM1_CC_IRQHandler         ; TIM1 Capture Compare
                DCD     TIM2_IRQHandler            ; TIM2
                DCD     TIM3_IRQHandler            ; TIM3
                DCD     TIM4_IRQHandler            ; TIM4
                DCD     I2C1_EV_IRQHandler         ; I2C1 Event
                DCD     I2C1_ER_IRQHandler         ; I2C1 Error
                DCD     I2C2_EV_IRQHandler         ; I2C2 Event
                DCD     I2C2_ER_IRQHandler         ; I2C2 Error
                DCD     SPI1_IRQHandler            ; SPI1
                DCD     SPI2_IRQHandler            ; SPI2
                DCD     USART1_IRQHandler          ; USART1
                DCD     USART2_IRQHandler          ; USART2
                DCD     USART3_IRQHandler          ; USART3
                DCD     EXTI15_10_IRQHandler       ; EXTI Line 15..10
                DCD     RTCAlarm_IRQHandler        ; RTC Alarm through EXTI Line
                DCD     USBWakeUp_IRQHandler       ; USB Wakeup from suspend
                DCD     TIM8_BRK_IRQHandler        ; TIM8 Break
                DCD     TIM8_UP_IRQHandler         ; TIM8 Update
                DCD     TIM8_TRG_COM_IRQHandler    ; TIM8 Trigger and Commutation
                DCD     TIM8_CC_IRQHandler         ; TIM8 Capture Compare
                DCD     ADC3_IRQHandler            ; ADC3
                DCD     FSMC_IRQHandler            ; FSMC
                DCD     SDIO_IRQHandler            ; SDIO
                DCD     TIM5_IRQHandler            ; TIM5
                DCD     SPI3_IRQHandler            ; SPI3
                DCD     UART4_IRQHandler           ; UART4
                DCD     UART5_IRQHandler           ; UART5
                DCD     TIM6_IRQHandler            ; TIM6
                DCD     TIM7_IRQHandler            ; TIM7
                DCD     DMA2_Channel1_IRQHandler   ; DMA2 Channel1
                DCD     DMA2_Channel2_IRQHandler   ; DMA2 Channel2
                DCD     DMA2_Channel3_IRQHandler   ; DMA2 Channel3
                DCD     DMA2_Channel4_5_IRQHandler ; DMA2 Channel4 & Channel5
__Vectors_End
```



### 1.2.2 M3/M4的异常/中断处理流程

发生异常/中断时，硬件上实现了这些事情：

* 保存现场：把被中断瞬间的寄存器的值保存进栈里

* 根据异常/中断号，从向量表中得到**函数地址**，跳转过去执行

* 函数执行完后，从栈中恢复现场

保存现场、分辨异常/中断、跳转执行，都是硬件实现的。
我们只需要在向量表中，把处理函数的地址填进去就可以了。

**硬件**承包了大部分的工作。

M3/M4的向量表中，存放的是**函数地址**。



## 1.3 cortex A7

参考资料：`ARM ArchitectureReference Manual ARMv7-A and ARMv7-R edition.pdf`

实际上，以前的S3C2440属于ARM9处理器，它的异常/中断处理流程给cortex A7是一样的。

### 1.3.1 A7的向量表

A7的向量表中，放置的是某类异常的**跳转指令**。
比如发生`Reset`异常时，CPU就会从向量表里找到第0项，得到`b reset`指令，执行后就跳转到reset函数。
比如发生任何的中断时，CPU就会从向量表里找到第6项，得到`ldr	pc, _irq`指令，执行后就跳转到_irq函数。

* 跳转之前，硬件只会保存CPSR寄存器
* 跳转之后，软件要保存现场
* 函数执行完毕，返回之前，软件恢复现场

```
_start: 
    b	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq
```



### 1.3.2 A7的异常/中断处理流程

发生异常/中断时，硬件上实现了这些事情：

* CPU切换到对应的异常模式，比如IRQ模式、未定义模式、SVC模式
* 保存被中断时的CPSR到SPSR
  * CPSR：current program status register，当前程序状态寄存器
  * SRSR：saved program status register，保存的程序状态寄存器

* 跳到这个异常的入口地址去，执行**指令**，这通常是一条跳转指令

软件要做的事情就比较多了：

* 保存现场
* 分辨异常/中断
* 调用对应的处理函数
* 恢复现场

A7的向量表中，存放的是**跳转指令**。

### 

