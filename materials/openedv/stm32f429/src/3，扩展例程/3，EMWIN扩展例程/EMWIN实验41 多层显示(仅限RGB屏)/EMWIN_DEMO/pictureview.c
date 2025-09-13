#include <stddef.h>
#include "pictureview.h"
#include "GUI.h"
#include "sys.h"
#include "ff.h"
#include "delay.h"
#include "exfuns.h"
#include "malloc.h"
#include "string.h"
#include "jpegdisplay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin ˫����ʾ 
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/6/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//��ֱˢ��(�������»���ˮƽ��)
//xSize:��Ļ���
//ySize:��Ļ�߶�
//Delay:��ʱ
static void EffectLineV0(int xSize,int ySize,int Delay) 
{
    int i;
    int j;

    GUI_SetColor(GUI_TRANSPARENT);      //������ɫΪ͸��
    for(i=0;i<32;i++) 
    {
        for (j=i;j<ySize;j+=32) 
        {
            GUI_DrawHLine(j,0,xSize-1); //����ˮƽ��
        }
        GUI_X_Delay(Delay);
    }
}

//��ֱˢ��(�������ϻ���ˮƽ��)
//xSize:��Ļ���
//ySize:��Ļ�߶�
//Delay:��ʱ
static void EffectLineV1(int xSize,int ySize,int Delay) 
{
    int i;
    int j;
    
    GUI_SetColor(GUI_TRANSPARENT);      //������ɫΪ͸��
    for(i=31;i>=0;i--) 
    {
        for(j=i;j<ySize;j+=32) 
        {
            GUI_DrawHLine(j,0,xSize-1); //����ˮƽ��
        }
        GUI_X_Delay(Delay);
    }
}

//ˮƽ����(�������һ��ƴ�ֱ��)
//xSize:��Ļ���
//ySize:��Ļ�߶�
//Delay:��ʱ
static void EffectLineH0(int xSize,int ySize,int Delay) 
{
    int i;
    int j;

    GUI_SetColor(GUI_TRANSPARENT);      //������ɫΪ͸��
    for(i=0;i<32;i++) 
    {
        for(j=i;j<xSize;j+=32) 
        {
            GUI_DrawVLine(j,0,ySize-1); //���ƴ�ֱ��
        }
        GUI_X_Delay(Delay);
    }
}

//���ƴ�ֱ��(����������ƴ�ֱ��)
//xSize:��Ļ���
//ySize:��Ļ�߶�
//Delay:��ʱ
static void EffectLineH1(int xSize,int ySize,int Delay) 
{
    int i;
    int j;
    GUI_SetColor(GUI_TRANSPARENT);      //������ɫΪ͸��
    
    for(i=31;i>=0;i--) 
    {
        for (j=i;j<xSize;j+=32) 
        {
            GUI_DrawVLine(j,0,ySize-1); //���ƴ�ֱ��
        }
        GUI_X_Delay(Delay);
    }
}

//���θ��£���ͬʱ����ˮƽ�ʹ�ֱ����(�����Ͻ������½ǻ���)
//xSize:��Ļ���
//ySize:��Ļ�߶�
//Delay:��ʱ
static void EffectLineHV0(int xSize, int ySize, int Delay) 
{
    int i;
    int j;
    
    GUI_SetColor(GUI_TRANSPARENT);      //������ɫΪ͸��
    for(i=0;i<32;i++) 
    {
        for(j=i;j<ySize;j+=32) 
        {
            GUI_DrawHLine(j,0,xSize-1); //����ˮƽ��
        }
        for(j=i;j<xSize;j+=32)
        {    
            GUI_DrawVLine(j,0,ySize-1); //���ƴ�ֱ��
        }
        GUI_X_Delay(Delay);
    }
}

//���θ��£���ͬʱ����ˮƽ�ʹ�ֱ����(����С�������Ͻǻ���)
//xSize:��Ļ���
//ySize:��Ļ�߶�
//Delay:��ʱ
static void EffectLineHV1(int xSize,int ySize,int Delay) 
{
    int i;
    int j;
    
    GUI_SetColor(GUI_TRANSPARENT);      //������ɫΪ͸��
    for(i=31;i>=0;i--) 
    {
        for(j=i;j<ySize;j+=32) 
        {
            GUI_DrawHLine(j,0,xSize-1); //����ˮƽ��
        }
        for(j=i;j<xSize;j+=32) 
        {
            GUI_DrawVLine(j,0,ySize-1); //���ƴ�ֱ��
        }
        GUI_X_Delay(Delay);
    }
}

