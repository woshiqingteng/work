#include "hzfontdemo.h"
#include "EmWinHZFont.h"
#include "DIALOG.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin �����ֿ⺺����ʾ    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


#define ID_FRAMEWIN_0 	(GUI_ID_USER + 0)
#define ID_MULTIEDIT_0 	(GUI_ID_USER + 1)
#define ID_BUTTON_0 	(GUI_ID_USER + 2)
#define ID_BUTTON_1 	(GUI_ID_USER + 3)
#define ID_CHECKBOX_0 	(GUI_ID_USER + 4)
#define ID_CHECKBOX_1 	(GUI_ID_USER + 5)
#define ID_CHECKBOX_2 	(GUI_ID_USER + 6)
#define ID_CHECKBOX_3 	(GUI_ID_USER + 7)
#define ID_RADIO_0 		(GUI_ID_USER + 8)
#define ID_LISTVIEW_0 	(GUI_ID_USER + 9)
#define ID_DROPDOWN_0 	(GUI_ID_USER + 10)
#define ID_TEXT_0 		(GUI_ID_USER + 11)
#define ID_TEXT_1 		(GUI_ID_USER + 12)
#define ID_TEXT_2 		(GUI_ID_USER + 13)
#define ID_TEXT_3 		(GUI_ID_USER + 14)
#define ID_SCROLLBAR_0 	(GUI_ID_USER + 15)
#define ID_SLIDER_0 	(GUI_ID_USER + 16)

WM_HWIN hDialog;

static const char *_ListViewTable[][4]={
	{"����",	"20141226001","97","A"},
	{"����",	"20141226002","84","B"},
	{"����",	"20141226003","73","C"},
	{"������",	"20141226004","85","B"},
	{"����",	"20141226005","45","E"},
	{"ŷ������",	"20141226006","98","A"},
	{"������",	"20141226007","84","B"},
	{"��Ʒ�",	"20141226008","65","D"},
};

char *MultiEdit_String	=  
"STM32F4/F7�����޿�����\r\n\
CPU:STM32F429/F746\r\n\
FLASH:1M\r\n\
RAM	 :240/256K\r\n\
����:10M/100M��̫��PHYоƬ\r\n\
     WM8978��ƵDAC	";
	
//�Ի�����Դ�б�
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, 	"Framewin", 	ID_FRAMEWIN_0, 	0, 		0, 		800, 	480, 	FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
  { MULTIEDIT_CreateIndirect, 	"Multiedit", 	ID_MULTIEDIT_0, 20, 	20, 	300, 	160, 	0, 0x0, 0 },
  { BUTTON_CreateIndirect, 		"BUTTON", 		ID_BUTTON_0, 	398,	369, 	140, 	50, 	0, 0x0, 0 },
  { BUTTON_CreateIndirect, 		"BUTTON", 		ID_BUTTON_1, 	599, 	364, 	140, 	50, 	0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, 	"Checkbox", 	ID_CHECKBOX_0, 	530, 	90, 	120, 	30, 	0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, 	"Checkbox",		ID_CHECKBOX_1, 	530, 	130, 	120, 	30, 	0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, 	"Checkbox", 	ID_CHECKBOX_2, 	530, 	170, 	120, 	30, 	0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, 	"Checkbox", 	ID_CHECKBOX_3, 	530, 	210, 	120, 	30, 	0, 0x0, 0 },
  { RADIO_CreateIndirect, 		"Radio", 		ID_RADIO_0, 	650, 	119, 	90, 	90, 	0, 0x1404,0 },
  { LISTVIEW_CreateIndirect, 	"Listview", 	ID_LISTVIEW_0, 	20, 	200, 	300, 	194, 	0, 0x0, 0 },
  { DROPDOWN_CreateIndirect, 	"Dropdown", 	ID_DROPDOWN_0, 	324, 	22, 	150, 	100, 	0, 0x0, 0 },
  { TEXT_CreateIndirect, 		"Text0", 		ID_TEXT_0, 		327, 	180, 	150, 	20, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 		"Text1",  		ID_TEXT_1, 		327, 	200, 	150, 	20, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 		"Text2", 		ID_TEXT_2, 		327, 	220, 	200, 	30, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 		"Text3", 		ID_TEXT_3, 		327, 	250, 	300, 	50, 	0, 0x64, 0 },
  { SCROLLBAR_CreateIndirect, 	"Scrollbar", 	ID_SCROLLBAR_0, 482, 	310, 	260, 	40, 	0, 0x0, 0 },
  { SLIDER_CreateIndirect, 		"Slider", 		ID_SLIDER_0, 	513, 	18, 	259, 	38, 	0, 0x0, 0 },
};


