实验器材:
	阿波罗STM32F429开发板
	
实验目的:
	学习emWin/STemWin CHECKBOX控件的使用。
	
硬件资源:
	1,DS0(连接在PB1) 
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面) 
	3,ALIENTEK 2.8/3.5/4.3/7寸LCD模块(包括MCU屏和RGB屏,都支持) 
	4,所使用的LCD的触摸屏。
	
实验现象:	
	下载本例程到开发板上，默认会创建一个窗口，窗口上会有多个CHECKBOX控件，点按不同的CHECKBOX
	来实现多选的功能。其中有的CHECKBOX拥有三种选择状态,有的CHECKBOX被禁掉了。
	
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程在LCD_Init函数里面(在lcd.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,本例程针对MCU屏和RGB屏提供了两种工程，其中MCU屏为 STemWin MCU，此工程也可以用于RGB屏
	另外一个专门用于RGB屏的工程为STemWin RGB，此工程只能用于RGB屏！
	4，本例程默认使用的800*480的屏，如果使用其他分辨率小于800*480的屏会导致显示不完全！
	 

					正点原子@ALIENTEK
					2016-6-26
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com
