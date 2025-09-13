#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "usmart.h"
#include "malloc.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验38
 内存管理实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 
int main(void)
{
    u8 paddr[20];				    //存放P Addr:+p地址的ASCII值
	u16 memused=0;
	u8 key;		 
 	u8 i=0;	    
	u8 *p=0;
	u8 *tp=0;
	u8 sramx=0;					    //默认为内部sram
    
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    usmart_dev.init(90); 		    //初始化USMART	
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
    LCD_Init();                     //初始化LCD
    
    my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMCCM);		    //初始化CCM内存池
     
   	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"MALLOC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");	 		
	LCD_ShowString(30,130,200,16,16,"KEY0:Malloc  KEY2:Free");
	LCD_ShowString(30,150,200,16,16,"KEY_UP:SRAMx KEY1:Read"); 
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(30,170,200,16,16,"SRAMIN");
	LCD_ShowString(30,190,200,16,16,"SRAMIN  USED:");
	LCD_ShowString(30,210,200,16,16,"SRAMEX  USED:");
	LCD_ShowString(30,230,200,16,16,"SRAMCCM USED:");
 	while(1)
	{	
		key=KEY_Scan(0);//不支持连按	
		switch(key)
		{
			case 0://没有按键按下	
				break;
			case KEY0_PRES:	//KEY0按下
				p=mymalloc(sramx,2048);//申请2K字节
				if(p!=NULL)sprintf((char*)p,"Memory Malloc Test%03d",i);//向p写入一些内容
				break;
			case KEY1_PRES:	//KEY1按下	   
				if(p!=NULL)
				{
					sprintf((char*)p,"Memory Malloc Test%03d",i);//更新显示内容 	 
					LCD_ShowString(30,270,200,16,16,p);			 //显示P的内容
				}
				break;
			case KEY2_PRES:	//KEY2按下	  
				myfree(sramx,p);//释放内存
				p=0;			//指向空地址
				break;
			case WKUP_PRES:	//KEY UP按下 
				sramx++; 
				if(sramx>2)sramx=0;
				if(sramx==0)LCD_ShowString(30,170,200,16,16,"SRAMIN ");
				else if(sramx==1)LCD_ShowString(30,170,200,16,16,"SRAMEX ");
				else LCD_ShowString(30,170,200,16,16,"SRAMCCM");
				break;
		}
		if(tp!=p&&p!=NULL)
		{
			tp=p;
			sprintf((char*)paddr,"P Addr:0X%08X",(u32)tp);
			LCD_ShowString(30,250,200,16,16,paddr);	//显示p的地址
			if(p)LCD_ShowString(30,270,200,16,16,p);//显示P的内容
		    else LCD_Fill(30,270,239,266,WHITE);	//p=0,清除显示
		}
		delay_ms(10);   
		i++;
		if((i%20)==0)//DS0闪烁.
		{
			memused=my_mem_perused(SRAMIN);
			sprintf((char*)paddr,"%d.%01d%%",memused/10,memused%10);
			LCD_ShowString(30+104,190,200,16,16,paddr);	//显示内部内存使用率
			memused=my_mem_perused(SRAMEX);
			sprintf((char*)paddr,"%d.%01d%%",memused/10,memused%10);
			LCD_ShowString(30+104,210,200,16,16,paddr);	//显示外部内存使用率
			memused=my_mem_perused(SRAMCCM);
			sprintf((char*)paddr,"%d.%01d%%",memused/10,memused%10);
			LCD_ShowString(30+104,230,200,16,16,paddr);	//显示CCM内存使用率 
 			LED0=!LED0;
 		}
	} 
}
