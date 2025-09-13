# mksunxi简单分析



## 1. 作用

执行如下命令：

```shell
mksunxi led.bin
```

它会做如下事情：

* 在文件后面补零扩大led.bin，大小必须是Flash扇区的整数倍
* 修改头部信息：长度、校验码

![image-20230309112742018](pic/04_mksunxi.png)



## 2. 代码速览

