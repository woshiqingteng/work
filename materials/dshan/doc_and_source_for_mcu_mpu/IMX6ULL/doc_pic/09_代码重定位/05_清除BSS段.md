# 清除BSS段

## 1. C语言中的BSS段
程序里的全局变量，如果它的初始值为0，或者没有设置初始值，这些变量被放在BSS段里。
```c
char g_Char = 'A';
const char g_Char2 = 'B';
int g_A = 0;  // 放在BSS段
int g_B;      // 放在BSS段
```
BSS段并不会放入bin文件中，否则也太浪费空间了。
在使用BSS段里的变量之前，把BSS段所占据的内存清零就可以了。




## 2. 清除BSS段

### 2.1 BSS段在哪？多大？

在链接脚本中，BSS段如下描述：

```
SECTIONS {
    . = 0xC0200000;   /* 对于STM32MP157设置链接地址为0xC0200000, 对于IMX6ULL设为0x80200000 */

    . = ALIGN(4);
    .text      :
    {
      *(.text)
    }

    . = ALIGN(4);
    __rodata_start = .;
    .rodata : { *(.rodata) }

    . = ALIGN(4);
    .data : { *(.data) }

    . = ALIGN(4);
    __bss_start = .;
    .bss : { *(.bss) *(.COMMON) }
    __bss_end = .;
}
```

BSS段的起始地址、结束地址，使用`__bss_start`和`__bss_end`来获得，它们是链接地址。


### 2.2  怎么清除BSS段

```
ldr r0, =__bss_start   /* 目的 */
mov r1, #0             /* 值 */
ldr r2, =__bss_end     
sub r2, r2, r1         /* 长度 */
bl memset              /* r0: 目的, r1: 值, r2: 长度 */
```

