#include "listviewdemo.h"
#include "DIALOG.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin LISTVIEW�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

student_info student;//ѧ����Ϣ
//�ؼ�ID
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00) 
#define ID_LISTVIEW_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_0 (GUI_ID_USER + 0x02)

//�û��Զ�����Ϣ
#define WM_DELETEWIN   (WM_USER +0)

int listviewitem=-1;    //LISTVIEWѡ�е���Ŀ���

//LISTVIEW�б�
static const char *ListViewTable[][4]={
	{"Brooks",	"20141226001","97","A"},
	{"Aaron",	"20141226002","84","B"},
	{"Faith",	"20141226003","73","C"},
	{"Mabel",	"20141226004","85","B"},
	{"Sabina",	"20141226005","45","E"},
	{"Alex",	"20141226006","98","A"},
	{"Dana",	"20141226007","84","B"},
	{"Wally",	"20141226008","65","D"},
};

//��Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240,FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 45, 15, 220, 140, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 100, 160, 100, 40, 0, 0x0, 0 },
};

//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    WM_MESSAGE Msg;
    u8 i=0;
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            //��ʼ��FRAMWIN
            hItem = pMsg->hWin;
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            FRAMEWIN_SetFont(hItem, GUI_FONT_20B_ASCII);
            FRAMEWIN_SetTextColor(hItem,GUI_YELLOW);
            FRAMEWIN_SetText(hItem, "ALIENTEK  LISTVIEW");

            //��ʼ��LISTVIEW
            hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0);
            LISTVIEW_SetGridVis(hItem,1);               //�����߿ɼ�
            LISTVIEW_SetAutoScrollH(hItem,1);           //�����Զ�ˮƽ������
            LISTVIEW_SetAutoScrollV(hItem,1);           //�����Զ���ֱ������
            LISTVIEW_SetHeaderHeight(hItem,20);         //���ñ���ĸ߶�
            LISTVIEW_SetRowHeight(hItem,15);            //����LISTVIEW���и�
            LISTVIEW_SetBkColor(hItem,0,0X00C080FF);    //���ñ�����ɫ
            //��LISTVIEW�����
            LISTVIEW_AddColumn(hItem,50, "Name", GUI_TA_HCENTER | GUI_TA_VCENTER); 
            LISTVIEW_AddColumn(hItem,80, "Num", GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,40, "Score", GUI_TA_HCENTER | GUI_TA_VCENTER);
            LISTVIEW_AddColumn(hItem,35, "Grade", GUI_TA_HCENTER | GUI_TA_VCENTER);
            //��LISTVIEW�����
            for(i=0;i<GUI_COUNTOF(ListViewTable);i++)
			{
				LISTVIEW_AddRow(hItem,ListViewTable[i]);
			}

            //��ʼ��BUTTON
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
            BUTTON_SetText(hItem, "OK");
            break;
        case WM_NOTIFY_PARENT:
            Id=WM_GetId(pMsg->hWinSrc);
            NCode=pMsg->Data.v;
            switch(Id) 
            {
                case ID_LISTVIEW_0: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0);
                            listviewitem=LISTVIEW_GetSel(hItem);//��ȡѡ�е���Ŀ���
                            //����ָ����Ԫ����ı�
                            LISTVIEW_GetItemText(hItem,0,listviewitem,(char*)student.name,20);  //��ȡ����  
                            LISTVIEW_GetItemText(hItem,1,listviewitem,(char*)student.num,20);   //��ȡѧ��  
                            LISTVIEW_GetItemText(hItem,2,listviewitem,(char*)(student.score),4);  //��ȡ����   
                            LISTVIEW_GetItemText(hItem,3,listviewitem,(char*)&(student.grade),4);  //��ȡ�ȼ�                           
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
                            Msg.MsgId=WM_DELETEWIN;
							Msg.hWinSrc=pMsg->hWin;
							WM_SendMessage(WM_HBKWIN,&Msg);//������Ϣ
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
            GUI_DispStringAt("ALIENTEK LISTVIEW DEMO",0,0);
            GUI_SetColor(GUI_BLACK);
            GUI_SetFont(&GUI_Font20_ASCII);
            GUI_DispStringAt(" Name:",0,30);
            GUI_DispStringAt("  Num:",0,50);
            GUI_DispStringAt("Score:",0,70);
            GUI_DispStringAt("Grade:",0,90);
        
            GUI_SetColor(GUI_RED);
            GUI_DispStringAt((char*)student.name,65,30);    //��ʾѡ�е�ѧ������
            GUI_DispStringAt((char*)student.num,65,50);     //��ʾѡ�е�ѧ��ѧ��
            GUI_DispStringAt((char*)student.score,65,70);   //��ʾѡ�е�ѧ������
            GUI_DispStringAt((char*)&(student.grade),65,90);//��ʾѡ�е�ѧ���ĵȼ�
			break;
        case WM_DELETEWIN:                      //ɾ������
            hWin=pMsg->hWinSrc;
            WM_DeleteWindow(hWin);
            WM_InvalidateWindow(WM_HBKWIN);     //����������Ч
            WM_CreateTimer(WM_HBKWIN,0,3000,0); //������ʱ��
            break;
        case WM_TIMER:
            GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);//�����Ի���
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//�����Ի���
void CreateListviewdemo(void) 
{
    WM_SetCallback(WM_HBKWIN,_cbBkWindow);//���ñ������ڻص�����
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}
