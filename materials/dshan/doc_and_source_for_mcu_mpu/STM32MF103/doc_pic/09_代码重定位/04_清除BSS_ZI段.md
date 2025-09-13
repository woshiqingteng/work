# 清除BSS段(ZI段)

## 1. C语言中的BSS段
程序里的全局变量，如果它的初始值为0，或者没有设置初始值，这些变量被放在BSS段里，也叫ZI段。
```c
char g_Char = 'A';
const char g_Char2 = 'B';
int g_A = 0;  // 放在BSS段
int g_B;      // 放在BSS段
```
BSS段并不会放入bin文件中，否则也太浪费空间了。
在使用BSS段里的变量之前，把BSS段所占据的内存清零就可以了。

* 注意：对于keil来说，一个本该放到BSS段的变量，如果它所占据的空间小于等于8字节自己，keil仍然会把它放在data段里。只有当它所占据的空间大于8字节时，才会放到BSS段。

  ```c
  int g_A[3] = {0, 0};   // 放在BSS段
  char g_B[9];           // 放在BSS段
  
  int g_A[2] = {0, 0};   // 放在data段
  char g_B[8];           // 放在data段
  ```

  

## 2. 清除BSS段

### 2.1 BSS段在哪？多大？

在散列文件中，BSS段(ZI段)在可执行域`RW_IRAM1`中描述：

```
LR_IROM1 0x08000000 0x00080000  {    ; load region size_region
  ER_IROM1 0x08000000 0x00080000  {  ; load address = execution address
   *.o (RESET, +First)
   *(InRoot$$Sections)
   .ANY (+RO)
   .ANY (+XO)
  }
  RW_IRAM1 0x20000000 0x00010000  {  ; RW data
   .ANY (+RW +ZI)
  }
}
```

BSS段(ZI段)的链接地址(基地址)、长度，使用下面的符号获得：
![](lesson\relocate\004_bss_section_symbol.png)

### 2.2  怎么清除BSS段

#### 2.2.1 汇编码

```
IMPORT |Image$$RW_IRAM1$$ZI$$Base|
IMPORT |Image$$RW_IRAM1$$ZI$$Length|

LDR R0, = |Image$$RW_IRAM1$$ZI$$Base|       ; DEST
LDR R1, = |Image$$RW_IRAM1$$ZI$$Length|     ; Length
BL memset
```

#### 2.2.2 C语言
  * 方法1
声明为外部变量，使用时**需要**使用取址符：

```c
extern int Image$$RW_IRAM1$$ZI$$Base;
extern int Image$$RW_IRAM1$$ZI$$Length;

memset(&Image$$RW_IRAM1$$ZI$$Base, 0, &Image$$RW_IRAM1$$ZI$$Length);
```
  *  方法2
     声明为外部数组，使用时**不需要**使用取址符：

```c
extern char Image$$RW_IRAM1$$ZI$$Base[];
extern int Image$$RW_IRAM1$$ZI$$Length[];

memset(Image$$RW_IRAM1$$ZI$$Base[], 0, Image$$RW_IRAM1$$ZI$$Length);
```