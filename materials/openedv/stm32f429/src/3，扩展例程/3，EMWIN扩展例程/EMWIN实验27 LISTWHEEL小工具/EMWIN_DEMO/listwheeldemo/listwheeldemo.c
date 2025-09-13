#include "listwheeldemo.h"
#include "DIALOG.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin LISTWHEEL�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/24
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

DateStruct datedemo={"1990","January","01"};    //����,Ĭ�ϳ�ʼ��Ϊ1990��1��1��

u8 listwheelindex[3];   //LISTWHEELѡ�е���Ŀ��
//��
const char *_apYear[]= 
{
    "1990", "1991", "1992", "1993", "1994", "1995", "1996", 
    "1997", "1998", "1999", "2000", "2001", "2002", "2003", 
    "2004", "2005", "2006", "2007", "2008", "2009", "2010", 
    "2011", "2012", "2013", "2014", "2015", "2016", "2017", 
    "2018", "2019", "2020", "2021", "2022", "2023","2024",
};

//��
const char *_apMonth[]=
{
    "January",      "February",     "March",    "April", 
    "May",          "June",         "July",     "August", 
    "September",    "October",      "November", "December",
};

//��
const char *_apDay[]=
{
    "01", "02", "03", "04", "05", "06", 
    "07", "08", "09", "10", "11", "12", 
    "13", "14", "15", "16", "17", "18", 
    "19", "20", "21", "22", "23", "24", 
    "25", "26", "27", "28", "29", "30", "31",
};

//�ؼ�ID
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x02)
#define ID_TEXT_0 (GUI_ID_USER + 0x03)
#define ID_TEXT_1 (GUI_ID_USER + 0x04)
#define ID_TEXT_2 (GUI_ID_USER + 0x05)
#define ID_LISTWHEEL_0 (GUI_ID_USER + 0x06)
#define ID_LISTWHEEL_1 (GUI_ID_USER + 0x07)
#define ID_LISTWHEEL_2 (GUI_ID_USER + 0x08)

//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240,FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 192, 150, 100, 40, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 190, 40, 51, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 178, 60, 66, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 196, 80, 53, 20, 0, 0x64, 0 },
    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_0, 10, 30, 50, 150, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_1, 60, 30, 80, 150, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_2, 140, 30, 30, 150, 0, 0x0, 0 },
};

//��LISTWHEEL0�ϻ��ƺ�ɫָʾ��
static int ListWheel0_OwnerDraw(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo)
{
    switch(pDrawItemInfo->Cmd)
    {
        case WIDGET_ITEM_DRAW_OVERLAY:
            GUI_SetColor(GUI_RED);
            GUI_DrawHLine(60,0,50);
            GUI_DrawHLine(90,0,50);
            break;
        default:
            return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    }
    return 0;
}

//��LISTWHEEL1�ϻ��ƺ�ɫָʾ��
static int ListWheel1_OwnerDraw(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo)
{
    switch(pDrawItemInfo->Cmd)
    {
        case WIDGET_ITEM_DRAW_OVERLAY:
            GUI_SetColor(GUI_RED);
            GUI_DrawHLine(60,0,80);
            GUI_DrawHLine(90,0,80);
            break;
        default:
            return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    }
    return 0;
}

