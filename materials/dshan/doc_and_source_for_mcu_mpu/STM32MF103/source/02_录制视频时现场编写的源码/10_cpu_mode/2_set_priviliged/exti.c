
typedef struct
{
  volatile unsigned int IMR;
  volatile unsigned int EMR;
  volatile unsigned int RTSR;
  volatile unsigned int FTSR;
  volatile unsigned int SWIER;
  volatile unsigned int PR;
} EXTI_TypeDef;


void exti_init(void)
{
	EXTI_TypeDef * exti = (EXTI_TypeDef *)0x40010400;
	
	/* 1. ���ô�����ʽ��˫���ش��� */
	exti->RTSR |= (1<<0);
	exti->FTSR |= (1<<0);
	
	/* 2. ʹ���ж�: �ܹ���NVIC�����ж� */
	exti->IMR |= (1<<0);
}

void exti_clear_int(int bit)
{
	EXTI_TypeDef * exti = (EXTI_TypeDef *)0x40010400;
	exti->PR |= (1<<bit);
}
