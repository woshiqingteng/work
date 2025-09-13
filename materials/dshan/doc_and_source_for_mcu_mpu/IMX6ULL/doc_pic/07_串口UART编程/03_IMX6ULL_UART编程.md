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

## 2. IMX6ULL串口框架

参考手册`IMX6ULLRM.pdf中《CChapter 55: Universal Asynchronous Receiver/Transmitter (UART)》`
各类芯片的UART框图都是类似的，当设置好UART后，程序读写数据寄存器就可以接收、发送数据了。

![](lesson\imx6ull_uart\001_imx6ull_uart_block_diagram.png)

## 3. IMX6ULL串口操作

### 3.1 看原理图确定引脚

* 100ASM IMX6ULL的UART1接到一个USB串口芯片，然后就可以通过USB线连接电脑了

* 原理图如下

  ![](lesson\imx6ull_uart\002_imx6ull_uart1_sch.png)



* 上图中的USART1_RX、USART1_TX，接到了PA9、PA10

  ![](lesson\imx6ull_uart\003_uart1_pin.png)

### 3.2 使能UART
#### 3.2.1 设置UART的总时钟

参考IMX6ULL芯片手册《Chapter 18: Clock Controller Module (CCM)》，
根据IMX6ULL的时钟树，设置CSCDR1寄存器就可以给UART提供总时钟，如下图：

![](lesson\imx6ull_uart\004_uart_clk.png)

下图是CSCDR1(CCM Serial Clock Divider Register 1)的位说明，对于UART，我们选择时钟源为80M：

![](lesson\imx6ull_uart\005_uart_clk_sel.png)

#### 3.2.2 给UART模块提供时钟

![](lesson\imx6ull_uart\006_uart1_enable.png)

#### 3.2.3 使能UART模块

虽然给UART提供了时钟，但是UART本身并未使能，需要设置以下寄存器：
![](lesson\imx6ull_uart\014_uart_enable.png)

#### 3.2.4 配置引脚功能

* 配置UART1_TX引脚
  在芯片手册中搜索UART1_TX，可以找到下图所示寄存器：
  ![](lesson\imx6ull_uart\007_uart1_tx_pin_sel.png)
* 配置UART1_RX引脚
  在芯片手册中搜索UART1_RX，可以找到下图所示寄存器：
  ![](lesson\imx6ull_uart\008_uart1_rx_pin_sel.png)

#### 3.2.5 IMX6ULL特殊的地方

* Daisy Chain select
  IMX6ULL还有一个“Daisy Chain select”功能，比如下图中：
  A、B、C三个引脚都可以连接到Module X，它们都可以驱动Module X。
  使用哪一个引脚呢？还需要设置“Daisy Chain select”，用来选择A、B、C之一。
  ![](lesson\imx6ull_uart\009_daisy_chain_select.png)

Daisy Chain Select有什么用处呢？
比如UART，它只有TXD、RXD两个引脚：RXD有外部电路输入。
能否让UART的TXD直接给RXD提供数据？可以！这就是回环，方便调试。
怎么做？
可以在外部电路把TXD接到RXD，也可以在芯片内部让RXD可以选择数据来源，如下图所示：
![](lesson\imx6ull_uart\010_uart1_daisy_chain_select.png)

* UART1_RX的输入选择
  ![](lesson\imx6ull_uart\011_uart1_daisy_chain_select_register.png)

### 3.3 设置串口参数

#### 3.3.1 设置波特率

波特率算公式：

![](lesson\imx6ull_uart\012_baudrate_cal.png)

上述公式中，Ref Freq为80MHZ，UBMR和UBIR在寄存器中设置。

* 要先设置UBIR，再设置BUMR
* UBIR、BUMR中的值，是实际值减1

![](lesson\imx6ull_uart\013_baudrate_register.png)


#### 3.3.2 设置数据格式

比如数据位设置为8，无校验位，停止位设置为1。
![](lesson\imx6ull_uart\015_uart_ucr2.png)

#### 3.3.3 IMX6ULL芯片要求必须设置

对于UART1_UCR3的bit2，必须设置为1，芯片要求的，没什么道理可讲：

![](lesson\imx6ull_uart\016_uart_ucr3.png)

### 3.4 根据状态寄存器读写数据

