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

## 2. STM32F103串口框架

各类芯片的UART框图都是类似的，当设置好UART后，程序读写数据寄存器就可以接收、发送数据了。

![](lesson\stm32f103_uart\002_usart_block_diagram.png)

## 3. STM32F103串口操作

### 3.1 看原理图确定引脚

* 100ASM STM32F103的USART1接到一个USB串口芯片，然后就可以通过USB线连接电脑了

* 原理图如下

  ![](lesson\stm32f103_uart\001_uart_sch.png)



* 上图中的USART1_RX、USART1_TX，接到了PA9、PA10

  ![](lesson\stm32f103_uart\003_usart1_pin.png)

### 3.2 配置引脚为UART功能
#### 3.2.1 使能GPIOA/USART1模块

需要设置GPIOA的寄存器，选择引脚功能：所以要使能GPIOA模块。
GPIOA模块、USART1模块的使能都是在同一个寄存器里实现。
![](lesson\stm32f103_uart\005_gpioa_usart1_enable.png)

#### 3.2.2 配置引脚功能

从上图可以知道，PA9、PA10有三种功能：GPIO、USART1、TIMER1。

![](lesson\stm32f103_uart\004_gpio_function_sel.png)

### 3.3 设置串口参数

#### 3.3.1 设置波特率

波特率算公式：

![](lesson\stm32f103_uart\007_usart1_baudrate_calc.png)

USARTDIV由整数部分、小数部分组成，计算公式如下：
`USARTDIV = DIV_Mantissa + (DIV_Fraction / 16)  `
DIV_Mantissa和DIV_Fraction来自USART_BRR寄存器，如下图：
![](lesson\stm32f103_uart\008_usart_brr.png)

#### 3.3.2 设置数据格式

比如数据位设置为8，无校验位，停止位设置为1。
需要设置2个寄存器。

* USART1_CR1：用来设置数据位、校验位，使能USART
  ![](lesson\stm32f103_uart\009_usart_cr1.png)

* USART_CR2：用来设置停止位
  ![image-20201020083144137](C:\Users\weidongshan\Documents\WXWork\1688850409309448\WeDrive\百问科技\我的文件\备份\全系列视频\单片机_裸机\串口UART编程\stm32f103\doc_pic\lesson\stm32f103_uart\009_usart_cr2.png)

### 3.4 根据状态寄存器读写数据

* 状态寄存器

  ![](lesson\stm32f103_uart\010_usart_sr.png)

* 数据寄存器
  写、读这个寄存器，就可：发送、读取串口数据，如下图：

  ![](lesson\stm32f103_uart\011_usart_dr.png)

### 3.5 USART1的寄存器地址

* 基地址

![](lesson\stm32f103_uart\012_memory_map.png)

* USART寄存器
  用结构体来表示比较方便：

  ```c
  typedef unsigned int uint32_t;
  typedef struct
  {
    volatile uint32_t SR;    /*!< USART Status register, Address offset: 0x00 */
    volatile uint32_t DR;    /*!< USART Data register,   Address offset: 0x04 */
    volatile uint32_t BRR;   /*!< USART Baud rate register, Address offset: 0x08 */
    volatile uint32_t CR1;   /*!< USART Control register 1, Address offset: 0x0C */
    volatile uint32_t CR2;   /*!< USART Control register 2, Address offset: 0x10 */
    volatile uint32_t CR3;   /*!< USART Control register 3, Address offset: 0x14 */
    volatile uint32_t GTPR;  /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  } USART_TypeDef;
  
  USART_TypeDef *usart1 = (USART_TypeDef *)0x40013800;
  ```

  