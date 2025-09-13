#include "rgbcontrol.h"
#include "EmwinHzFont.h"
#include "pngdisplay.h"
#include "rgblight.h"
#include "ff.h"
#include "includes.h"
#include "math.h"
#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin RGB�ʵƿ��� 
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//�ؼ�ID
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_SLIDER_0 (GUI_ID_USER + 0x03)
#define ID_TEXT_0   (GUI_ID_USER + 0x04)
#define ID_IMAGE_0  (GUI_ID_USER + 0X05)

#define ID_PNG_DISPLAYEX 0x00       //IMAGE�ؼ�PNG��ʾID

rgb_struct rgb_dev;                 //����һ��rgb���ƽṹ��

//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 800, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 70, 719, 100, 40, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 300, 720, 100, 40, 0, 0x0, 0 },
    { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 110, 630, 295, 33, 0, 0x0, 0 },
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 50, 130, 380, 380, 0, 0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 23, 641, 116, 20, 0, 0x64, 0 },
    
};

//��ȡ��ɫ��ͼ������
//����ֵ:0 �ɹ������� ʧ��
u8 GetColorLoopData(char* PNGFileName)
{
	u16 bread;
	char result;
	
	#if SYSTEM_SUPPORT_OS
		CPU_SR_ALLOC();
	#endif

	result=f_open(&rgb_dev.ColorLoopFile,(const TCHAR*)PNGFileName,FA_READ);	//���ļ�
	//�ļ��򿪴�������ļ�PNGMEMORYSIZE
	if((result!=FR_OK)||(rgb_dev.ColorLoopFile.obj.objsize>PNGMEMORYSIZE)) return 1;
    rgb_dev.colorloopsize=rgb_dev.ColorLoopFile.obj.objsize;
    if(rgb_dev.coloroop_buffer==NULL) rgb_dev.coloroop_buffer=mymalloc(SRAMEX,rgb_dev.ColorLoopFile.obj.objsize);
	if(rgb_dev.coloroop_buffer==NULL) return 2;
	
	#if SYSTEM_SUPPORT_OS
		OS_CRITICAL_ENTER();	//�ٽ���
	#endif
		
	result = f_read(&rgb_dev.ColorLoopFile,rgb_dev.coloroop_buffer,rgb_dev.ColorLoopFile.obj.objsize,(UINT *)&bread);//��ȡ����
	if(result != FR_OK) return 3;
	
	#if SYSTEM_SUPPORT_OS
		OS_CRITICAL_EXIT();	    //�˳��ٽ���
	#endif
    f_close(&rgb_dev.ColorLoopFile);//�ر��ļ�
    return 0;
}

//��ȡͼƬ����,IMAGEС�ؼ�ʹ��
//PNGFileName:ͼƬ��SD���еĴ洢·��
//Id:��ʾ����(��ʾ����PNG?JEGP?BMP?)
//pSize:ͼƬ��С 
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

//�Ի���ص�����
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
            GUI_SetBkColor(0X0068646C);//���ñ�����ɫ
            GUI_Clear();
            break;
        case WM_INIT_DIALOG:
            //��ʼ��BUTTON
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetFont(hItem,&GUI_FontHZ16);
            BUTTON_SetText(hItem, "��");
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            BUTTON_SetFont(hItem,&GUI_FontHZ16);
            BUTTON_SetText(hItem, "�ر�");
        
            //��ʼ��IMAGE
            hItem=WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
            pData=GetImageById("0:/APP/RGBLIGHT/colorloop.png",ID_PNG_DISPLAYEX,&FileSize);
            IMAGE_SetPNG(hItem,pData,FileSize);//��ʾ��ɫ��
        
            //��ʼ��SLIDER
            hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
            SLIDER_SetRange(hItem,0,100);//���÷�Χ
            SLIDER_SetValue(hItem,50);
        
            //��ʼ��TEXT
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetFont(hItem,&GUI_FontHZ16);
            TEXT_SetText(hItem, "RGB������:");
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
                            RGB_Open();     //��RGB��                       
                            break;
                    }
                    break;
                case ID_BUTTON_1: 
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            RGB_Close();    //�ر�RGB��
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
                            //����RGB����ɫ����������������
                            RGB_SetColor(rgb_dev.red*rgb_dev.brightness/100,\
                                        rgb_dev.green*rgb_dev.brightness/100,rgb_dev.blue*rgb_dev.brightness/100);
                            break;
                    }
                    break;
                case ID_IMAGE_0: //IMAGE�ؼ�
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            rgb_dev.image_flag=1;   //IMAGE�ؼ������
                            break;
                        case WM_NOTIFICATION_RELEASED:
                        case WM_NOTIFICATION_MOVED_OUT:
                            rgb_dev.image_flag=0;   //�Ƴ�IMAGE�ؼ�����
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

//�������ڵĻص�����
static void _BkCallback(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
            GUI_SetBkColor(GUI_BLUE);//���ñ�����ɫ
            GUI_Clear();
			break;	
		default:
			WM_DefaultProc(pMsg);
			break;
	}	
}

//��ɫ��ȡ,��ȡ��BGR888��ɫ��ʽ�еĸ�����ɫ����(EMWIN����ɫ��ʽΪBGR888)
//rgbcolor:Ҫ��ȡ��BGR888����ɫ
//red:��ȡ�����ĺ�ɫֵ
//green:��ȡ��������ɫֵ
//blue:��ȡ��������ɫֵ
void ColorExtract(u32 rgbcolor,u8* red,u8* green,u8* blue)
{
    *red=rgbcolor&0XFF;
    *green=(rgbcolor>>8)&0XFF;
    *blue=(rgbcolor>>16)&0XFF;
}

//��������֮��ľ���
//x0,y0:��ʼ����
//y0,y1:��ֹ����
//����ֵ:������ľ���
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
//RGB�ƿ���demo
void CreateRGBControlDemo(void) 
{
    int color,colorindex;
    float distance=0;
    rgb_dev.red=255;      //Ĭ�ϳ�ʼ��Ϊ255�����׹�
    rgb_dev.green=255;
    rgb_dev.blue=255;
    rgb_dev.image_flag=0;   
    rgb_dev.brightness=50;//Ĭ��50%����
    WM_SetCallback(WM_HBKWIN,_BkCallback);
    rgb_dev.AppWin=GUI_CreateDialogBox(_aDialogCreate,GUI_COUNTOF(_aDialogCreate),_cbDialog,WM_HBKWIN,0,0);
    while(1)
    {
        if(rgb_dev.image_flag)//������ƽ�IMAGE�ؼ���ʱ��ͽ�����ɫ����
        {
            GUI_PID_GetState(&rgb_dev.PidData);//��ȡ��굱ǰ��λ��
            distance=CalculatteDistance((float)rgb_dev.PidData.x,(float)rgb_dev.PidData.y,240,320);
            if(distance>70&&distance<180)      //����������ɫ����
            {
                colorindex=GUI_GetPixelIndex(rgb_dev.PidData.x,rgb_dev.PidData.y);  //��ȡ��ɫ����
                color=GUI_Index2Color(colorindex);                                  //ת��Ϊ��ɫֵ
                ColorExtract(color,&rgb_dev.red,&rgb_dev.green,&rgb_dev.blue);      //��ɫ������ȡ
                RGB_SetColor(rgb_dev.red*rgb_dev.brightness/100,rgb_dev.green*rgb_dev.brightness/100,rgb_dev.blue*rgb_dev.brightness/100);//����RGB����ɫ����������������
            }
        }
        GUI_Delay(100);
    }
}
