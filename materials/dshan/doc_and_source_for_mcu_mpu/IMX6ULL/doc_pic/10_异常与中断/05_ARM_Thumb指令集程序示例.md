# ARM_Thumb指令集程序示例

## 1.1 汇编里指定指令集

对于IMX6ULL、STM32MP157，默认使用ARM指令集。
在汇编文件里，可以使用这样的语法告诉编译器：

### 1.1.1 新语法

```
.arm   // 表示后续的指令使用ARM指令集
.thumb // 表示后续的指令使用thumb指令集	
```

### 1.1.2 以前的语法

```
.code 32 // 表示后续的指令使用ARM指令集
.code 16 // 表示后续的指令使用thumb指令集
```



## 1.2 编译C文件时指定指令集

使用GCC编译时：

```
-marm     // 使用ARM指令集编译
-mthumb   // 使用Thumb指令集编译

比如：
arm-linux-gcc -marm -c -o main.o main.c
```

### 

## 1.3 汇编里切换状态

要切换CPU的State，比如从ARM切换到Thumb，或者从Thumb切换到ARM，可以使用BX、BLX指令：

```
// 如果R0的bit0为0，表示切换到ARM State; 如果R0的bit0位1，表示切换到Thumb State
BX R0   
BLX R0
```



汇编里调用C函数时，可以直接如此调用：

```
LDR PC, =main   // 如果main函数时用Thumb指令集编译的，最终的指令如下：

// 注意到下面的c020051b，它的bit0为1
c0200010:	e59ff004 	ldr	pc, [pc, #4]	; c020001c <AB+0x14>
c0200014:	c0100000 	andsgt	r0, r0, r0
c0200018:	c02004a3 	eorgt	r0, r0, r3, lsr #9
c020001c:	c020051b 	eorgt	r0, r0, fp, lsl r5
```

