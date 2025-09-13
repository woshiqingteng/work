#include "dropdowndemo.h"
#include "DIALOG.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemWin DROPDOWN�ؼ�ʹ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/19
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//�ؼ�ID
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x01)
#define ID_DROPDOWN_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_0   (GUI_ID_USER + 0x03)

#define WM_APP_SHOW_TEXT	(WM_USER +0)    //�û��Զ�����Ϣ
#define TEXT_MAXLEN	40

WM_HWIN DialoghWin;             //�Ի��򴰿�
char TextBuffer[20]={"China"};  //�����ѡ�е�����,Ĭ��ֵΪ"China"

//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 75, 20, 150, 120, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,"Ok", ID_BUTTON_0,100, 170,  100,	40},
};

//�������ڵĻص�����
static void _BkCallback(WM_MESSAGE *pMsg)
{
	static WM_HWIN hWin;
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_DARKGRAY);
			GUI_Clear();
			GUI_SetFont(&GUI_Font24_ASCII);
			GUI_DispStringAt("ALIENTEK DROPDOWNDEMO", 20, 5);
			GUI_SetFont(&GUI_Font20B_ASCII);
            GUI_DispStringAt("Your Select is:", 20, 30);
            GUI_SetColor(GUI_RED);
            GUI_DispStringAt(TextBuffer,155,30);//��ʾ��ѡ�е�����
			break;	
		case WM_APP_SHOW_TEXT:
			hWin=pMsg->hWinSrc;
			WM_HideWindow(hWin);	//���ضԻ���
			WM_InvalidateWindow(WM_HBKWIN); //����������Ч
			WM_CreateTimer(WM_HBKWIN,0,3000,0);
			break;
		case WM_TIMER:
			WM_ShowWindow(hWin);    //��ʾ�Ի���
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}	
}

//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;
    WM_MESSAGE Msg;
    int value;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			hItem = pMsg->hWin;
			//��ʼ��FRAMEWIN
			FRAMEWIN_SetText(hItem, "ALIENTEK DROPDOWNDEMO");
            FRAMEWIN_SetTextColor(hItem,GUI_YELLOW);
			FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        
			//��ʼ��DROPDOWN
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
			DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
			DROPDOWN_SetAutoScroll(hItem,1);	//�����Զ�ʹ�ù�����
			DROPDOWN_AddString(hItem, "China");
			DROPDOWN_AddString(hItem, "Cambodia");
			DROPDOWN_AddString(hItem, "Japan");
			DROPDOWN_AddString(hItem, "Australien");
			DROPDOWN_AddString(hItem, "Canada");
        
            //��ʼ��BUTTON
            hItem=WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetFont(hItem,&GUI_FontComic18B_ASCII);
            BUTTON_SetText(hItem,"OK");
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_DROPDOWN_0: //DROPDOWN֪ͨ����
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                            hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_0);
                            value=DROPDOWN_GetSel(hItem);
                            DROPDOWN_GetItemText(hItem,value,TextBuffer,20);//��ȡѡ�е���Ŀ���ı�
							break;
					}
					break;
                case ID_BUTTON_0:
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						
						case WM_NOTIFICATION_RELEASED:
							Msg.MsgId=WM_APP_SHOW_TEXT;
							Msg.hWinSrc=pMsg->hWin;
							WM_SendMessage(WM_HBKWIN,&Msg);
							break;
		
					}    
			}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

//�����Ի���
void dropdown_demo(void) 
{
    WM_SetCallback(WM_HBKWIN,_BkCallback);
	DialoghWin=GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	while(1)
	{
		GUI_Delay(100);
	}
}
