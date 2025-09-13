# IDE背后的命令 #
## 1. IDE是什么
IDE指集成开发环境(Integrated Development Environment)。
我们开发STM32F103等单片机程序时使用是keil就是一种IDE。

使用IDE，很容易操作，点点鼠标就可完成：

* 添加文件
* 指定文件路径(头文件路径、库文件路径)
* 指定链接库
* 编译、链接
* 下载、调试



## 2. IDE的背后是命令

现场使用keil来演示命令的操作。

* 注意
  * 使用GitBash执行命令的话
    由于GitBash采用类似Linux的文件路径表示方法(比如 /d/abc，而非 d:\abc)，命令行中windows格式的路径名要加上双引号，比如`".\objects\main.o"`
  * 使用dos命令行执行命令的话
    不需要加双引号

* 在某个Keil工程所在目录下，打开Git Bash：
  `doc_and_source_for_mcu_mpu\STM32MF103\source\02_录制视频时现场编写的源码\01_led_c`

* 编译main.c
  执行命令：

  ```
  "C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --c99 --gnu -c --cpu Cortex-M3 -D__EVAL -g -O0 --apcs=interwork --split_sections -I.\RTE\_led_c -I"C:\Users\thisway_diy\AppData\Local\Arm\Packs\Keil\STM32F1xx_DFP\2.3.0\Device\Include" -I"C:\Keil_v5\ARM\CMSIS\Include" -D__UVISION_VERSION="527" -DSTM32F10X_HD -o ".\objects\main.o" --omf_browse ".\objects\main.crf" --depend ".\objects\main.d" "main.c"
  ```

* 编译start.S
  执行命令：

  ```
  "C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --cpu Cortex-M3 --pd "__EVAL SETA 1" -g --apcs=interwork -I.\RTE\_led_c -I"C:\Users\thisway_diy\AppData\Local\Arm\Packs\Keil\STM32F1xx_DFP\2.3.0\Device\Include" -I"C:\Keil_v5\ARM\CMSIS\Include" --pd "__UVISION_VERSION SETA 527" --pd "STM32F10X_HD SETA 1" --list ".\listings\start.lst" --xref -o ".\objects\start.o" --depend ".\objects\start.d" "start.s"
  ```

* 链接
  执行命令：

  ```
  "C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --cpu Cortex-M3 ".\objects\main.o" ".\objects\start.o" --ro-base 0x08000000 --entry 0x08000000 --rw-base 0x20000000 --entry Reset_Handler --first __Vectors --strict --summary_stderr --info summarysizes --map --load_addr_map_info --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --list ".\Listings\led_c.map" -o ".\Objects\led_c.axf"
  ```

  

## 3. 提出几个问题

* 头文件在哪？

* 库文件在哪？库文件是哪个？

* 源文件有哪些？

* 源文件怎么编译？可以指定编译参数吗？

* 多个源文件怎么链接成一个可执行程序？

* 有a.c, b.c, c.c，我只修改了a.c，就只需要编译a.c，然后在链接：怎么做到的？



## 4. 要解决这些疑问，需要了解命令行

如果你只学习单片机，只想使用keil，当然可以不学习命令行。
但是如果想升级到Linux、各类RTOS，需要掌握命令行。



## 5. 有两套主要的编译器

* armcc

  * ARM公司的编译器
  * keil使用的就是armcc
* gcc
  
  * GNU工具链
  * Linux等开源软件经常使用gcc
  

后面以GNU工具链为例讲解，所涉及的知识可以平移到armcc上。