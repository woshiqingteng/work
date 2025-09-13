# UART编程 #
## 1. 串口编程步骤

### 1.1 看原理图确定引脚

* 有很多串口，使用哪一个？看原理图确定

### 1.2 配置引脚为UART功能
* 至少用到发送、接收引脚：txd、rxd
* 需要把这些引脚配置为UART功能，并使能UART模块

### 1.3 设置串口参数

* 有哪些参数？
  * 波特率
  * 数据位
  * 校验位
  * 停止位
* 示例：
比如`15200,8n1`表示`波特率为115200,8个数据为，没有校验位，1个停止位`

### 1.4 根据状态寄存器读写数据

* 肯定有一个数据寄存器，程序把数据写入，即刻通过串口向外发送数据
* 肯定有一个数据寄存器，程序读取这个寄存器，就可以获得先前接收到的数据
* 很多有状态寄存器
  * 判断数据是否发送出去？是否发送成功？
  * 判断是否接收到了数据？

## 2. STM32MP157串口框架

参考手册`DM00327659.pdf中《53 Universal synchronous/asynchronous receiver transmitter (USART/UART)》`
各类芯片的UART框图都是类似的，当设置好UART后，程序读写数据寄存器就可以接收、发送数据了。

![image-20201020181658592](lesson\stm32mp157_uart\001_stm32mp157_uart_block_diagram.png)

## 3. STM32MP157串口操作

### 3.1 看原理图确定引脚

* 100ASM STM32MP157的UART接到一个USB串口芯片，然后就可以通过USB线连接电脑了

* 原理图如下

  ![](lesson\stm32mp157_uart\002_stm32mp157_uart_sch.png)

* 它是哪个UART呢？UART4
  参考`DM00489389.pdf《4 Pinouts, pin description and alternate functions》`
  ![](lesson\stm32mp157_uart\003_uart4_pins.png)

* 从上图也可以看出来，要把PA11、PA12设置为AF6才能用作UART4的引脚

### 3.2 使能UART 及配置引脚

#### 3.2.1 设置UART的时钟

参考STM32MP157开发参考手册`DM00327659.pdf《10 Reset and clock control (RCC)》`，
整理出UART4的时钟树，沿着这条线去设置一系列的寄存器，就可以设置UART4的时钟源。
这不是本节课程的重点，所以不细讲时钟的设置。
如下图：

![](lesson\stm32mp157_uart\004_uart4_clk_path.png)

使用以下函数就可给UART4设置时钟源为104.5MHz：

```c
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
```

#### 3.2.2 配置引脚功能

* 配置PA11、PA12为Alternate function模式
  ![](lesson\stm32mp157_uart\005_uart4_pin_mode.png)
  
* 配置PA11、PA12为AF6
  ![](lesson\stm32mp157_uart\006_uart4_pin_af6.png)
  


### 3.3 设置串口参数

#### 3.3.1 设置波特率

波特率算公式：

![](lesson\stm32mp157_uart\007_baudrate_calc.png)

上述公式中，usart_ker_ckpres 在本节教程中为104.5MHZ，USARTDIV在寄存器中设置，如下图所示：
![](lesson\stm32mp157_uart\008_usart_brr.png)

#### 3.3.2 STM32MP157的特殊地方

STM32MP157的UART寄存器中，在FIFO使能、FIFO不使能的情况下，同一个寄存器的位含义是不一样的。
为简化程序，本节教程不使用FIFO。
在芯片手册中，要观看标有“FIFO mode disabled”的寄存器介绍。

#### 3.3.3 设置数据格式

比如数据位设置为8，无校验位，停止位设置为1。
这需要设置USART_CR1、USART_CR2，这2个寄存器的很多位，必须在UART禁止状态(UE=0)下设置。

* USART_CR1：禁止FIFO、设置数据位等，还可以使能UART

![](lesson\stm32mp157_uart\009_usart_cr1.png)

* USART_CR2：设置停止位

  ![](lesson\stm32mp157_uart\010_usart_cr2.png)

#### 3.3.4 使能UART模块

虽然给UART提供了时钟，但是UART本身并未使能，需要设置一下USART_CR1寄存器。

### 3.4 根据状态寄存器读写数据

#### 3.4.1 状态寄存器

  ![](lesson\stm32mp157_uart\011_usart_isr.png)

#### 3.4.2 接收数据寄存器

读这个寄存器，就可读取串口数据，如下图：

![](lesson\stm32mp157_uart\012_usart_rdr.png)

#### 3.4.3 发送数据寄存器

把数据写入这个寄存器，即可发送出去：
![](lesson\stm32mp157_uart\013_usart_tdr.png)

### 3.5 寄存器地址

* 配置引脚的寄存器地址
  用到的寄存器，从C代码定义如下：

  ```c
  static volatile unsigned int *GPIOA_AFRH    ;
  static volatile unsigned int *GPIOA_MODER	;
  
  GPIOA_MODER 	 = (volatile unsigned int *)(0x50002000);
  GPIOA_AFRH	     = (volatile unsigned int *)(0x50002024);
  ```
  
  
  
* UART4寄存器
  

  UART4基地址：0x40010000，里面的寄存器用结构体来表示比较方便：
  
  ```c
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
  
  UART_Type *uart4 = (UART_Type *)0x40010000;
  ```
  
  