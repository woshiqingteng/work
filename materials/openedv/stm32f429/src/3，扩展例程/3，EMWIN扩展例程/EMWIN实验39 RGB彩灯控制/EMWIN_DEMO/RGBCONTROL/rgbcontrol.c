#include "rgbcontrol.h"
#include "EmwinHzFont.h"
#include "pngdisplay.h"
#include "rgblight.h"
#include "ff.h"
#include "includes.h"
#include "math.h"
#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin RGB彩灯控制 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//控件ID
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_SLIDER_0 (GUI_ID_USER + 0x03)
#define ID_TEXT_0   (GUI_ID_USER + 0x04)
#define ID_IMAGE_0  (GUI_ID_USER + 0X05)

#define ID_PNG_DISPLAYEX 0x00       //IMAGE控件PNG显示ID

rgb_struct rgb_dev;                 //定义一个rgb控制结构体

//对话框资源表
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 800, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 70, 719, 100, 40, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 300, 720, 100, 40, 0, 0x0, 0 },
    { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 110, 630, 295, 33, 0, 0x0, 0 },
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 50, 130, 380, 380, 0, 0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 23, 641, 116, 20, 0, 0x64, 0 },
    
};

//获取颜色环图像数据
//返回值:0 成功，其他 失败
u8 GetColorLoopData(char* PNGFileName)
{
	u16 bread;
	char result;
	
	#if SYSTEM_SUPPORT_OS
		CPU_SR_ALLOC();
	#endif

	result=f_open(&rgb_dev.ColorLoopFile,(const TCHAR*)PNGFileName,FA_READ);	//打开文件
	//文件打开错误或者文件PNGMEMORYSIZE
	if((result!=FR_OK)||(rgb_dev.ColorLoopFile.obj.objsize>PNGMEMORYSIZE)) return 1;
    rgb_dev.colorloopsize=rgb_dev.ColorLoopFile.obj.objsize;
    if(rgb_dev.coloroop_buffer==NULL) rgb_dev.coloroop_buffer=mymalloc(SRAMEX,rgb_dev.ColorLoopFile.obj.objsize);
	if(rgb_dev.coloroop_buffer==NULL) return 2;
	
	#if SYSTEM_SUPPORT_OS
		OS_CRITICAL_ENTER();	//临界区
	#endif
		
	result = f_read(&rgb_dev.ColorLoopFile,rgb_dev.coloroop_buffer,rgb_dev.ColorLoopFile.obj.objsize,(UINT *)&bread);//读取数据
	if(result != FR_OK) return 3;
	
	#if SYSTEM_SUPPORT_OS
		OS_CRITICAL_EXIT();	    //退出临界区
	#endif
    f_close(&rgb_dev.ColorLoopFile);//关闭文件
    return 0;
}

//获取图片数据,IMAGE小控件使用
//PNGFileName:图片在SD卡中的存储路径
//Id:显示类型(显示的是PNG?JEGP?BMP?)
//pSize:图片大小 
static const void * GetImageById(char* PNGFileName,U32 Id, U32 *pSize) 
{
    const char* buffer;
    switch (Id) 
    {
        case ID_PNG_DISPLAYEX:
            if(GetColorLoopData(PNGFileName)==0)
                buffer=(const void *)rgb_dev.coloroop_buffer;
            else buffer=NULL;
            *pSize=rgb_dev.colorloopsize;
            break;
    }
    return buffer;
}

