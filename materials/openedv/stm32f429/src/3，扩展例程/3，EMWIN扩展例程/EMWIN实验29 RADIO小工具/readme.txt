实验器材:
	阿波罗STM32F429开发板
	
实验目的:
	学习emWin/STemWin RADIO控件的使用。
	
硬件资源:
	1,DS0(连接在PB1) 
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面) 
	3,ALIENTEK 2.8/3.5/4.3/7寸LCD模块(包括MCU屏和RGB屏,都支持) 
	4,所使用的LCD的触摸屏。
	
实验现象:	
	下载程序到开发板，程序默认创建了一个FRAMWIN窗口，在FRAMWIN窗口上创建了两个控件，分别为
	一个RADIO控件、一个BUTTON控件。在RADIO控件中选中一个选项，然后按下OK键，FRAMWIN就会消失，
	背景窗口颜色就会改变为刚刚选中的颜色，并且背景窗口上也会显示当前选中的RADIO控件的值。
	
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程在LCD_Init函数里面(在lcd.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,本例程针对MCU屏和RGB屏提供了两种工程，其中MCU屏为 STemWin MCU，此工程也可以用于RGB屏
	另外一个专门用于RGB屏的工程为STemWin RGB，此工程只能用于RGB屏！
	 

					正点原子@ALIENTEK
					2016-6-26
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com