//�Ի��򴰿ڻص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	WM_HWIN hHeader;
	int     NCode;
	int     Id;
	int 	i;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			
			//��ʼ��FRAMEWIN
			hItem = pMsg->hWin;
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			FRAMEWIN_SetTitleHeight(hItem, 30);
			FRAMEWIN_SetFont(hItem,&GUI_FontHZ16);
			FRAMEWIN_SetTextColor(hItem, GUI_RED);
			FRAMEWIN_SetText(hItem, "ALIENTEK �����ֿ⺺����ʾ");
		
			//��ʼ��MULTIEDIT
			hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
			MULTIEDIT_SetTextColor(hItem,MULTIEDIT_CI_EDIT,GUI_BLACK);
			MULTIEDIT_SetBkColor(hItem,MULTIEDIT_CI_EDIT,GUI_LIGHTCYAN);
			MULTIEDIT_SetFont(hItem,&GUI_FontHZ16);
			MULTIEDIT_SetText(hItem,MultiEdit_String);
            MULTIEDIT_EnableBlink(hItem,500,1);
		
			//��ʼ��BUTTON
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
			BUTTON_SetFont(hItem,&GUI_FontHZ16);
			BUTTON_SetText(hItem,"ȷ��");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetFont(hItem,&GUI_FontHZ16);
			BUTTON_SetText(hItem,"ȡ��");
		
			//��ʼ��CHECKBOX
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
			CHECKBOX_SetFont(hItem,&GUI_FontHZ16);
			CHECKBOX_SetText(hItem, "��ѡ�� 1");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
			CHECKBOX_SetFont(hItem,&GUI_FontHZ16);
			CHECKBOX_SetText(hItem, "��ѡ�� 2");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_2);
			CHECKBOX_SetFont(hItem,&GUI_FontHZ16);
			CHECKBOX_SetText(hItem, "��ѡ�� 3");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_3);
			CHECKBOX_SetFont(hItem,&GUI_FontHZ16);
			CHECKBOX_SetText(hItem, "��ѡ�� 4");
		
			//��ʼ��RADIO
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
			RADIO_SetFont(hItem,&GUI_Font13HB_ASCII);
            RADIO_SetText(hItem, "Option 1", 0);
			RADIO_SetText(hItem, "Option 2", 1);
			RADIO_SetText(hItem, "Option 3", 2);
			RADIO_SetText(hItem, "Option 4", 3);
  
			//��ʼ��LISTVIEW
			hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
			hHeader = LISTVIEW_GetHeader(hItem);
			LISTVIEW_SetHeaderHeight(hItem, 30);
			HEADER_SetFont(hHeader,&GUI_FontHZ16);
			LISTVIEW_AddColumn(hItem, 40, "����", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(hItem, 40, "ѧ��", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(hItem, 40, "�ɼ�", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_AddColumn(hItem, 40, "�ȼ�", GUI_TA_HCENTER | GUI_TA_VCENTER);
			LISTVIEW_SetGridVis(hItem, 1);
			LISTVIEW_SetAutoScrollH(hItem, 1);
			LISTVIEW_SetAutoScrollV(hItem, 1);
			LISTVIEW_SetRowHeight(hItem, 20);
			LISTVIEW_SetFont(hItem,&GUI_FontHZ16);
			LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_ORANGE);
			for(i=0;i<GUI_COUNTOF(_ListViewTable);i++)
			{
				LISTVIEW_AddRow(hItem,_ListViewTable[i]);
			}
		

			//��ʼ��DROPDOWN
			hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
			DROPDOWN_SetAutoScroll(hItem,1);//�����Զ�������
			DROPDOWN_SetFont(hItem,&GUI_FontHZ16);
			DROPDOWN_AddString(hItem, "����");
			DROPDOWN_AddString(hItem, "Ӣ��");
			DROPDOWN_AddString(hItem, "����");
			DROPDOWN_AddString(hItem, "����������");
			DROPDOWN_AddString(hItem, "��������");
			DROPDOWN_AddString(hItem, "��������......");
 
			//��ʼ��TEXT
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetFont(hItem,&GUI_FontHZ12);
			TEXT_SetTextColor(hItem,GUI_YELLOW);
			TEXT_SetText(hItem, "�����������");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			TEXT_SetFont(hItem,&GUI_FontHZ16);
			TEXT_SetTextColor(hItem,GUI_MAGENTA);
			TEXT_SetText(hItem, "�����������");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
			TEXT_SetFont(hItem,&GUI_FontHZ24);
			TEXT_SetTextColor(hItem,GUI_TRANSPARENT);
			TEXT_SetText(hItem, "�����������");
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
			TEXT_SetFont(hItem,&GUI_FontHZ32);
			TEXT_SetTextColor(hItem,GUI_BLUE);
			TEXT_SetText(hItem, "�V���������");
			break;
		case WM_NOTIFY_PARENT:
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) 
			{
				case ID_MULTIEDIT_0: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
						
							break;
						case WM_NOTIFICATION_RELEASED:
							
							break;
						case WM_NOTIFICATION_VALUE_CHANGED:
							
						break;
					}
					break;
				case ID_BUTTON_0: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
						
							break;
					}
					break;
				case ID_BUTTON_1:
					switch(NCode) 
					{
						case WM_NOTIFICATION_RELEASED:
							
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

void HZFontDemo(void) 
{
	GUI_CURSOR_Show();
	#if GUI_SUPPORT_MEMDEV
		WM_SetCreateFlags(WM_CF_MEMDEV);
	#endif
	
    WM_SetCallback(WM_HBKWIN,_cbBkWindow);
	hDialog = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	GUI_Exec();
	while(1)
	{
		GUI_Delay(100);
	}
}
