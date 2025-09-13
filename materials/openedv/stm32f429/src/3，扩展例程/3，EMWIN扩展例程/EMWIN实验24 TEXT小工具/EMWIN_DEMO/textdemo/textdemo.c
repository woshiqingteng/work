#include "textdemo.h"
#include "DIALOG.h"
#include "adc.h"
#include "rtc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin TEXT�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//�ؼ�ID
#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_TEXT_0 (GUI_ID_USER + 0x01)
#define ID_TEXT_1 (GUI_ID_USER + 0x02)
#define ID_TEXT_2 (GUI_ID_USER + 0x03)


WM_HWIN TextDialoghWin;     //�ı��Ի��򴰿�

//�Ի���ͻᣬ��Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]=
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240,FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { TEXT_CreateIndirect, "AD VALUE:", ID_TEXT_0, 20, 50, 120, 25, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "DATE:",     ID_TEXT_1, 68, 80, 69, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "TIME:",     ID_TEXT_2, 75, 110, 65, 20, 0, 0x64, 0 },
};

//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    static u16 adcvalue=0;
    u8 tbuf[20];
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            GUI_SetColor(GUI_RED);
            GUI_SetFont(GUI_FONT_24B_ASCII);
        
            //��ʾADֵ
            adcvalue=Get_Adc_Average(ADC_CHANNEL_5,10); //��ȡADֵ
            GUI_DispDecAt(adcvalue,140,50,4);
        
            //��ʾ����
            HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);//��ȡ����
            sprintf((char*)tbuf,"20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); 
            GUI_DispStringAt((const char*)tbuf,140,80);//��ʾ����
            
            //��ʾʱ��
            HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);//��ȡʱ��
			sprintf((char*)tbuf,"%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds); 
			GUI_DispStringAt((const char*)tbuf,140,110);//��ʾ����	
			break;
        case WM_INIT_DIALOG:
            //FRAMWIN��ʼ��
            hItem=pMsg->hWin;
            FRAMEWIN_SetFont(hItem,GUI_FONT_20B_ASCII);
            FRAMEWIN_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_SetText(hItem,"ALIENTEK TEXT");
            FRAMEWIN_SetTextColor(hItem,0x0000FFFF);

            //TEXT��ʼ��
            hItem=WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
            TEXT_SetText(hItem, "AD VALUE:");

            //���ڳ�ʼ��
            hItem=WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
            TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
            TEXT_SetText(hItem, "DATE:");
        
            //ʱ���ʼ��
            hItem=WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
            TEXT_SetTextColor(hItem, 0x00000000);
            TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
            TEXT_SetText(hItem, "TIME:");
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
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//�����Ի���
void CreateTextDemo(void) 
{
    WM_SetCallback(WM_HBKWIN,_cbBkWindow);//���ñ������ڻص�����
    TextDialoghWin=GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),_cbDialog,WM_HBKWIN,0,0);
    while(1)
    {
        GUI_Delay(100);
        WM_InvalidateWindow(TextDialoghWin); //�Ի��򴰿���Ч
    }
}
