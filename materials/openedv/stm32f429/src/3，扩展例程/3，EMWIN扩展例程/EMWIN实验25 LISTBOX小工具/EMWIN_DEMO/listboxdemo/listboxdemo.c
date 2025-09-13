#include "listboxdemo.h"
#include "DIALOG.h"
#include "pcf8574.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin LISTBOX�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//�ؼ�ID
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
#define ID_LISTBOX_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_0       (GUI_ID_USER + 0x02)
#define ID_BUTTON_0     (GUI_ID_USER + 0x04)
#define ID_BUTTON_1     (GUI_ID_USER + 0x05)

//�û��Զ�����Ϣ
#define WM_DELETEWIN   (WM_USER +0)

u8 led1_switch=0;       //LED1���أ�0���ر�;1����
u8 beep_switch=0;       //���������أ�0���ر�;1����
int listboxitem=-1;     //LISTBOX��ѡ�е���Ŀ���

//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { LISTBOX_CreateIndirect, "Listbox", ID_LISTBOX_0, 21, 20, 115, 128, 0, 0x0, 0 },
    { TEXT_CreateIndirect,  "Text", ID_TEXT_0, 170, 70, 133, 35, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 30, 160, 90, 40, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 175, 160, 90, 40, 0, 0x0, 0 },
};

//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    static char textbuf[20];
    WM_MESSAGE Msg;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            //FRAMWIN�ؼ���ʼ��
            hItem = pMsg->hWin;
            FRAMEWIN_SetFont(hItem, GUI_FONT_20B_ASCII);
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            FRAMEWIN_SetText(hItem, "ALIENTEK LISTBOX");
            FRAMEWIN_SetTextColor(hItem,GUI_YELLOW);
            
            //LISTBOX�ؼ���ʼ��
            hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
            LISTBOX_SetFont(hItem, GUI_FONT_20_ASCII);
            LISTBOX_SetAutoScrollH(hItem,1);               //�����Զ�ʹ��ˮƽ������
            LISTBOX_SetAutoScrollV(hItem,1);               //�����Զ�ʹ�ô�ֱ������
            LISTBOX_AddString(hItem, "LED1 ON");
            LISTBOX_AddString(hItem, "LED1 OFF");
            LISTBOX_AddString(hItem, "BEEP ON");
            LISTBOX_AddString(hItem, "BEEP OFF");
            LISTBOX_AddString(hItem, "ALL ON");
            LISTBOX_AddString(hItem, "ALL OFF");

            //CHECKBOX�ؼ���ʼ��
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetTextColor(hItem,GUI_RED);
            TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
            TEXT_SetText(hItem,"NO SELECT!");

            //BUTTON�ؼ���ʼ��
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetText(hItem, "OK");
            BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
            BUTTON_SetText(hItem, "CANCEL");
            break;
        case WM_NOTIFY_PARENT:
            Id=WM_GetId(pMsg->hWinSrc);
            NCode=pMsg->Data.v;
            switch(Id) 
            {
                case ID_LISTBOX_0: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTBOX_0);
                            listboxitem=LISTBOX_GetSel(hItem);                  //��ȡѡ�е���Ŀ���
                            LISTBOX_GetItemText(hItem,listboxitem,textbuf,10);//��ȡѡ�е���Ŀ������
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            break;
                    }
                    break;
                case ID_BUTTON_0: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            switch(listboxitem)
                            {
                                case 0:
                                    LED1=0;                         //LED1��
                                    break;
                                case 1:
                                    LED1=1;                         //LED1�ر�
                                    break;
                                case 2:
                                    PCF8574_WriteBit(BEEP_IO,0);    //��������
                                    break;  
                                case 3:
                                    PCF8574_WriteBit(BEEP_IO,1);    //�������ر�
                                    break;
                                case 4:
                                    LED1=0;                         //LED1��
                                    PCF8574_WriteBit(BEEP_IO,0);    //��������
                                    break;
                                case 5:
                                    LED1=1;                         //LED1�ر�
                                    PCF8574_WriteBit(BEEP_IO,1);    //�������ر�
                                    break;  
                            }  
                            if(listboxitem>=0)
                            {
                                hItem=WM_GetDialogItem(pMsg->hWin,ID_TEXT_0);//��TEXT�ؼ�����ʾѡ�е�����
                                TEXT_SetText(hItem,textbuf);
                            }
                            break;
                    }
                    break;
                case ID_BUTTON_1: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Msg.MsgId=WM_DELETEWIN;
							Msg.hWinSrc=pMsg->hWin;
							WM_SendMessage(WM_HBKWIN,&Msg);
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

//��������WM_HBKWIN�ص�����
static void _cbBkWindow(WM_MESSAGE* pMsg)
{
    static WM_HWIN hWin;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLUE);
			GUI_Clear();
            GUI_SetColor(GUI_YELLOW);
            GUI_SetFont(&GUI_Font24B_ASCII);
            GUI_DispStringAt("ALIENTEK LISTBOX DEMO",0,0);
			break;
        case WM_DELETEWIN:                     //ɾ������
            hWin=pMsg->hWinSrc;
            WM_DeleteWindow(hWin);
            WM_InvalidateWindow(WM_HBKWIN);     //����������Ч
            WM_CreateTimer(WM_HBKWIN,0,3000,0); //������ʱ��
            break;
        case WM_TIMER:
            GUI_CreateDialogBox(_aDialogCreate,GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN,0,0);//�����Ի���
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//�����Ի���
void CreateListboxdemo(void) 
{
    WM_SetCallback(WM_HBKWIN,_cbBkWindow);//���ñ������ڻص�����
    GUI_CreateDialogBox(_aDialogCreate,GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN,0,0);
}
