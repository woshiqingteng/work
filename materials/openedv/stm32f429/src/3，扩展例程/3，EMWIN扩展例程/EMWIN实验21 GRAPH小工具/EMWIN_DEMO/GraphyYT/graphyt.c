#include "graphyt.h"
#include "DIALOG.h"
#include "stdlib.h"
#include "math.h"
#include "GUI.h"
#include "usart.h"
#include "adc.h"
#include "malloc.h"
#include "includes.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin GRAPH�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/19
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//�ؼ�ID
#define ID_FRAMEWIN_0 	(GUI_ID_USER + 0x00)
#define ID_GRAPH_0 		(GUI_ID_USER + 0x01)
#define ID_BUTTON_0 	(GUI_ID_USER + 0x02)
#define ID_BUTTON_1 	(GUI_ID_USER + 0x03)
#define ID_BUTTON_2 	(GUI_ID_USER + 0x04)
#define ID_BUTTON_3 	(GUI_ID_USER + 0x05)
#define ID_BUTTON_4 	(GUI_ID_USER + 0x06)
#define ID_BUTTON_5 	(GUI_ID_USER + 0x07)

#define BORDER_TOP        	10
#define BORDER_BOTTOM     	15
#define BORDER_LEFT       	40
#define BORDER_RIGHT      	10

#define VERMULTIPLE         (10)        //��ֱ�̶���������

static GRAPH_SCALE_Handle hScaleV;   	//��ֱ�̶Ⱦ�� 
static GRAPH_SCALE_Handle hScaleH;   	//ˮƽ�̶Ⱦ��


_oscill_dev oscilldev;

GUI_COLOR LineColor[]={GUI_RED,GUI_BLACK,GUI_WHITE,GUI_YELLOW,GUI_BLUE,GUI_DARKMAGENTA};
GUI_COLOR BackColor[]={GUI_BLACK,GUI_LIGHTRED,GUI_DARKMAGENTA,GUI_WHITE,GUI_GREEN};
//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0},
	{ GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 10, 10, 653, 428, 0, 0x0, 0},
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 685, 15, 80, 50, 0, 0x0, 0},
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 685, 90, 80, 50, 0, 0x0, 0},
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 685, 165, 80, 50, 0, 0x0, 0},
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_3, 685, 240, 80, 50, 0, 0x0, 0},
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_4, 685, 315, 80, 50, 0, 0x0, 0},
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_5, 685, 390, 80, 50, 0, 0x0, 0},
};

