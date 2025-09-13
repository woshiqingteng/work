#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x_lib.h"

#define SYS_FREQ                (72000000)  // 系统频率

#define TIMx                    TIM3        // 目标定时器
#define TIMx_PERIOD             (100-1)       // 定时器的周期，单位：us
#define TIMx_PRE_LOAD_VALUE     (1024-1)    // 定时器的计数预装载值，范围：0~65535
#define TIMx_PRESCALER          (SYS_FREQ/1000000*TIMx_PERIOD/TIMx_PRE_LOAD_VALUE)           // 定时器预分频系数，范围：0~65535
#define TIMx_DIV                (TIM_CKD_DIV1)
#define TIMx_Clk_Enable()       RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE )   // 目标定时器的时钟使能

#define TIMx_IRQ                TIM3_IRQChannel     // 目标中断
#define TIMx_NVIC_PRE_PRIORITY  (0)             // 中断的抢占优先级等级
#define TIMx_NVIC_SUB_PRIORITY  (1)             // 中断的子优先级等级

#define TIMx_IRQHandler         TIM3_IRQHandler

extern void TimerInit(void);
extern u32 Test_GetCount(void);

#endif /* __TIMER_H */

