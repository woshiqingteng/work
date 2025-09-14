#include <stdio.h>
#include "main.h"
#include "stm32f103ze.h"
#include "driver_uart.h"

void SystemClock_Config(void);
uint8_t send_buf[128] = "Hello World!";

/********** ���ð�����ģʽ **********/
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
*function:	д�ַ��ļ�����
*param1:	������ַ�
*param2:	�ļ�ָ��
*return:	����ַ���ASCII��
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
        scanf("%c", &key);  // ������һ��Ҫ�ٰ��ո�Ż���ȷ����
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
    // ��HSE
    RCC->CR |= (1<<16);                     // ʹ��HSE
    while( (RCC->CR & (1<<17)) == 0 );      // �ȴ�HSE����
    
    // ����PLL
    RCC->CFGR |= (7<<18);                   // PLL9��Ƶ
    RCC->CFGR |= (1<<16);                   // HSE��ΪPLL��ʱ������Դ
    
    // ��PLL
    RCC->CR |= (1<<24);                     // ʹ��PLL
    while( (RCC->CR & (1<<25)) == 0 );      // �ȴ�PLL����

    // ����Flashʱ��
    FLASH->ACR |= (0x02<<0);                // 48MHz~72MHz��Flashʱ����2���ȴ�״̬
    
    // ����ϵͳʱ��
    RCC->CFGR |= (2<<0);                    // PLL��Ϊϵͳʱ��Դ
    while( (RCC->CFGR & (1<<3)) == 0 );     // �ȴ�ϵͳʱ�Ӿ���
    
    // APB1 ���ʱ����36MHz������Ҫ����2��Ƶ
    RCC->CFGR |= (0x04<<8);
}