# 使用按键控制LED
## 1. 先看原理图

* 100ASK IMX6ULL按键原理图

![](lesson\lesson_gpio_led\001_imx6ull_key_sch.png)

* 我们使用KEY2来控制LED：按下KEY2则灯亮，松开后灯灭

* 课后作业：使用KEY1来控制LED(我在视频里演示KEY2，它更复杂一点)

* KEY2用的是GPIO04_IO14引脚


## 2. 再看芯片手册

### 2.1 使能GPIO4模块

**CCM_CCGR3地址：20C_4000h base + 74h offset = 0x020C4074  **

![](lesson\lesson_gpio_led\003_enable_gpio4.png)

### 2.2 设置引脚工作于GPIO模式
**IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B   地址：20E_0000h base + 1B0h offset = 0x020E01B0  **

![](lesson\lesson_gpio_led\004_config_gpio04_io14_as_gpio.png)

### 2.3 设置引脚为输入引脚
**GPIO4_GDIR地址：0x020A8004 **

![](lesson\lesson_gpio_led\005_confgigpio_dir.png)

### 2.4 读取引脚值
**GPIO4_DR地址：0x020A8000 **

![image-20201019115820020](lesson\lesson_gpio_led\006_read_gpio_data.png)

## 3. 现场写程序

写好的源码：`doc_and_source_for_mcu_mpu\IMX6ULL\source\02_录制视频时现场编写的源码\04_key_led`