#include "uart.h"

/*根据STM32MP157芯片手册<<53.7 Universal synchronous/asynchronous receiver transmitter (USART/UART)>>的2655页，定义USART4的结构体,*/
typedef struct {
  volatile unsigned int  USART_CR1;          /**< USART control register 1,                 offset: 0x00 	 串口控制寄存器1，            偏移地址0x00  */
  volatile unsigned int  USART_CR2;          /**< USART control register 2,                 offset: 0x04 	 串口控制寄存器2，            偏移地址0x04  */
  volatile unsigned int  USART_CR3;          /**< USART control register 3,                 offset: 0x08 	 串口控制寄存器3，            偏移地址0x08  */
  volatile unsigned int  USART_BRR;          /**< USART Baud Rate register,                 offset: 0x0C     串口波特率寄存器             偏移地址0x0C  */
  volatile unsigned int  USART_GTPR;         /**< USART guard time and prescaler register,  offset: 0x10     串口保护时间和预分频器寄存器 偏移地址0x10  */
  volatile unsigned int  USART_RTOR;         /**< USART receiver timeout register,          offset: 0x14     串口接收超时寄存器           偏移地址0x14  */
  volatile unsigned int  USART_RQR;          /**< USART request register,                   offset: 0x18     串口请求寄存器               偏移地址0x18  */
  volatile unsigned int  USART_ISR;          /**< USART interrupt and status register,      offset: 0x1C     串口中断与状态寄存器         偏移地址0x1C  */
  volatile unsigned int  USART_ICR;          /**< USART interrupt flag clear register ,     offset: 0x20     串口中断状态清除寄存器       偏移地址0x20  */
  volatile unsigned int  USART_RDR;          /**< USART receive data register,              offset: 0x24     串口接收数据寄存器           偏移地址0x24  */
  volatile unsigned int  USART_TDR;          /**< USART transmit data register,             offset: 0x28     串口发送数据寄存器           偏移地址0x28  */
  volatile unsigned int  USART_PRESC;        /**< USART prescaler register,                 offset: 0x2C     串口预分频器寄存器           偏移地址0x2C  */
} UART_Type;


static volatile unsigned int *RCC_PLL3FRACR ;
static volatile unsigned int *RCC_RCK3SELR  ;
static volatile unsigned int *RCC_PLL3CFGR1 ;
static volatile unsigned int *RCC_PLL3CFGR2 ;
static volatile unsigned int *RCC_MSSCKSELR ;
static volatile unsigned int *RCC_APB1DIVR  ;
static volatile unsigned int *RCC_PLL3CR  	;
static volatile unsigned int *RCC_UART24CKSELR;

/**********************************************************************
 * 函数名称： uart4_clk_init
 * 功能描述： 初始化UART4的时钟源为104.5MHz
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2020/09/13	     V1.0	  zh(angenao)	      创建
 ***********************************************************************/
void uart4_clk_init(void)	 				
{
	RCC_PLL3CR       = (volatile unsigned int *)(0x50000880);
	RCC_RCK3SELR     = (volatile unsigned int *)(0x50000820);
	RCC_PLL3FRACR    = (volatile unsigned int *)(0x5000088C);
	RCC_PLL3CFGR1    = (volatile unsigned int *)(0x50000884);
	RCC_PLL3CFGR2    = (volatile unsigned int *)(0x50000888);
	RCC_MSSCKSELR    = (volatile unsigned int *)(0x50000048);
	RCC_APB1DIVR     = (volatile unsigned int *)(0x50000834);
	RCC_UART24CKSELR = (volatile unsigned int *)(0x500008E8);
	
	/*CLK初始化*/
	*RCC_PLL3CR &= ~(1<<0);		/*PLL3禁止*/
	
	*RCC_RCK3SELR &= ~(0x3<<0); /*晶振作为时钟源，HSE*/ 
	*RCC_RCK3SELR |= 0x1;       /*晶振作为时钟源，HSE*/ 
	
	*RCC_UART24CKSELR = 0;      /*pclk1为UART4的时钟*/
	
	*RCC_PLL3CFGR1 &= ~(0xC7<<0);/*清空DIVN3，DIVM3采用默认值0x1*/
	*RCC_PLL3CFGR1 |= (0x33<<0); /*DIVN3 = 51  , DIVM3 = 1 */
	
	*RCC_PLL3CFGR2 &= ~(0x7F<<0);  /*清空DIVP*/
	*RCC_PLL3CFGR2 |= (0x2<<0);    /*DIVP = 2 */

	*RCC_PLL3FRACR &= ~(1<<16);	/*latch disable*/
	*RCC_PLL3FRACR  = (0x7FF<<3); 		/*2048*/
	*RCC_PLL3FRACR |= (1<<16);	/*latch enable*/

	*RCC_MSSCKSELR &= ~(0x3<<0);
	*RCC_MSSCKSELR |= (0x3<<0); /*选择pll3_p_ck作为子系统时钟 209MHz*/
	
	*RCC_APB1DIVR &= ~(0x7<<0);
	*RCC_APB1DIVR |=  (0x1<<0);/*2分频，p1clk = 104.5MHz，即串口的时钟*/
	
	*RCC_PLL3CR |= 1<<0;		/*PLL3使能*/
	while(!(*RCC_PLL3CR & 0x2));/*等待PLL3起来*/
	*RCC_PLL3CR |= 1<<4;        /*pll3_p_ck使能*/
}



void uart_init(void)
{
	volatile unsigned int *GPIOA_AFRH    ;
	volatile unsigned int *GPIOA_MODER	;
	UART_Type *uart4 = (UART_Type *)0x40010000;

	GPIOA_MODER 	 = (volatile unsigned int *)(0x50002000);
	GPIOA_AFRH	     = (volatile unsigned int *)(0x50002024);
	
	/* 设置UART的时钟 */
	uart4_clk_init();

	/* 配置引脚功能
	 * PA11, PA12 as alternate function
	 */
	*GPIOA_MODER &= ~((3<<22) | (3<<24));
	*GPIOA_MODER |= ((2<<22) | (2<<24));

	/* AF6 : UART4_TX, UART4_RX */
	*GPIOA_AFRH &= ~((0xf<<12) | (0xf<<16));
	*GPIOA_AFRH |= ((6<<12) | (6<<16));

	/* 先关闭串口 */
	uart4->USART_CR1 &= ~(1<<0);


	/* 设置波特率 
	 * 115200 = 104.5*1000,000/USARTDIV
	 * USARTDIV = 104.5*1000,000/115200 = 907
	 */
	uart4->USART_BRR = 907;

	/* 设置数据格式
	 * 8N1
	 */
	uart4->USART_CR1 |= (1<<3) | (1<<2);
	uart4->USART_CR2 &= ~(3<<12);
	
	/* 使能UART模块 */
	uart4->USART_CR1 |= (1<<0);
}

int getchar(void)
{
	UART_Type *uart4 = (UART_Type *)0x40010000;
	while ((uart4->USART_ISR & (1<<5)) == 0);
	return uart4->USART_RDR;
}

int	putchar(char c)
{
	UART_Type *uart4 = (UART_Type *)0x40010000;
	while ((uart4->USART_ISR & (1<<7)) == 0);
	uart4->USART_TDR = c;
	return c;
}

