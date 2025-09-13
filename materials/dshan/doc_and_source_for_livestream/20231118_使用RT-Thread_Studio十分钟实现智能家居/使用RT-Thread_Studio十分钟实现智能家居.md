# 使用RT-Thread_Studio十分钟实现智能家居

资料下载：

```shell
https://e.coding.net/weidongshan/livestream/doc_and_source_for_livestream.git
```

![image-20231118145240330](pic/03_download.png)

## 1. 创建工程

本节课程源码为：



启动RT-Thread Studio后，可以看到如下界面：

![image-20231108104346670](pic/rtt/01_rt-thread_startup.png)

选择你的目录，点击"启动"。

在如下界面点击""

![image-20231108104612038](pic/rtt/02_create.png)

然后配置工程：选择芯片、调试串口、调试器等：

![image-20231108105448262](pic/rtt/03_config.png)



## 2. 编译体验

在工程界面，选中工程后，如下图操作：

![image-20231108105735501](pic/rtt/04_build.png)

然后点击如下图标烧写程序：

![image-20231108111234681](pic/rtt/05_burn.png)



在串口工具里就可以看到启动记录：

![image-20231108111426732](pic/rtt/06_bootup.png)



## 3. 使用DHT11

本节课程源码为：



做实验之前，先如下图接上DHT11模块：

![image-20231118144838733](pic/02_connect_dht11.png)



### 3.1 添加DHT11

如下图添加软件包：

![image-20231108140125139](pic/rtt/07_rtt_setting.png)

然后搜索DHT11并添加：

![image-20231108144011771](pic/rtt/08_dht11.png)

添加后，使用快捷键"Ctrl + S"保存配置，它会自动进行配置。

### 3.2 解决编译错误

使用快捷键"Ctrl + B"编译程序，会出现如下错误：

* drv_gpio.h无法找到
* sensor.h无法找到
* rt_hw_sensor_register函数未定义



对于drv_gpio.h，在`dht11_sample.c`中改为：

```c
#include "drv_common.h"
```



对于sensor.h，添加头文件目录：

//${ProjName}//rt-thread/components/drivers/include/drivers

如下图操作：

![image-20231108153610554](pic/rtt/10_sensor_h.png)



对于rt_hw_sensor_register函数未定义，配置组件：

![image-20231108152304115](pic/rtt/09_sensor.png)



修改使用的引脚：

![image-20231118144710299](pic/rtt/17_change_pin.png)



最后重新编译、运行程序，在串口上可以看到：

![image-20231108154145196](pic/rtt/11_dht11_ok.png)

### 3.3 改进程序

重启板子时，经常看见如下错误：

![image-20231108154245340](pic/rtt/12_dht11_err.png)

原因是刚复位、上电时，DHT11尚未准备就绪，如下添加延时即可：

![image-20231108154504327](pic/rtt/13_dht11_ok.png)



## 4. 使用AT设备ESP8266

本节课程源码：



做实验之前，先接上ESP8288:

![image-20231118144455386](pic/01_connect_esp8266.png)

### 4.1 添加AT设备

如下图操作：

![image-20231109082231038](pic/rtt/18_add_at_device1.png)

![image-20231109082633482](pic/rtt/18_add_at_device2.png)

使用快捷键"Ctrl + S"保存后编译，有如下错误：

![image-20231118181447057](pic/04_err_esp8266.png)



![image-20231109083223404](pic/rtt/19_at_err.png)

在百度搜索错误信息"rt-thread undefined reference to `_sbrk'“，得到解决方法：

![image-20231109083345282](pic/rtt/20_at_err_fix.png)



再次使用快捷键"Ctrl + S"保存后编译，烧写、测试，有如下错误：

![image-20231109084030697](pic/rtt/21_at_not_uart3.png)



在源码里搜"uart1"，仿照uart1添加uart3，在`drivers\board.h`添加如下代码：

```c
#define BSP_USING_UART1
#define BSP_UART1_TX_PIN       "PA9"
#define BSP_UART1_RX_PIN       "PA10"

/* 100ask添加的 */
#define BSP_USING_UART2
#define BSP_UART2_TX_PIN       "PA2"
#define BSP_UART2_RX_PIN       "PA3"
```

### 4.2 添加AT客户端

开发板是AT Client，ESP8266是AT Server:

![image-20231109085212817](pic/rtt/24_at_client_server.png)

在开发板上运行的程序要添加AT Client（默认已经添加），如下操作：

![image-20231109085324889](pic/rtt/25_enable_at_client.png)

### 4.3 测试

重新编译 、烧写，可以成功识别出AT设备，然后可以执行ifconfig命令查看IP、使用ping命令测试网络：

![image-20231109095420678](pic/rtt/22_at_dev_ok.png)



## 5. ESP8266驱动分析

### 5.1 层次

* 上层APP或其他协议，只看到netdev，它不在意这个网卡是使用AT命令实现的，还是通过其他方式实现的
* 对于ESP8266，它是使用AT命令实现的，它对应的netdev的操作函数里，要使用AT命令
  * netdev的函数要使用AT命令，就是使用下面的at_device提供的函数
  * at_device
* 我们可能使用多个ESP8266，它们的通用代码放在at_deivce_class里
* 每个ESP8266通过一个串口跟板子连接，这个串口分为2层
  * 上层：at_client，实现AT命令相关操作
  * 下层：纯粹的UART驱动（对应rt_device_t），只是实现UART收发

![image-20231111142500589](pic/rtt/36_net_dev_levels.png)





## 6. 使用MQTT

本节源码：



### 6.1 添加MQTT

如下图操作，添加MQTT：

![image-20231109180814276](pic/rtt/27_add_mqtt.png)



然后进行配置：

![image-20231109180859206](pic/rtt/30_config_mqtt.png)

最后修改源码`packages\pahomqtt-v1.1.0\samples\mqtt_sample.c`，代码如下：

```c
#define MQTT_URI                "tcp://iot.100ask.net:1883"
#define MQTT_USERNAME           "admin"
#define MQTT_PASSWORD           "public"
#define MQTT_SUBTOPIC           "/topic/ctrl"
#define MQTT_PUBTOPIC           "/topic/humiture"
#define MQTT_WILLMSG            "Goodbye!"
```

### 6.2 编译测试

1. 启动微信小程序"百问网嵌入式物联网"

   进去后，如下图点击：

   ![image-20231111174410338](pic/rtt/38_wechat.png)

2. 在串口操作

```shell
// 连接服务器
mqtt_start

// 发布消息
mqtt_publish /topic/humiture {\"Temperature\":1,\"Humidity\":9}
```

同时，可以在微信小程序点击电灯、风扇等图标，观察串口信息。



注意：如果小程序不好用，如下操作重新进入：

![image-20231111174621079](pic/rtt/39_reboot_wechat.png)



