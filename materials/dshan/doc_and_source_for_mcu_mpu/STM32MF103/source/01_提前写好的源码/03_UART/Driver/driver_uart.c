#include "driver_uart.h"

static void UART_ClockEnable(void)
{
    // �Ĵ���RCC_APB2ENR[14]��USART1��ʱ�ӿ���λ
    RCC->APB2ENR |= (1<<14);
}

static void UART_ClockDisable(void)
{
    RCC->APB2ENR &= ~(1<<14);
}

static void UART_GpioInit(void)
{
    // �Ĵ���RCC_APB2ENR[2]��GPIOA��ʱ�ӿ���λ
    RCC->APB2ENR |= (1<<2);
    
    // ��PA9����Ϊ�������ģʽ������10M
    GPIOA->CRH &= ~(3<<4);      // ����MODE
    GPIOA->CRH |= (1<<4);       // PA9->MODE = 0x01, Max speed = 10MHz
    GPIOA->CRH &= ~(3<<6);      // ����CNF
    GPIOA->CRH |= (2<<6);       // PA9->CNF  = 0x02, Alternate function output Push-pull
    // ��PA10����Ϊ����ģʽ
    GPIOA->CRH &= ~(3<<8);
    GPIOA->CRH |= (0<<8);       // PA10->MODE = 0x00, Input mode
    GPIOA->CRH &= ~(3<<10);
    GPIOA->CRH |= (2<<10);      // PA10->CNF = 0x02, Input with pull-up/pull-down
}

void UART_Init(uint32_t baudrate)
{
    float temp = 0.0;
    uint16_t mantissa = 0, fraction = 0;
    UART_ClockEnable();         // ʹ��USART1��ʱ��
    UART_GpioInit();            // ��ʼ��PA9 PA10
    
    temp = (72000000.0/baudrate/16);
    mantissa = (uint16_t)temp;
    fraction = (temp - mantissa)*16;
    USART1->BRR |= (mantissa<<4) | (fraction);    // ���ò�����
    
    USART1->CR1 |= (1<<2);      // ʹ�ܽ���
    USART1->CR1 |= (1<<3);      // ʹ�ܷ���
    
    USART1->CR1 |= (0<<12);     // ��ʼλ��0������λ��8��ֹͣλ��n
    USART1->CR2 |= (0<<12);     // ֹͣλ��1
    USART1->CR1 |= (0<<10);     // ��ʹ��У��
    
    USART1->CR1 |= (1<<13);     // ʹ��USART;
}

void UART_Transmit(uint8_t *pdata, uint16_t len)
{
    uint16_t i = 0;
    if(len==0)
    {
        return;
    }
    for(i=0; i<len; i++)
    {
        USART1->SR &= ~(1<<6);  // ���TC��־
        while(( (USART1->SR & (1<<7)) == 0)  ); // ״̬�Ĵ���SR[7]��TXE��־λ��0��ʾû�н����ݷ��͵���λ�Ĵ�����1��ʾ�Ѿ������ݷ��͵���λ�Ĵ�����TXE��Ӳ����λ��DR�Ĵ���д����������
        USART1->DR = pdata[i];
        while( (USART1->SR & (1<<6)) == 0 );
    }
}

void UART_Receive(uint8_t *pdata, uint16_t len)
{
    uint16_t i = 0; 
    if(len==0)
    {
        return;
    }
    
    for(i=0; i<len; i++)
    {
        while(( (USART1->SR & (1<<5)) == 0)  ); // ״̬�Ĵ���SR[5]��RXNE��־λ��0��ʾû�����ݽ��ܵ���1��ʾ���ݵȴ���ȡ��RXNE��Ӳ����λ����DR�Ĵ�������
        pdata[i] = USART1->DR;
    }
}