//��LISTWHEEL2�ϻ��ƺ�ɫָʾ��
static int ListWheel2_OwnerDraw(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo)
{
    switch(pDrawItemInfo->Cmd)
    {
        case WIDGET_ITEM_DRAW_OVERLAY:
            GUI_SetColor(GUI_RED);
            GUI_DrawHLine(60,0,30);
            GUI_DrawHLine(90,0,30);
            break;
        default:
            return LISTWHEEL_OwnerDraw(pDrawItemInfo);
    }
    return 0;
}
//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    int     i=0;
    static u8 LineHeight=30;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_DrawGradientV(172,10,175,200,GUI_RED,GUI_BLUE);            //���ƴ�ֱ�ݶ�������
            GUI_SetColor(GUI_RED);          //����Ϊ��ɫ
            GUI_SetFont(&GUI_Font8x18);     //��������
            GUI_DispStringAt(datedemo.year,230,40); //��ʾ��
            GUI_DispStringAt(datedemo.month,230,60);//��ʾ��
            GUI_DispStringAt(datedemo.day,230,80);  //��ʾ��
            break;
        case WM_INIT_DIALOG:
            //��ʼ��FRAMEWIN
            hItem = pMsg->hWin;
            FRAMEWIN_SetFont(hItem, GUI_FONT_20_ASCII);
            FRAMEWIN_SetText(hItem, "ALIENTEK LISTWHEEL");
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            FRAMEWIN_SetTextColor(hItem,GUI_YELLOW);

            //��ʼ��LISTWHEEL
            for(i=0;i<3;i++)
            {   
                hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0+i);
                LISTWHEEL_SetFont(hItem,&GUI_FontComic18B_ASCII);           //��������
                LISTWHEEL_SetTextAlign(hItem,GUI_TA_VCENTER|GUI_TA_HCENTER);//���뷽ʽ
                LISTWHEEL_SetSnapPosition(hItem,(150-LineHeight)/2);        //����������λ��
                LISTWHEEL_SetLineHeight(hItem,LineHeight);                  //���û�����������ʹ�õ��и�
                LISTWHEEL_SetTextColor(hItem,LISTWHEEL_CI_SEL,0xEE8822);    //����ѡ�е���Ŀ���ı���ɫ
            }
            //����Year��LISTWHEEL
            hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
            LISTWHEEL_SetOwnerDraw(hItem,ListWheel0_OwnerDraw);         //���ƺ�ɫָʾ��
            for(i=0;i<GUI_COUNTOF(_apYear);i++) //���������
            {
                LISTWHEEL_AddString(hItem,*(_apYear+i));
            }
            
            //����Month��LISTWHEEL
            hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_1);
            LISTWHEEL_SetOwnerDraw(hItem,ListWheel1_OwnerDraw);         //���ƺ�ɫָʾ��
            for(i=0;i<GUI_COUNTOF(_apMonth);i++) //���������
            {
                LISTWHEEL_AddString(hItem,*(_apMonth+i));
            }
            
            //����Day��LISTWHEEL
            hItem=WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_2);
            LISTWHEEL_SetOwnerDraw(hItem,ListWheel2_OwnerDraw);         //���ƺ�ɫָʾ��
            for(i=0;i<GUI_COUNTOF(_apDay);i++) //���������
            {
                LISTWHEEL_AddString(hItem,*(_apDay+i));
            }

            //��ʼ��BUTTON
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_1);
            BUTTON_SetText(hItem, "ok");

            //��ʼ��TEXT
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetFont(hItem, &GUI_Font8x18);
            TEXT_SetText(hItem, "Year:");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
            TEXT_SetFont(hItem, &GUI_Font8x18);
            TEXT_SetText(hItem, "Month:");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
            TEXT_SetFont(hItem, &GUI_Font8x18);
            TEXT_SetText(hItem, "Day:");
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
                            WM_InvalidateWindow(pMsg->hWin);     //FRAMEWIN������Ч
                            break;
                    }
                    break;
                case ID_LISTWHEEL_0: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_0);
                            listwheelindex[0]=LISTWHEEL_GetPos(hItem);      //��ȡ��ǰ��ѡ�����������
                            //ע�⣺LISTWHEEL_GetPos()ֻ�ǻ�ȡ������ͣ������ѡ�е��Ǹ���Ŀ��ţ����Ǵ�ʱ��û��ѡ�����
                            //��Ŀ���������LISTWHEEL_SetSel()��ѡ�������Ŀ��
                            LISTWHEEL_SetSel(hItem,listwheelindex[0]);      //���õ�ǰ��ѡ�����
                            LISTWHEEL_GetItemText(hItem,listwheelindex[0],datedemo.year,5);//��ȡ��ѡ�е���Ŀ���ı�
                            break;
                    }
                    break;
                case ID_LISTWHEEL_1: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_1);
                            listwheelindex[1]=LISTWHEEL_GetPos(hItem);  //��ȡ��ǰ��ѡ�����������
                            LISTWHEEL_SetSel(hItem,listwheelindex[1]);  //���õ�ǰ��ѡ������
                            LISTWHEEL_GetItemText(hItem,listwheelindex[1],datedemo.month,20);//��ȡ��ѡ�е���Ŀ���ı�
                            break;
                    }
                    break;
                case ID_LISTWHEEL_2: 
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_SEL_CHANGED:
                            hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTWHEEL_2);
                            listwheelindex[2]=LISTWHEEL_GetPos(hItem);  //��ȡ��ǰ��ѡ�����������
                            LISTWHEEL_SetSel(hItem,listwheelindex[2]);  //���õ�ǰ��ѡ������
                            LISTWHEEL_GetItemText(hItem,listwheelindex[2],datedemo.day,5);//��ȡ��ѡ�е���Ŀ���ı�
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
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLUE);
			GUI_Clear();
            GUI_SetColor(GUI_YELLOW);
            GUI_SetFont(&GUI_Font24B_ASCII);
            GUI_DispStringAt("ALIENTEK LISTWHEEL DEMO",0,0);
		default:
			WM_DefaultProc(pMsg);
	}
}

//�����Ի���
void CreateListWheelDemo(void) 
{
    WM_SetCallback(WM_HBKWIN,_cbBkWindow);//���ñ������ڻص�����
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

