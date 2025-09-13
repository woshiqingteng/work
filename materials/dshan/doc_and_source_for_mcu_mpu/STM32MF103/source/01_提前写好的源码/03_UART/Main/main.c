#include <stdio.h>
#include "main.h"
#include "stm32f103ze.h"
#include "driver_uart.h"

void SystemClock_Config(void);
uint8_t send_buf[128] = "Hello World!";

/********** 禁用半主机模式 **********/
#pragma import(__use_no_semihosting)
 
struct __FILE
{
	int a;
};
 
FILE __stdout;
FILE __stdin;  
 
void _sys_exit(int x)
{
	
}

/*****************************************************
*function:	写字符文件函数
*param1:	输出的字符
*param2:	文件指针
*return:	输出字符的ASCII码
******************************************************/
int fputc(int ch, FILE *f)
{
    UART_Transmit((uint8_t*)&ch, 1);
	return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    UART_Receive(&ch ,1);
//    if (ch == '\r')
//    {
//        UART_Transmit(&ch, 1);
//        ch = '\n';
//        UART_Transmit(&ch, 1);
//    }
    return (int)ch;
}

int main(void)
{
    char key = 0;
    SystemClock_Config();
    UART_Init(115200);
    
    printf("\r\nHello World!");
    printf("\r\nPress a key: ");
    
    while(1)
    {
        scanf("%c", &key);  // 按键后一定要再按空格才会正确输入
        printf("%c", key);  
    }
}

void SystemInit (void)
{
    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= (uint32_t)0xF8FF0000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    RCC->CFGR &= (uint32_t)0xFF80FFFF;

    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = 0x009F0000; 
}

void SystemClock_Config(void)
{
    // 打开HSE
    RCC->CR |= (1<<16);                     // 使能HSE
    while( (RCC->CR & (1<<17)) == 0 );      // 等待HSE就绪
    
    // 设置PLL
    RCC->CFGR |= (7<<18);                   // PLL9倍频
    RCC->CFGR |= (1<<16);                   // HSE作为PLL的时钟输入源
    
    // 打开PLL
    RCC->CR |= (1<<24);                     // 使能PLL
    while( (RCC->CR & (1<<25)) == 0 );      // 等待PLL就绪

    // 设置Flash时延
    FLASH->ACR |= (0x02<<0);                // 48MHz~72MHz的Flash时延是2个等待状态
    
    // 设置系统时钟
    RCC->CFGR |= (2<<0);                    // PLL作为系统时钟源
    while( (RCC->CFGR & (1<<3)) == 0 );     // 等待系统时钟就绪
    
    // APB1 最大时钟是36MHz，所以要设置2分频
    RCC->CFGR |= (0x04<<8);
}