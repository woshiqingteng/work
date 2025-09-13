# 准备工作 #

## 1. arm-linux-gcc和gcc是类似的

* arm-linux-gcc
  * 给ARM芯片编译程序
* gcc
  * 在x86编译程序
* 用法基本一样
* 为方便演示，我们使用gcc
* 为了方便在windows下演示，我们使用`Code::Blocks`
  * 它的安装程序自带gcc


## 2.  Code::Blocks
它是一款基于GCC的windows IDE，可以用来开发C/C++/Fortran。
官网地址：`http://www.codeblocks.org/`

![](lesson\gcc\003_download_codeblocks.png)

在我们提供的GIT仓库里也有：`git clone https://e.coding.net/weidongshan/noos/cortexA7_windows_tools.git`
下载GIT后，在apps目录下。


### 2.1 安装

双击安装。

### 2.2 设置windows环境变量

在Path环境变量中添加：`C:\Program Files\CodeBlocks\MinGW\bin`	

### 2.3 命令行示例

启动Git Bash，编译程序hello.c: 

```c

#include <stdio.h>

int main(void)
{
	printf("hello, world!\n");
	return 0;
}
```

编译、运行命令如下：

```
gcc -o hello  hello.c
./hello.exe
```