//ͼƬ���º�������
static void (* _apFunc[])(int xSize,int ySize,int Delay)= 
{
    EffectLineV0,
    EffectLineV1,
    EffectLineH0,
    EffectLineH1,
    EffectLineHV0,
    EffectLineHV1,
};

//��䵱ǰ��
//picname:Ҫ��ʾ��ͼƬ
static void FillCurrentLayer(u8 *picname) 
{
    u8 err=0;
    GUI_MULTIBUF_Begin();
    err=displyjpeg(picname,1,0,0,1,1);//��LCD�м���ʾͼƬ
    if(err) printf("ͼƬ%s��ʾ����,������%d\r\n",picname,err);
    GUI_MULTIBUF_End();
}

//ͼƬ��ʾ
//pFunc:ͼƬ���º���
//Index:������
//Delay:��ʱ
static void ShowPicture(void (* pFunc)(int xSize, int ySize, int Delay),u8 *picname,int Delay) 
{
    int xSize;
    int ySize;

    xSize=LCD_GetXSize(); //��ȡ��Ļ���
    ySize=LCD_GetYSize(); //��ȡ��Ļ�߶�

    //���Ƶ�0���ͼƬ�����ɼ�����Ϊ����1�㸲����
    GUI_SelectLayer(0);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    FillCurrentLayer(picname);
    //��1����͸��ɫ��䣬������1���ͼƬ�Ϳ��Կ�����
    GUI_SelectLayer(1);
    GUI_SetColor(GUI_TRANSPARENT);
    pFunc(xSize,ySize,Delay);

    //��1��Ҳ��ʾ����ͼƬ��ֻ�е�һ��Ҳ��ʾ�˵�0���ͼƬ
    //�Ż�����ʾ��һ����Ƭ��ʱ���и��µ�Ч�����֡�
    FillCurrentLayer(picname);
}

//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO *tfileinfo;	//��ʱ�ļ���Ϣ	    			     
	tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));//�����ڴ�
    res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼ 
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�  	 
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;//������/��ĩβ��,�˳�	 		 
			res=f_typetell((u8*)tfileinfo->fname);
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	}  
	myfree(SRAMIN,tfileinfo);//�ͷ��ڴ�
	return rval;
}

//������ʾDemo
void PictureViewDemo(void)
{
    int Index=0;
    u8 res;
 	DIR picdir;	 		    //ͼƬĿ¼
	FILINFO *picfileinfo;   //�ļ���Ϣ
	u8 *pname;			    //��·�����ļ���
	u16 totpicnum; 		    //ͼƬ�ļ�����
	u16 curindex;		    //ͼƬ��ǰ����
    u16 temp;
    u32 *picoffblktbl;	    //ͼƬ�ļ�off block������ 
    
    totpicnum=pic_get_tnum("0:/APP/PICTUREVIEW");           //�õ�����Ч�ļ���
    picfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//�����ڴ�
 	pname=mymalloc(SRAMIN,_MAX_LFN*2+1);					//Ϊ��·�����ļ��������ڴ�
 	picoffblktbl=mymalloc(SRAMIN,4*totpicnum);				//����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ����
 	while(!picfileinfo||!pname||!picoffblktbl)				//�ڴ�������
 	{	    
		printf("�ڴ�����ʧ��\r\n");
		delay_ms(200);				  				  
	} 
    //��¼����
    res=f_opendir(&picdir,"0:/APP/PICTUREVIEW"); //��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
	        res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
			temp=picdir.blk_ofs;							//��¼��ǰblockƫ��
	        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�	 	 
			res=f_typetell((u8*)picfileinfo->fname);	
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				picoffblktbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}  
	delay_ms(1500);  	   
	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&picdir,(const TCHAR*)"0:/APP/PICTUREVIEW"); 	//��Ŀ¼
    while(1) 
    {
		dir_sdi(&picdir,picoffblktbl[curindex]);		//�ı䵱ǰĿ¼����	   
        res=f_readdir(&picdir,picfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||picfileinfo->fname[0]==0)break;	//������/��ĩβ��,�˳�
		strcpy((char*)pname,"0:/APP/PICTUREVIEW/");	    //����·��(Ŀ¼)
		strcat((char*)pname,(const char*)picfileinfo->fname);//���ļ������ں���
        ShowPicture(_apFunc[Index++],pname,25);
        if(Index==6) Index=0;
        curindex++;		   	    
        if(curindex>=totpicnum)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
        GUI_Delay(2000);
    }
    myfree(SRAMIN,picfileinfo);			//�ͷ��ڴ�						   		    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,picoffblktbl);		//�ͷ��ڴ�
}