//对话框回调函数
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
    const void * pData;
    int     NCode;
    int     Id;
    U32     FileSize;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_SetBkColor(0X0068646C);//设置背景颜色
            GUI_Clear();
            break;
        case WM_INIT_DIALOG:
            //初始化BUTTON
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetFont(hItem,&GUI_FontHZ16);
            BUTTON_SetText(hItem, "打开");
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            BUTTON_SetFont(hItem,&GUI_FontHZ16);
            BUTTON_SetText(hItem, "关闭");
        
            //初始化IMAGE
            hItem=WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
            pData=GetImageById("0:/APP/RGBLIGHT/colorloop.png",ID_PNG_DISPLAYEX,&FileSize);
            IMAGE_SetPNG(hItem,pData,FileSize);//显示颜色环
        
            //初始化SLIDER
            hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
            SLIDER_SetRange(hItem,0,100);//设置范围
            SLIDER_SetValue(hItem,50);
        
            //初始化TEXT
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetFont(hItem,&GUI_FontHZ16);
            TEXT_SetText(hItem, "RGB灯亮度:");
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) 
            {
                case ID_BUTTON_0: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            RGB_Open();     //打开RGB灯                       
                            break;
                    }
                    break;
                case ID_BUTTON_1: 
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            RGB_Close();    //关闭RGB灯
                            break;
                    }
                    break;
                case ID_SLIDER_0: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem=WM_GetDialogItem(pMsg->hWin,ID_SLIDER_0);
                            rgb_dev.brightness=SLIDER_GetValue(hItem); 
                            //设置RGB灯颜色，加入了亮度设置
                            RGB_SetColor(rgb_dev.red*rgb_dev.brightness/100,\
                                        rgb_dev.green*rgb_dev.brightness/100,rgb_dev.blue*rgb_dev.brightness/100);
                            break;
                    }
                    break;
                case ID_IMAGE_0: //IMAGE控件
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            rgb_dev.image_flag=1;   //IMAGE控件被点击
                            break;
                        case WM_NOTIFICATION_RELEASED:
                        case WM_NOTIFICATION_MOVED_OUT:
                            rgb_dev.image_flag=0;   //移出IMAGE控件区域
                            break;
                    }
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

//背景窗口的回调函数
static void _BkCallback(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
            GUI_SetBkColor(GUI_BLUE);//设置背景颜色
            GUI_Clear();
			break;	
		default:
			WM_DefaultProc(pMsg);
			break;
	}	
}

//颜色提取,提取出BGR888颜色格式中的各个颜色分量(EMWIN中颜色格式为BGR888)
//rgbcolor:要提取的BGR888的颜色
//red:提取出来的红色值
//green:提取出来的绿色值
//blue:提取出来的蓝色值
void ColorExtract(u32 rgbcolor,u8* red,u8* green,u8* blue)
{
    *red=rgbcolor&0XFF;
    *green=(rgbcolor>>8)&0XFF;
    *blue=(rgbcolor>>16)&0XFF;
}

//计算两点之间的距离
//x0,y0:起始坐标
//y0,y1:终止坐标
//返回值:计算出的距离
float CalculatteDistance(float x0,float y0,float x1,float y1)
{
    float Xsize,Ysize,Distance;
    Xsize=x0-x1;
    Ysize=y0-y1;
    if(Xsize<0) Xsize=-Xsize;
    if(Ysize<0) Ysize=-Ysize;
    Distance=sqrt(Xsize*Xsize+Ysize*Ysize);
    return Distance;
}
//RGB灯控制demo
void CreateRGBControlDemo(void) 
{
    int color,colorindex;
    float distance=0;
    rgb_dev.red=255;      //默认初始化为255，即白光
    rgb_dev.green=255;
    rgb_dev.blue=255;
    rgb_dev.image_flag=0;   
    rgb_dev.brightness=50;//默认50%亮度
    WM_SetCallback(WM_HBKWIN,_BkCallback);
    rgb_dev.AppWin=GUI_CreateDialogBox(_aDialogCreate,GUI_COUNTOF(_aDialogCreate),_cbDialog,WM_HBKWIN,0,0);
    while(1)
    {
        if(rgb_dev.image_flag)//当鼠标移进IMAGE控件的时候就进行颜色调整
        {
            GUI_PID_GetState(&rgb_dev.PidData);//获取鼠标当前的位置
            distance=CalculatteDistance((float)rgb_dev.PidData.x,(float)rgb_dev.PidData.y,240,320);
            if(distance>70&&distance<180)      //触摸点在颜色环内
            {
                colorindex=GUI_GetPixelIndex(rgb_dev.PidData.x,rgb_dev.PidData.y);  //获取颜色索引
                color=GUI_Index2Color(colorindex);                                  //转换为颜色值
                ColorExtract(color,&rgb_dev.red,&rgb_dev.green,&rgb_dev.blue);      //颜色分量提取
                RGB_SetColor(rgb_dev.red*rgb_dev.brightness/100,rgb_dev.green*rgb_dev.brightness/100,rgb_dev.blue*rgb_dev.brightness/100);//设置RGB灯颜色，加入了亮度设置
            }
        }
        GUI_Delay(100);
    }
}
