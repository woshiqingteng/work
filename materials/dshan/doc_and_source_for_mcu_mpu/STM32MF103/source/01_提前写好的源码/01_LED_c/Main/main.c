
#define RCC_APB2ENR  (0x40021000 + 0x18)
#define GPIOB_BASE   (0x40010C00)
#define GPIOB_CRL    (GPIOB_BASE + 0)
#define GPIOB_ODR    (GPIOB_BASE + 0X0c)

void delay(volatile int i)
{
	while (i--);
}

int main(void)
{
	volatile unsigned int *pRccApb2Enr;
	volatile unsigned int *pGpiobCrl;
	volatile unsigned int *pGpiobOdr;
	
	pRccApb2Enr = (volatile unsigned int *)RCC_APB2ENR;
	pGpiobCrl   = (volatile unsigned int *)GPIOB_CRL;
	pGpiobOdr   = (volatile unsigned int *)GPIOB_ODR;
	
	/* 1. ʹ��GPIOB��ʱ��
	 * GPIO�ǹ�����APB2�ϵģ���������Ҫʹ��APB2������ʱ��ʹ��λ
	 * оƬ�ֲ�113ҳ RCC_APB2ENR�Ĵ������������ǿ��Կ���APB2ENR[8:2]��GPIO[G:A]ʱ�ӵĿ���λ������GPIOʱ����ʧ�ܻ���ʹ��
	 * �������ϵ�LED��GPIO����ʹ�õ�GPIOB����Ҫʹ��APB2ENR[3]λ��Ϊ�丳ֵ��1�����ɴ�GPIOB��ʱ�� */
	*pRccApb2Enr |= (1<<3);

	/* 2. ����GPIOΪ�������:
	 * MODE_0=1,PB0���ģʽ������������10MHz 
	 */
  *pGpiobCrl |= (1<<0);
	
	while (1)
	{
    *pGpiobOdr |= (1<<0);
		delay(100000);
    *pGpiobOdr &= ~(1<<0);
		delay(100000);
	}
	return 0;
}

