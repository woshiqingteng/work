#include "multieditdemo.h"
#include "DIALOG.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin MULTIEDIT�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//�ؼ�ID
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_MULTIEDIT_0 (GUI_ID_USER + 0x01)
#define ID_EDIT_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_0 (GUI_ID_USER + 0x03)
#define ID_TEXT_0 (GUI_ID_USER + 0x04)
#define ID_TEXT_1 (GUI_ID_USER + 0x05)

//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240,FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_0, 20, 35, 130, 150, 0, 0x0, 0 },
    { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 170, 61, 133, 26, 0, 0x64, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 188, 129, 92, 46, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_0, 30, 5, 107, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 198, 5, 80, 20, 0, 0x64, 0 },
};

//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    u8 databuff[50];
    int datalen=0;
    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_DrawGradientV(162,10,165,200,GUI_RED,GUI_BLUE); //���ƴ�ֱ�ݶ�������
            GUI_SetColor(GUI_RED);          //����Ϊ��ɫ
            GUI_SetFont(&GUI_Font8x18);     //��������
            break;      
        case WM_INIT_DIALOG:
            
            //FRAMWIN��ʼ��
            hItem = pMsg->hWin;
            FRAMEWIN_SetFont(hItem,GUI_FONT_20_ASCII);
            FRAMEWIN_SetTextColor(hItem,0x0000FFFF);
            FRAMEWIN_SetTextAlign(hItem,GUI_TA_HCENTER |GUI_TA_VCENTER);
            FRAMEWIN_SetText(hItem,"ALIENTEK MULTIEDIT");

            //MULTIEDIT��ʼ��
            hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
            MULTIEDIT_SetFont(hItem, GUI_FONT_8X18);
            MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_EDIT, 0x00FF0000);
            MULTIEDIT_SetAutoScrollH(hItem,1);      //�����Զ�ˮƽ������
            MULTIEDIT_SetAutoScrollV(hItem,1);      //�����Զ���ֱ������ 
            MULTIEDIT_EnableBlink(hItem,500,1);     //���������˸
        
            //EDIT��ʼ��
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
            EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
            EDIT_SetFont(hItem, GUI_FONT_16B_ASCII);
            EDIT_SetText(hItem, "www.openedv.com");
            EDIT_EnableBlink(hItem,500,1);          //���������˸

            //BUTTON��ʼ��
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
            BUTTON_SetText(hItem, "SEND");
            
            //TEXT��ʼ��
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
            TEXT_SetTextColor(hItem, 0x000000FF);
            TEXT_SetText(hItem, "Receive Area");

            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
            TEXT_SetTextColor(hItem, 0x000000FF);
            TEXT_SetText(hItem, "Send Area");
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
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
                case ID_EDIT_0: 
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
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_0);
                            datalen=EDIT_GetNumChars(hItem)+1;  //��ȡEDIT�е��ı����ȣ�ע��Ҫ��һ���ֽڵĽ�����'\0'
                            EDIT_GetText(hItem,(char*)databuff,datalen); //��ȡEDIT�е��ı�
                            printf("%s\r\n",databuff);          //ͨ�����ڷ��ͳ�ȥ
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
            GUI_DispStringAt("ALIENTEK MULIEDT DEMO",0,0);
		default:
			WM_DefaultProc(pMsg);
	}
}

//�����Ի���
void CreateMultieditDemo(void) 
{
    WM_HWIN hWin;
    WM_HWIN hItem;
    u8 len;
    WM_SetCallback(WM_HBKWIN,_cbBkWindow);      //���ñ������ڻص�����  
    hWin=GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN,0,0);
    hItem=WM_GetDialogItem(hWin,ID_MULTIEDIT_0);//��ȡMULTIEDIT�ؼ����
    while(1)
    {
        if(USART_RX_STA&0x8000)             //�յ�����
		{					   
			len=USART_RX_STA&0x3fff;        //�õ��˴ν��յ������ݳ���
            USART_RX_BUF[len]='\r';         //��ӻ��з�
            USART_RX_BUF[len+1]='\n';
            MULTIEDIT_AddText(hItem,(char*)USART_RX_BUF);  //��MULTIEDIT����ʾ����
			USART_RX_STA=0;
		}else
        GUI_Delay(100);
    }
}
