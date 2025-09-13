    这是我们修改后的STM32F429xx系列的共用启动文件。 
    在官方启动文件的基础上，主要增加了使能STM32F4的硬件浮点运算功能、设置Heap_Size为0、禁止了SystemInit函数的调用。
    对于寄存器版本的例程，我们可以直接使用startup_stm32f429xx.s这个文件。对于库函数版本，如果你想在汇编里面调用SystemInit函数，则只需取消对SystemInit函数的屏蔽即可。
 
                                  

		广州市星翼电子科技有限公司
                电话：020-38271790
                传真：020-36773971
	       	购买：http://shop62103354.taobao.com
                      http://shop62057469.taobao.com
               	技术支持论坛：www.openedv.com
                公司网址：www.alientek.com