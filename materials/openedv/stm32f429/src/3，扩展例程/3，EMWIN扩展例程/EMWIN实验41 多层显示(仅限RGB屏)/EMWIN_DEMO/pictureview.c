#include <stddef.h>
#include "pictureview.h"
#include "GUI.h"
#include "sys.h"
#include "ff.h"
#include "delay.h"
#include "exfuns.h"
#include "malloc.h"
#include "string.h"
#include "jpegdisplay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin 双层显示 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/6/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//垂直刷新(从上往下绘制水平线)
//xSize:屏幕宽度
//ySize:屏幕高度
//Delay:延时
static void EffectLineV0(int xSize,int ySize,int Delay) 
{
    int i;
    int j;

    GUI_SetColor(GUI_TRANSPARENT);      //设置颜色为透明
    for(i=0;i<32;i++) 
    {
        for (j=i;j<ySize;j+=32) 
        {
            GUI_DrawHLine(j,0,xSize-1); //绘制水平线
        }
        GUI_X_Delay(Delay);
    }
}

//垂直刷新(从下往上绘制水平线)
//xSize:屏幕宽度
//ySize:屏幕高度
//Delay:延时
static void EffectLineV1(int xSize,int ySize,int Delay) 
{
    int i;
    int j;
    
    GUI_SetColor(GUI_TRANSPARENT);      //设置颜色为透明
    for(i=31;i>=0;i--) 
    {
        for(j=i;j<ySize;j+=32) 
        {
            GUI_DrawHLine(j,0,xSize-1); //绘制水平线
        }
        GUI_X_Delay(Delay);
    }
}

//水平更新(从左往右绘制垂直线)
//xSize:屏幕宽度
//ySize:屏幕高度
//Delay:延时
static void EffectLineH0(int xSize,int ySize,int Delay) 
{
    int i;
    int j;

    GUI_SetColor(GUI_TRANSPARENT);      //设置颜色为透明
    for(i=0;i<32;i++) 
    {
        for(j=i;j<xSize;j+=32) 
        {
            GUI_DrawVLine(j,0,ySize-1); //绘制垂直线
        }
        GUI_X_Delay(Delay);
    }
}

//绘制垂直线(从右往左绘制垂直线)
//xSize:屏幕宽度
//ySize:屏幕高度
//Delay:延时
static void EffectLineH1(int xSize,int ySize,int Delay) 
{
    int i;
    int j;
    GUI_SetColor(GUI_TRANSPARENT);      //设置颜色为透明
    
    for(i=31;i>=0;i--) 
    {
        for (j=i;j<xSize;j+=32) 
        {
            GUI_DrawVLine(j,0,ySize-1); //绘制垂直线
        }
        GUI_X_Delay(Delay);
    }
}

//矩形更新，即同时进行水平和垂直更新(从左上角向右下角绘制)
//xSize:屏幕宽度
//ySize:屏幕高度
//Delay:延时
static void EffectLineHV0(int xSize, int ySize, int Delay) 
{
    int i;
    int j;
    
    GUI_SetColor(GUI_TRANSPARENT);      //设置颜色为透明
    for(i=0;i<32;i++) 
    {
        for(j=i;j<ySize;j+=32) 
        {
            GUI_DrawHLine(j,0,xSize-1); //绘制水平线
        }
        for(j=i;j<xSize;j+=32)
        {    
            GUI_DrawVLine(j,0,ySize-1); //绘制垂直线
        }
        GUI_X_Delay(Delay);
    }
}

//矩形更新，即同时进行水平和垂直更新(从右小角向左上角绘制)
//xSize:屏幕宽度
//ySize:屏幕高度
//Delay:延时
static void EffectLineHV1(int xSize,int ySize,int Delay) 
{
    int i;
    int j;
    
    GUI_SetColor(GUI_TRANSPARENT);      //设置颜色为透明
    for(i=31;i>=0;i--) 
    {
        for(j=i;j<ySize;j+=32) 
        {
            GUI_DrawHLine(j,0,xSize-1); //绘制水平线
        }
        for(j=i;j<xSize;j+=32) 
        {
            GUI_DrawVLine(j,0,ySize-1); //绘制垂直线
        }
        GUI_X_Delay(Delay);
    }
}

//图片更新函数数组
static void (* _apFunc[])(int xSize,int ySize,int Delay)= 
{
    EffectLineV0,
    EffectLineV1,
    EffectLineH0,
    EffectLineH1,
    EffectLineHV0,
    EffectLineHV1,
};

