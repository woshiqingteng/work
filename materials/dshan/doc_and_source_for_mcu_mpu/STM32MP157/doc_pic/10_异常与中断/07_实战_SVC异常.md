# 实战_SVC异常

要想深入理解异常处理，需要写程序来验证。
本节课程故意执行一条SVC指令，让它触发异常。

参考资料：`ARM ArchitectureReference Manual ARMv7-A and ARMv7-R edition.pdf`

## 1.1 A7的异常向量表

从向量表可以看出，A7支持哪些异常：未定义指令、软中断(SVC)、预取指令中止、数据中止、IRQ、FIQ。

```
_start: 
    b	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq
```


## 1.2 什么是SVC指令？

在ARM指令中，有一条指令：

```
SVC #VAL
```

它会触发一个异常。
在操作系统中，比如各类RTOS或者Linux，都会使用`SVC`指令故意触发异常，从而导致内核的异常处理函数被调用，进而去使用内核的服务。
比如Linux中，各类文件操作的函数`open`、`read`、`write`，它的实质都是`SVC`指令。
本节课程不讲解`SVC`在内核中的使用，我们只是看看如何处理`SVC`触发的异常。



## 1.3 在汇编代码里插入SVC指令

在代码中插入：

```
SVC #1
```

看看会发生什么事情。



## 1.4 编程

### 1.4.1 设置SVC模式的栈



### 1.4.2 保存现场



### 1.4.3 处理异常



### 1.4.4 恢复现场