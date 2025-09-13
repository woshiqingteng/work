# 使用按键控制LED
## 1. 先看原理图

* 100ASK STM32MP157按键原理图

![](lesson\lesson_gpio_led\001_stm32mp157_key_sch.png)

* 我们使用KEY1来控制LED：按下KEY1则灯亮，松开后灯灭

* KEY1用的是GPG3引脚


## 2. 再看芯片手册

### 2.1 使能PLL4

**RCC_PLL4CR地址：0x50000000 + 0x894**

![](lesson\lesson_gpio_led\002_enable_pll4.png)

### 2.2 使能GPIOG模块

**A7、M4对应的寄存器地址不一样，位含义一样**
**RCC_MP_AHB4ENSETR地址：0x50000000 + 0xA28**
**RCC_MC_AHB4ENSETR地址：0x50000000 + 0xAA8  **

![](lesson\lesson_gpio_led\003_enable_gpiog.png)

### 2.3 设置引脚为输入引脚
**GPIOG_MODER地址：0x50008000 + 0x00 **

![](lesson\lesson_gpio_led\004_config_gpio_input.png)

### 2.4 读取引脚值
**GPIOG_IDR地址：0x50008000 + 0x10**

![](lesson\lesson_gpio_led\005_read_gpio.png)

## 3. 现场写程序

写好的源码：`doc_and_source_for_mcu_mpu\STM32MP157\source\02_录制视频时现场编写的源码\04_key_led`