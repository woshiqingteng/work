#include "gic.h"

#define IRQ_EXTI3  41

typedef struct
{
volatile unsigned int RTSR1;               /*!< EXTI Rising trigger selection register,          Address offset: 0x00 */
volatile unsigned int FTSR1;               /*!< EXTI Falling trigger selection register,         Address offset: 0x04 */
volatile unsigned int SWIER1;              /*!< EXTI Software interrupt event register,          Address offset: 0x08 */
volatile unsigned int RPR1;                /*!< EXTI rising edge pending register,               Address offset: 0x0C */
volatile unsigned int FPR1;                /*!< EXTI falling edge pending register,              Address offset: 0x10 */
volatile unsigned int TZENR1;              /*!< EXTI TrustZone enable register,                  Address offset: 0x14 */
volatile unsigned int RESERVED1[2];        /*!< Reserved,                                        0x18 to 0x1C */
volatile unsigned int RTSR2;               /*!< EXTI Rising trigger selection register,          Address offset: 0x20 */
volatile unsigned int FTSR2;               /*!< EXTI Falling trigger selection register,         Address offset: 0x24 */
volatile unsigned int SWIER2;              /*!< EXTI Software interrupt event register,          Address offset: 0x28 */
volatile unsigned int RPR2;                /*!< EXTI rising edge pending register,               Address offset: 0x2C */
volatile unsigned int FPR2;                /*!< EXTI falling edge pending register ,             Address offset: 0x30 */
volatile unsigned int TZENR2;              /*!< EXTI TrustZone enable register,                  Address offset: 0x34 */
volatile unsigned int RESERVED2[2];        /*!< Reserved,                                        0x38 to 0x3C         */
volatile unsigned int RTSR3;               /*!< EXTI Rising trigger selection register,          Address offset: 0x40 */
volatile unsigned int FTSR3;               /*!< EXTI Falling trigger selection register,         Address offset: 0x44 */
volatile unsigned int SWIER3;              /*!< EXTI Software interrupt event register,          Address offset: 0x48 */
volatile unsigned int RPR3;                /*!< EXTI rising edge pending register,               Address offset: 0x4C */
volatile unsigned int FPR3;                /*!< EXTI Falling trigger pending register,           Address offset: 0x50 */
volatile unsigned int TZENR3;              /*!< EXTI TrustZone enable register,                  Address offset: 0x54 */
volatile unsigned int RESERVED3[2];        /*!< Reserved,                                        0x58 to 0x5C         */
volatile unsigned int EXTICR[4];             /*!< EXTI external interrupt selection register 1,    Address offset: 0x60-0x6C */
volatile unsigned int RESERVED4[4];        /*!< Reserved,                                        0x70 to 0x7C         */
volatile unsigned int C1IMR1;              /*!< EXTI CPU wakeup with interrupt mask register,    Address offset: 0x80 */
volatile unsigned int C1EMR1;              /*!< EXTI CPU wakeup with event mask register,        Address offset: 0x84 */
volatile unsigned int RESERVED5[2];        /*!< Reserved,                                        0x88 to 0x8C         */
volatile unsigned int C1IMR2;              /*!< EXTI CPU wakeup with interrupt mask register,    Address offset: 0x90 */
volatile unsigned int C1EMR2;              /*!< EXTI CPU wakeup with event mask register,        Address offset: 0x94 */
volatile unsigned int RESERVED6[2];        /*!< Reserved,                                        0x98 to 0x9C         */
volatile unsigned int C1IMR3;              /*!< EXTI Interrupt mask register,                    Address offset: 0xA0 */
volatile unsigned int C1EMR3;              /*!< EXTI CPU wakeup with event mask register,        Address offset: 0xA4 */
volatile unsigned int RESERVED7[6];        /*!< Reserved,                                        0xA8 to 0xBC         */
volatile unsigned int C2IMR1;              /*!< EXTI CPU2 wakeup with interrupt mask register,   Address offset: 0xC0 */
volatile unsigned int C2EMR1;              /*!< EXTI Interrupt mask register,                    Address offset: 0xC4 */
volatile unsigned int RESERVED8[2];        /*!< Reserved,                                        0xC8 to 0xCC         */
volatile unsigned int C2IMR2;              /*!< EXTI CPU2 wakeup with interrupt mask register,   Address offset: 0xD0 */
volatile unsigned int C2EMR2;              /*!< EXTI CPU2 wakeup with event mask register,       Address offset: 0xD4 */
volatile unsigned int RESERVED9[2];        /*!< Reserved,                                        0xD8 to 0xDC         */
volatile unsigned int C2IMR3;              /*!< EXTI CPU2 wakeup with interrupt mask register ,  Address offset: 0xE0 */
volatile unsigned int C2EMR3;              /*!< EXTI CPU wakeup with event mask register ,       Address offset: 0xE4 */
}EXTI_TypeDef;




void exti_init(void)
{
	EXTI_TypeDef *exti = (EXTI_TypeDef *)0x5000D000;

	/* 1. 设置EXTI3的中断源位PG3 */
	exti->EXTICR[0] &= ~(0xff<<24);
	exti->EXTICR[0] |= (0x06<<24);

	/* 2. 设置中断触发方式:双边沿触发 */
	exti->RTSR1 |= (1<<3);
	exti->FTSR1 |= (1<<3);

	/* 3. 使能中断 */
	exti->C1IMR1 |= (1<<3);	
	gic_enable_irq(IRQ_EXTI3);
}

void do_irq_c(void)
{
	int irq;
	EXTI_TypeDef *exti = (EXTI_TypeDef *)0x5000D000;
	volatile unsigned int *pRegKey;
	
	/* 1. 分辨中断 */
	irq = get_gic_irq();

	/* 2. 调用处理函数 */
	if (irq == IRQ_EXTI3)
	{
		/*
		 * GPIOG_IDR地址：0x50008000 + 0x10
		 */
		pRegKey = (volatile unsigned int *)(0x50008000 + 0x10);
		if (*pRegKey & (1<<3))
		{
			puts("KEY1 released!\n\r");
		}
		else
		{
			puts("KEY1 pressed!\n\r");
		}
		exti->RPR1 |= (1<<3);
		exti->FPR1 |= (1<<3);
	}

	/* 3. 清除中断 */
	clear_gic_irq(irq);
}



