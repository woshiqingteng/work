#include "timer.h"
#include "stm32f10x_tim.h"

NVIC_InitTypeDef            gTIMx_NVIC_Init;        // 定义一个中断对象结构体
TIM_TimeBaseInitTypeDef     gTIMX_TimeBase_Init;    // 定义一个定时器对象结构体

static vu8 test_cnt = 0;

static unsigned int  g_timer_cnt;

void TimerInit(void)
{    
    /* 1. 使能时钟 */
    TIMx_Clk_Enable();
    
    /* 2. 配置定时器对象结构体的成员，设置定时器属性 */
    gTIMX_TimeBase_Init.TIM_Period          = TIMx_PRE_LOAD_VALUE;      // 设置预装载值，定时器会向上从0计数到value或者向下从value计数到0
	gTIMX_TimeBase_Init.TIM_Prescaler       = TIMx_PRESCALER;           // 设置预分频系数
	gTIMX_TimeBase_Init.TIM_ClockDivision   = TIMx_DIV;                 // 设置分频系数     
	gTIMX_TimeBase_Init.TIM_CounterMode     = TIM_CounterMode_Up;       // 设置计数方式：Up/Down
    
    /* 3. 调用库函数初始化定时器 */
	TIM_TimeBaseInit( TIMx, &gTIMX_TimeBase_Init );

    /* 4. 配置中断的属性 */
    gTIMx_NVIC_Init.NVIC_IRQChannel                     = TIMx_IRQ;                 // 选择中断
	gTIMx_NVIC_Init.NVIC_IRQChannelSubPriority          = TIMx_NVIC_SUB_PRIORITY;   // 设置中断的子优先级
	gTIMx_NVIC_Init.NVIC_IRQChannelPreemptionPriority   = TIMx_NVIC_PRE_PRIORITY;   // 设置中断的抢占优先级
	gTIMx_NVIC_Init.NVIC_IRQChannelCmd                  = ENABLE;                   // 使能中断
    
    /* 5. 初始化中断 */
	NVIC_Init( &gTIMx_NVIC_Init );
        
    /* 6. 选择定时器的中断触发方式 */
	TIM_ITConfig( TIMx, TIM_IT_Update, ENABLE );    // 选择为更新触发中断，即向上计数到预装载值或者向下技术到0触发中断
    
    /* 7. 使能定时器 */
    TIM_Cmd( TIMx, ENABLE );
}

u32 Test_GetCount(void)
{
    return test_cnt;
}


u32 TimerGetCount(void)
{
    return g_timer_cnt;
}


void TIMx_IRQHandler(void)
{
    if( TIM_GetITStatus(TIMx, TIM_IT_Update) == SET )   // 判断是否是更新触发中断
    {
        test_cnt = !test_cnt;
		g_timer_cnt++;
        TIM_ClearITPendingBit( TIMx, TIM_IT_Update );   // 清除中断
    }
}