//填充当前层
//picname:要显示的图片
static void FillCurrentLayer(u8 *picname) 
{
    u8 err=0;
    GUI_MULTIBUF_Begin();
    err=displyjpeg(picname,1,0,0,1,1);//在LCD中间显示图片
    if(err) printf("图片%s显示出错,错误码%d\r\n",picname,err);
    GUI_MULTIBUF_End();
}

//图片显示
//pFunc:图片更新函数
//Index:层索引
//Delay:延时
static void ShowPicture(void (* pFunc)(int xSize, int ySize, int Delay),u8 *picname,int Delay) 
{
    int xSize;
    int ySize;

    xSize=LCD_GetXSize(); //获取屏幕宽度
    ySize=LCD_GetYSize(); //获取屏幕高度

    //绘制第0层的图片，不可见！因为被第1层覆盖了
    GUI_SelectLayer(0);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    FillCurrentLayer(picname);
    //第1层用透明色填充，这样第1层的图片就可以看到了
    GUI_SelectLayer(1);
    GUI_SetColor(GUI_TRANSPARENT);
    pFunc(xSize,ySize,Delay);

    //第1层也显示这张图片，只有第一张也显示了第0层的图片
    //才会在显示下一张照片的时候有更新的效果出现。
    FillCurrentLayer(picname);
}

//得到path路径下,目标文件的总个数
//path:路径		    
//返回值:总有效文件数
u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO *tfileinfo;	//临时文件信息	    			     
	tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));//申请内存
    res=f_opendir(&tdir,(const TCHAR*)path); 	//打开目录 
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//查询总的有效文件数
		{
	        res=f_readdir(&tdir,tfileinfo);       		//读取目录下的一个文件  	 
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;//错误了/到末尾了,退出	 		 
			res=f_typetell((u8*)tfileinfo->fname);
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件	
			{
				rval++;//有效文件数增加1
			}	    
		}  
	}  
	myfree(SRAMIN,tfileinfo);//释放内存
	return rval;
}

//例程演示Demo
void PictureViewDemo(void)
{
    int Index=0;
    u8 res;
 	DIR picdir;	 		    //图片目录
	FILINFO *picfileinfo;   //文件信息
	u8 *pname;			    //带路径的文件名
	u16 totpicnum; 		    //图片文件总数
	u16 curindex;		    //图片当前索引
    u16 temp;
    u32 *picoffblktbl;	    //图片文件off block索引表 
    
    totpicnum=pic_get_tnum("0:/APP/PICTUREVIEW");           //得到总有效文件数
    picfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//申请内存
 	pname=mymalloc(SRAMIN,_MAX_LFN*2+1);					//为带路径的文件名分配内存
 	picoffblktbl=mymalloc(SRAMIN,4*totpicnum);				//申请4*totpicnum个字节的内存,用于存放图片索引
 	while(!picfileinfo||!pname||!picoffblktbl)				//内存分配出错
 	{	    
		printf("内存申请失败\r\n");
		delay_ms(200);				  				  
	} 
    //记录索引
    res=f_opendir(&picdir,"0:/APP/PICTUREVIEW"); //打开目录
	if(res==FR_OK)
	{
		curindex=0;//当前索引为0
		while(1)//全部查询一遍
		{
	        res=f_readdir(&picdir,picfileinfo);       		//读取目录下的一个文件
			temp=picdir.blk_ofs;							//记录当前block偏移
	        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出	 	 
			res=f_typetell((u8*)picfileinfo->fname);	
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件	
			{
				picoffblktbl[curindex]=temp;//记录索引
				curindex++;
			}	    
		} 
	}  
	delay_ms(1500);  	   
	curindex=0;											//从0开始显示
   	res=f_opendir(&picdir,(const TCHAR*)"0:/APP/PICTUREVIEW"); 	//打开目录
    while(1) 
    {
		dir_sdi(&picdir,picoffblktbl[curindex]);		//改变当前目录索引	   
        res=f_readdir(&picdir,picfileinfo);       		//读取目录下的一个文件
        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出
		strcpy((char*)pname,"0:/APP/PICTUREVIEW/");	    //复制路径(目录)
		strcat((char*)pname,(const char*)picfileinfo->fname);//将文件名接在后面
        ShowPicture(_apFunc[Index++],pname,25);
        if(Index==6) Index=0;
        curindex++;		   	    
        if(curindex>=totpicnum)curindex=0;//到末尾的时候,自动从头开始
        GUI_Delay(2000);
    }
    myfree(SRAMIN,picfileinfo);			//释放内存						   		    
	myfree(SRAMIN,pname);				//释放内存			    
	myfree(SRAMIN,picoffblktbl);		//释放内存
}