#### 3.4.1 状态寄存器

  ![](lesson\imx6ull_uart\017_uart_usr2.png)

#### 3.4.2 接收数据寄存器

读这个寄存器，就可读取串口数据，如下图：

![](lesson\imx6ull_uart\018_uart_urxd.png)

#### 3.4.3 发送数据寄存器

![](lesson\imx6ull_uart\019_uart_utxd.png)

### 3.5 寄存器地址

* 其他寄存器地址
  用到的寄存器，从C代码定义如下：

  ```c
  volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA ;
  volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA	;
  volatile unsigned int *IOMUXC_UART1_RX_DATA_SELECT_INPUT ;
  volatile unsigned int *CCM_CSCDR1;
  volatile unsigned int *CCM_CCGR5;
  
  IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA		= (volatile unsigned int *)(0x20E0084);
  IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA		= (volatile unsigned int *)(0x20E0088);
  IOMUXC_UART1_RX_DATA_SELECT_INPUT		= (volatile unsigned int *)(0x20E0624);
  CCM_CSCDR1 = (volatile unsigned int *)(0x020C4024);
  CCM_CCGR5 = (volatile unsigned int *)(0x020C407C);
  ```

  

* UART1寄存器
  

  UART1基地址：0x02020000，里面的寄存器用结构体来表示比较方便：
  
  ```c
  /*根据IMX6ULL芯片手册<<55.15 UART Memory Map/Register Definition>>的3608页，定义UART的结构体,*/
  typedef struct {
    volatile unsigned int  URXD;               /**< UART Receiver Register, offset: 0x0 	           串口接收寄存器，偏移地址0x0     */
    		   unsigned char RESERVED_0[60];		
    volatile unsigned int  UTXD;               /**< UART Transmitter Register, offset: 0x40          串口发送寄存器，偏移地址0x40*/
    		   unsigned char RESERVED_1[60];		
    volatile unsigned int  UCR1;               /**< UART Control Register 1, offset: 0x80 	       串口控制寄存器1，偏移地址0x80*/
    volatile unsigned int  UCR2;               /**< UART Control Register 2, offset: 0x84 	       串口控制寄存器2，偏移地址0x84*/
    volatile unsigned int  UCR3;               /**< UART Control Register 3, offset: 0x88            串口控制寄存器3，偏移地址0x88*/
    volatile unsigned int  UCR4;               /**< UART Control Register 4, offset: 0x8C            串口控制寄存器4，偏移地址0x8C*/
    volatile unsigned int  UFCR;               /**< UART FIFO Control Register, offset: 0x90         串口FIFO控制寄存器，偏移地址0x90*/
    volatile unsigned int  USR1;               /**< UART Status Register 1, offset: 0x94             串口状态寄存器1，偏移地址0x94*/
  volatile unsigned int  USR2;               /**< UART Status Register 2, offset: 0x98             串口状态寄存器2，偏移地址0x98*/
    volatile unsigned int  UESC;               /**< UART Escape Character Register, offset: 0x9C     串口转义字符寄存器，偏移地址0x9C*/
    volatile unsigned int  UTIM;               /**< UART Escape Timer Register, offset: 0xA0         串口转义定时器寄存器 偏移地址0xA0*/
    volatile unsigned int  UBIR;               /**< UART BRM Incremental Register, offset: 0xA4      串口二进制倍率增加寄存器 偏移地址0xA4*/
    volatile unsigned int  UBMR;               /**< UART BRM Modulator Register, offset: 0xA8 	   串口二进制倍率调节寄存器 偏移地址0xA8*/
    volatile unsigned int  UBRC;               /**< UART Baud Rate Count Register, offset: 0xAC      串口波特率计数寄存器 偏移地址0xAC*/
    volatile unsigned int  ONEMS;              /**< UART One Millisecond Register, offset: 0xB0      串口一毫秒寄存器 偏移地址0xB0*/
    volatile unsigned int  UTS;                /**< UART Test Register, offset: 0xB4                 串口测试寄存器 偏移地址0xB4*/		
    volatile unsigned int  UMCR;               /**< UART RS-485 Mode Control Register, offset: 0xB8  串口485模式控制寄存器 偏移地址0xB8*/
  } UART_Type;
  
  UART_Type *uart1 = (UART_Type *)0x02020000;
  ```
  
  

## 4. 编程