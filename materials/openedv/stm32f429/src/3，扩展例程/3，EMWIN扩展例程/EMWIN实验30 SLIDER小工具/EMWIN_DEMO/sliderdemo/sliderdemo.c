#include "sliderdemo.h"
#include "DIALOG.h"
#include "usart.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin SLIDER�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//�ؼ�ID
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
#define ID_SLIDER_0     (GUI_ID_USER + 0x01)
#define ID_SLIDER_1     (GUI_ID_USER + 0x02)
#define ID_SLIDER_2     (GUI_ID_USER + 0x03)
#define ID_SLIDER_3     (GUI_ID_USER + 0x04)
#define ID_EDIT_0       (GUI_ID_USER + 0x05)
#define ID_EDIT_1       (GUI_ID_USER + 0x06)
#define ID_EDIT_2       (GUI_ID_USER + 0x07)
#define ID_TEXT_0       (GUI_ID_USER + 0x08)
#define ID_TEXT_1       (GUI_ID_USER + 0x09)
#define ID_TEXT_2       (GUI_ID_USER + 0x0A)
#define ID_TEXT_3       (GUI_ID_USER + 0x0B)
#define ID_TEXT_4       (GUI_ID_USER + 0x0C)
#define ID_TEXT_5       (GUI_ID_USER + 0x0D)

slider_structer slider_dev;

char *text_tbl[]=
{
    "RGB BLOCK CONTROL",
    "RED:",
    "GREEN:",
    "BLUE:",
    "LED1  BRIGHTNESS CONTROL",
};
//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
    { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 90, 100, 400, 35, 0, 0x0, 0 },
    { SLIDER_CreateIndirect, "Slider", ID_SLIDER_1, 90, 165, 400, 35, 0, 0x0, 0 },
    { SLIDER_CreateIndirect, "Slider", ID_SLIDER_2, 90, 230, 400, 35, 0, 0x0, 0 },
    { SLIDER_CreateIndirect, "Slider", ID_SLIDER_3, 90, 360, 595, 32, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 500, 100, 55, 35, 0, 0x64, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 500, 165, 55, 35, 0, 0x64, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_2, 500, 230, 55, 35, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 311, 47, 202, 29, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 268, 314, 279, 31, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 31, 109, 51, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, -2, 175, 92, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_4, 12, 239, 80, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_5, 23, 366, 66, 20, 0, 0x64, 0 },
};

//���ڻص�����
//pMsg:��Ϣ
static void _cbBkWindow(WM_MESSAGE *pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLUE);
			GUI_Clear();
		default:
			WM_DefaultProc(pMsg);
	}
}
//������ɫ��
void DrawColorBlock(void)
{
    GUI_SetColor(slider_dev.slider_value[0]);
    GUI_FillRect(560,100,595,135);
    GUI_SetColor(slider_dev.slider_value[1]<<8);
    GUI_FillRect(560,165,595,200);
    GUI_SetColor((u32)slider_dev.slider_value[2]<<16);
    GUI_FillRect(560,230,595,265);
    
    GUI_SetColor(slider_dev.slider_value[0]|(((u32)slider_dev.slider_value[1])<<8)|(((u32)slider_dev.slider_value[2])<<16));
    GUI_FillRect(600,100,765,265);
    
}

//����SLIDER��ֵ����EDIT��ֵ
//hDlg�����ھ��
//Id:С����ID
void SetEditValue(WM_HWIN hDlg,int Id)
{
    WM_HWIN hSlider;
    WM_HWIN hEdit;
    u8 index;
    
    index=Id-ID_SLIDER_0;
    hSlider=WM_GetDialogItem(hDlg, ID_SLIDER_0+index);
    hEdit=WM_GetDialogItem(hDlg, ID_EDIT_0+index);
    slider_dev.slider_value[index]=SLIDER_GetValue(hSlider);
    EDIT_SetValue(hEdit,slider_dev.slider_value[index]);
    //���ʹ�ͻ��˴�����Ч
    WM_InvalidateWindow(WM_GetClientWindow(hDlg));
}

//�Ի���ص�����
//pMsg:��Ϣ
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    u8      i;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            DrawColorBlock();
            return;
        case WM_INIT_DIALOG:
            //��ʼ��FRAMEWIN
            hItem = pMsg->hWin;
            FRAMEWIN_SetText(hItem, "ALIENTEK SLIDER");
            FRAMEWIN_SetFont(hItem, GUI_FONT_24_ASCII);
            FRAMEWIN_SetTextColor(hItem, 0x0080FFFF);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

            //��ʼ��TEXT0
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetText(hItem, "RGB BLOCK CONTROL");
            TEXT_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            //��ʼ��TEXT1
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
            TEXT_SetText(hItem, "LED1  BRIGHTNESS CONTROL");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
            //��ʼ��TEXT2
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
            TEXT_SetText(hItem, "RED:");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            //��ʼ��TEXT3
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
            TEXT_SetText(hItem, "GREEN:");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            //��ʼ��TEXT4
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
            TEXT_SetText(hItem, "BLUE:");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);   
            //��ʼ��TEXT5
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
            TEXT_SetText(hItem, "LED:");
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            
            //��ʼ��EDIT0
            for(i=0;i<3;i++)
            {
                hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0+i);
                EDIT_SetDecMode(hItem,slider_dev.slider_value[i],0,255,0,0);
                EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
                EDIT_SetFont(hItem, GUI_FONT_24_ASCII);
            }   
            
            //��ʼ��SLIDER
            for(i=0;i<3;i++)
            {
                hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0+i);
                SLIDER_SetRange(hItem,0,255); 
				SLIDER_SetValue(hItem,slider_dev.slider_value[i]);//���û����λ��				
            }
            hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_3);
            SLIDER_SetRange(hItem,0,300);  
			SLIDER_SetValue(hItem,300);//���û��鵱ǰֵΪ300����ʱLED0����
            TIM_SetCompare3(TIM3,slider_dev.slider_value[3]);//����ռ�ձȣ�Ҳ��������LED0������
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) 
            {
                case ID_SLIDER_0:
                case ID_SLIDER_1:
                case ID_SLIDER_2:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            SetEditValue(pMsg->hWin,Id);
                            break;
                    }
                case ID_SLIDER_3:
                     switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem=WM_GetDialogItem(pMsg->hWin, ID_SLIDER_3);
                            slider_dev.slider_value[3]=SLIDER_GetValue(hItem); 
                            TIM_SetCompare3(TIM3,slider_dev.slider_value[3]);//����ռ�ձȣ�Ҳ��������LED0������                        			
                            break;
                    }
            }
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

//��ʾdemo����
void CreateSliderDemo(void)
{
    slider_dev.slider_value[0]=0;
    slider_dev.slider_value[1]=127;
    slider_dev.slider_value[2]=255;
	slider_dev.slider_value[3]=300;
    WM_SetCallback(WM_HBKWIN,_cbBkWindow);
    slider_dev.slider=GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

