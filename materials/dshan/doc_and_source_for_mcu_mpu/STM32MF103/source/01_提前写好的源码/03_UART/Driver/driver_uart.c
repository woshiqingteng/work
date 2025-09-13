#include "driver_uart.h"

static void UART_ClockEnable(void)
{
    // 寄存器RCC_APB2ENR[14]是USART1的时钟控制位
    RCC->APB2ENR |= (1<<14);
}

static void UART_ClockDisable(void)
{
    RCC->APB2ENR &= ~(1<<14);
}

static void UART_GpioInit(void)
{
    // 寄存器RCC_APB2ENR[2]是GPIOA的时钟控制位
    RCC->APB2ENR |= (1<<2);
    
    // 将PA9设置为复用输出模式，速率10M
    GPIOA->CRH &= ~(3<<4);      // 清零MODE
    GPIOA->CRH |= (1<<4);       // PA9->MODE = 0x01, Max speed = 10MHz
    GPIOA->CRH &= ~(3<<6);      // 清零CNF
    GPIOA->CRH |= (2<<6);       // PA9->CNF  = 0x02, Alternate function output Push-pull
    // 将PA10设置为输入模式
    GPIOA->CRH &= ~(3<<8);
    GPIOA->CRH |= (0<<8);       // PA10->MODE = 0x00, Input mode
    GPIOA->CRH &= ~(3<<10);
    GPIOA->CRH |= (2<<10);      // PA10->CNF = 0x02, Input with pull-up/pull-down
}

void UART_Init(uint32_t baudrate)
{
    float temp = 0.0;
    uint16_t mantissa = 0, fraction = 0;
    UART_ClockEnable();         // 使能USART1的时钟
    UART_GpioInit();            // 初始化PA9 PA10
    
    temp = (72000000.0/baudrate/16);
    mantissa = (uint16_t)temp;
    fraction = (temp - mantissa)*16;
    USART1->BRR |= (mantissa<<4) | (fraction);    // 设置波特率
    
    USART1->CR1 |= (1<<2);      // 使能接收
    USART1->CR1 |= (1<<3);      // 使能发送
    
    USART1->CR1 |= (0<<12);     // 起始位：0，数据位：8，停止位：n
    USART1->CR2 |= (0<<12);     // 停止位：1
    USART1->CR1 |= (0<<10);     // 不使能校验
    
    USART1->CR1 |= (1<<13);     // 使能USART;
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
        USART1->SR &= ~(1<<6);  // 清楚TC标志
        while(( (USART1->SR & (1<<7)) == 0)  ); // 状态寄存器SR[7]是TXE标志位，0表示没有将数据发送到移位寄存器，1表示已经将数据发送到移位寄存器；TXE是硬件置位，DR寄存器写入数据清零
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
        while(( (USART1->SR & (1<<5)) == 0)  ); // 状态寄存器SR[5]是RXNE标志位，0表示没有数据接受到，1表示数据等待读取；RXNE是硬件置位，读DR寄存器清零
        pdata[i] = USART1->DR;
    }
}