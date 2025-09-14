#include "string.h"
#include "exti.h"
#include "nvic.h"

/* stm32f103xe.h */

typedef struct
{
  volatile unsigned int EVCR;
  volatile unsigned int MAPR;
  volatile unsigned int EXTICR[4];
  volatile unsigned int RESERVED0;
  volatile unsigned int MAPR2;  
} AFIO_TypeDef;

void key_init(void)
{
	AFIO_TypeDef *afio = (AFIO_TypeDef *)0x40010000;
	
	unsigned int *pReg;
	unsigned int *pRegA;
	
	/* 1.��ʼ��GPIO����PA0, ����Ϊ�������� */
	/* ʹ��GPIOB, GPIOA */
	pReg = (unsigned int *)(0x40021000 + 0x18);
	*pReg |= (1<<3) | (1<<2);
	
	/* ����GPIOA0Ϊ�������� */
	pRegA = (unsigned int *)(0x40010800 + 0x00);
	*pRegA &= ~(3);     /* mode0 = 0b00 */
	*pRegA &= ~(3<<2);  /* cnf0 = 0b00 */
	*pRegA |= (1<<2);   /* cnf0 = 0b01 */
	
	/* 2. ����ΪEXTI0 */
	afio->EXTICR[0] &= ~0xf;  /* PA0 as EXTI0 */ 
}


void EXTI0_IRQHandler(void)
{
	/* GPIOA input data register */
	unsigned int * pRegA = (unsigned int *)(0x40010800 + 0x08);
	
		if ((*pRegA & (1<<0)) == 0) /* KEY1������ */
		{
			puts("KEY1 pressed!\n\r");
		}
		else
		{
			puts("KEY1 released!\n\r");
		}
		
		/* ����ж� */
		exti_clear_int(0);
		nvic_clear_int(6);
}