//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int NCode;
	int Id;
	
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			//��ʼ��FRAMEWIN
			hItem = pMsg->hWin;
			FRAMEWIN_SetText(hItem, "ALIENTEK GRAPH YT");
			FRAMEWIN_SetFont(hItem, GUI_FONT_20B_ASCII);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
			//��ʼ��GRAPH
			hItem=WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
			oscilldev.linecolorindex=0;	        //����Ĭ�Ϻ�ɫ
			oscilldev.backcolorindex=2;	        //����Ĭ�Ϻ�ɫ
			oscilldev.linecolor=LineColor[oscilldev.linecolorindex];
			oscilldev.backcolor=BackColor[oscilldev.backcolorindex];
			oscilldev.graphdata=GRAPH_DATA_YT_Create(oscilldev.linecolor,600,0,0);  //����YT���ݶ���
			GRAPH_AttachData(hItem,oscilldev.graphdata);		                    //�����ݶ�����ӵ�GRAPH�ؼ���
			GRAPH_SetBorder(hItem,BORDER_LEFT,BORDER_TOP,BORDER_RIGHT,BORDER_BOTTOM);//���ñ߽�
            GRAPH_SetGridDistX(hItem,50);		//����ˮƽ������
			GRAPH_SetGridDistY(hItem,20);		//���ô�ֱ������
			GRAPH_SetGridVis(hItem,1);			//��������ɼ�
		
			hScaleV=GRAPH_SCALE_Create(35,GUI_TA_RIGHT,GRAPH_SCALE_CF_VERTICAL,20); //���ƴ�ֱ�̶�
			GRAPH_SCALE_SetTextColor(hScaleV,GUI_YELLOW);
			GRAPH_AttachScale(hItem,hScaleV);   //���̶ȶ��󸽼ӵ�ͼ��С����
            GRAPH_SCALE_SetFactor(hScaleV,20);  //���ÿ̶�ϵ��
            GRAPH_SCALE_SetOff(hScaleV,200);    //��ֱ�̶������ƶ�200�����ص㣬������ֱ�̶Ȼ��и�ֵ
			
			hScaleH=GRAPH_SCALE_Create(420,GUI_TA_HCENTER,GRAPH_SCALE_CF_HORIZONTAL,50);//����ˮƽ�̶�
			GRAPH_SCALE_SetTextColor(hScaleH,GUI_DARKGREEN);
			GRAPH_AttachScale(hItem,hScaleH);   //���̶ȶ��󸽼ӵ�ͼ��С����

			//��ʼ��BUTTON
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
			BUTTON_SetFont(hItem,&GUI_Font16B_ASCII);
			BUTTON_SetText(hItem,"COLOR");
			
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetFont(hItem,&GUI_Font16B_ASCII);
			BUTTON_SetText(hItem,"BKCOLOR");
            
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
			BUTTON_SetFont(hItem,&GUI_Font16B_ASCII);
			BUTTON_SetText(hItem,"TIME/DIV-");
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
			BUTTON_SetFont(hItem,&GUI_Font16B_ASCII);
			BUTTON_SetText(hItem,"TIME/DIV+");
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
			BUTTON_SetFont(hItem,&GUI_Font16B_ASCII);
			BUTTON_SetText(hItem,"UNUSED");
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
			BUTTON_SetFont(hItem,&GUI_Font16B_ASCII);
			BUTTON_SetText(hItem,"UNUSED");
		   
            oscilldev.resolut=(float)3300/4096;    //STM32��ADΪ12λ����ôÿ��ADֵΪ3300/4096=0.806mv
            oscilldev.div=100;
            TIM3_Init(oscilldev.div-1,96);	        //AD����Ƶ�ʣ�10KHz
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
							hItem=WM_GetDialogItem(pMsg->hWin,ID_GRAPH_0);
							oscilldev.linecolorindex+=1;
							if(oscilldev.linecolorindex>GUI_COUNTOF(LineColor)) oscilldev.linecolorindex=0;
							oscilldev.linecolor=LineColor[oscilldev.linecolorindex];
							GRAPH_DetachData(hItem,oscilldev.graphdata);//�ȷ�������
							GRAPH_DATA_YT_Delete(oscilldev.graphdata);	//ɾ�����������ݶ���
							oscilldev.graphdata=GRAPH_DATA_YT_Create(oscilldev.linecolor,600,0,0);//���´������ݶ���
							GRAPH_AttachData(hItem,oscilldev.graphdata);
							break;
					}
					break;
                case ID_BUTTON_1: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							hItem=WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
							oscilldev.backcolorindex+=1;
							if(oscilldev.backcolorindex>GUI_COUNTOF(BackColor)) oscilldev.backcolorindex=0;
							oscilldev.backcolor=BackColor[oscilldev.backcolorindex];
							GRAPH_SetColor(hItem,oscilldev.backcolor,GRAPH_CI_BK);//����GRAPH������ɫ
							break;
					}
					break;
				case ID_BUTTON_2: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							oscilldev.div+=10;
                            if(oscilldev.div>1000)oscilldev.div=100;
                            __HAL_TIM_DISABLE(&TIM3_Handler);//TIM3ʧ��
							TIM3_Init(oscilldev.div,95);	
							break;
					}
					break;
				case ID_BUTTON_3: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED: 
							break;
						case WM_NOTIFICATION_RELEASED:
							oscilldev.div-=10;
							if(oscilldev.div<10) oscilldev.div=10;
                            __HAL_TIM_DISABLE(&TIM3_Handler);//TIM3ʧ��
							TIM3_Init(oscilldev.div,95);	
							break;
					}
					break;
				case ID_BUTTON_4:   //δʹ��
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
					}
					break;
				case ID_BUTTON_5:   //δʹ��
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
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

//��������WM_HBKWIN�ص�����
static void _cbBkWindow(WM_MESSAGE* pMsg)
{
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();;
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//ͼ����ʾdemo
void Graphyt_demo(void) 
{
	WM_HWIN hWin,ClientWin;
	int Graph_xSize,Graph_ySize;                
	int i;
    
	WM_SetCallback(WM_HBKWIN,_cbBkWindow);    //���ñ������ڻص�����
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN,0,0);
	ClientWin = WM_GetDialogItem(hWin, ID_GRAPH_0);
	Graph_xSize=WM_GetWindowSizeX(ClientWin);
	Graph_ySize=WM_GetWindowSizeY(ClientWin);
	
	oscilldev.dataxsize=Graph_xSize-BORDER_LEFT-BORDER_RIGHT-3;
	oscilldev.dataysize=Graph_ySize-BORDER_TOP-BORDER_BOTTOM-3;	
	oscilldev.buffer=mymalloc(SRAMIN,oscilldev.dataxsize*4);	//�����ڴ�
	GUI_Exec();
	while(1)
	{
		if(oscilldev.adflag==1)
		{	
			for(i=0;i<oscilldev.dataxsize;i++)
			{
                //��GRAPHͼ��С�����������,ע��Ҫ��20������Ϊ��ֱ��������(����)20����
                //���һ�Ҫ��200,��Ϊ��ֱ����������200�����ص�
				GRAPH_DATA_YT_AddValue(oscilldev.graphdata,oscilldev.buffer[i]/20+200);
			}
			memset(oscilldev.buffer,0,oscilldev.dataxsize);	//���buffer
			oscilldev.adflag=0;	
		}
		GUI_Delay(100);
	}
}

