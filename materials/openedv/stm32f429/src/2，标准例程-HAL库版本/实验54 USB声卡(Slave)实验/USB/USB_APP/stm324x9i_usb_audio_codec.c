#include "stm324x9i_usb_audio_codec.h"
#include "usbd_audio_core.h"
#include "wm8978.h"
#include "sai.h"
#include "malloc.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//USB�����ײ�ӿں��� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
u8 volume=0;								//��ǰ����  

vu8 audiostatus=0;							//bit0:0,��ͣ����;1,��������   
vu8 saiplaybuf=0;							//�������ŵ���Ƶ֡������
vu8 saisavebuf=0;							//��ǰ���浽����Ƶ������ 
#define AUDIO_BUF_NUM		100				//���ڲ��õ���USBͬ���������ݲ���
											//��STM32 SAI���ٶȺ�USB���͹������ݵ��ٶȴ��ڲ���,������48Khz��,ʵ
											//��SAI�ǵ���48Khz(47.991Khz)��,���Ե����͹�����������,���STM32����
											//�ٶȿ�,������дλ��׷�ϲ���λ��(saisavebuf==saiplaybuf)ʱ,�ͻ����
											//���.���þ������AUDIO_BUF_NUMֵ,���Ծ������ٻ������. 
								
u8 *saibuf[AUDIO_BUF_NUM]; 					//��Ƶ����֡,ռ���ڴ���=AUDIO_BUF_NUM*AUDIO_OUT_PACKET �ֽ�
  
//��Ƶ����SAI DMA����ص�����
void audio_sai_dma_callback(void) 
{      
	if((saiplaybuf==saisavebuf)&&audiostatus==0)
	{ 
		SAI_Play_Stop();
	}else
	{
		saiplaybuf++;
		if(saiplaybuf>(AUDIO_BUF_NUM-1))saiplaybuf=0;
		if(DMA2_Stream3->CR&(1<<19))
		{	 
			DMA2_Stream3->M0AR=(u32)saibuf[saiplaybuf];//ָ����һ��buf 
		}
		else 
		{   		
			DMA2_Stream3->M1AR=(u32)saibuf[saiplaybuf];//ָ����һ��buf 
		} 
	}
}  

//������Ƶ�ӿ�
//OutputDevice:����豸ѡ��,δ�õ�.
//Volume:������С,0~100
//AudioFreq:��Ƶ������
uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{   
	u16 t=0;
	for(t=0;t<AUDIO_BUF_NUM;t++)		//�ڴ����� 
	{
		saibuf[t]=mymalloc(SRAMIN,AUDIO_OUT_PACKET);
	}
	if(saibuf[AUDIO_BUF_NUM-1]==NULL)	//�ڴ�����ʧ��
	{
		printf("Malloc Error!\r\n");
		for(t=0;t<AUDIO_BUF_NUM;t++)myfree(SRAMIN,saibuf[t]); 
		return 1;
	} 
    SAIA_Init(SAI_MODEMASTER_TX,SAI_CLOCKSTROBING_RISINGEDGE,SAI_DATASIZE_16);//����SAI,������,16λ����				
	SAIA_SampleRate_Set(AudioFreq);	//���ò�����  
	EVAL_AUDIO_VolumeCtl(Volume);	//��������
	SAIA_TX_DMA_Init(saibuf[0],saibuf[1],AUDIO_OUT_PACKET/2,1); //����TX DMA,16λ 
 	sai_tx_callback=audio_sai_dma_callback;		//�ص�����ָ��audio_sai_dma_callback
	SAI_Play_Start();							//����DMA  
	printf("EVAL_AUDIO_Init:%d,%d\r\n",Volume,AudioFreq);
	return 0; 
}
 
//��ʼ������Ƶ����
//pBuffer:��Ƶ�������׵�ַָ��
//Size:��������С(��λ:�ֽ�)
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size)
{  
	printf("EVAL_AUDIO_Play:%x,%d\r\n",pBuffer,Size);
	return 0;
}

//��ͣ/�ָ���Ƶ������
//Cmd:0,��ͣ����;1,�ָ����� 
//����ֵ:0,�ɹ�
//    ����,����ʧ��
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd)
{     
	if(Cmd==AUDIO_PAUSE)
	{
 		audiostatus=0; 
	}else
	{
		audiostatus=1; 
		SAI_Play_Start();	//����DMA  
	} 
	return 0;
}
 
//ֹͣ����
//Option:���Ʋ���,1/2,���:CODEC_PDWN_HW����
//����ֵ:0,�ɹ�
//    ����,����ʧ��
uint32_t EVAL_AUDIO_Stop(uint32_t Option)
{ 
	printf("EVAL_AUDIO_Stop:%d\r\n",Option);
	audiostatus=0;
	return 0;
} 
//�������� 
//Volume:0~100
//����ֵ:0,�ɹ�
//    ����,����ʧ��
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume)
{ 
	volume=Volume; 
	WM8978_HPvol_Set(volume*0.63,volume*0.63);
	WM8978_SPKvol_Set(volume*0.63);
	return 0;
} 
//��������
//Cmd:0,����
//    1,����
//����ֵ:0,����
//    ����,�������
uint32_t EVAL_AUDIO_Mute(uint32_t Cmd)
{  
	if(Cmd==AUDIO_MUTE_ON)
	{
		WM8978_HPvol_Set(0,0);
		WM8978_SPKvol_Set(0);
	}else
	{
		WM8978_HPvol_Set(volume*0.63,volume*0.63);
		WM8978_SPKvol_Set(volume*0.63);
	}
	return 0;
}  
//������Ƶ������
//Addr:��Ƶ�����������׵�ַ
//Size:��Ƶ��������С(��λ:harf word,Ҳ����2���ֽ�)
void Audio_MAL_Play(uint32_t Addr, uint32_t Size)
{  
	u16 i;
	u8 t=saisavebuf;
	u8 *p=(u8*)Addr;
	u8 curplay=saiplaybuf;	//��ǰ���ڲ��ŵĻ���֡���
	if(curplay)curplay--;
	else curplay=AUDIO_BUF_NUM-1; 
	audiostatus=1;
	t++;
	if(t>(AUDIO_BUF_NUM-1))t=0; 
	if(t==curplay)			//д���������˵�ǰ���ڲ��ŵ�֡,������һ֡
	{
		t++;
		if(t>(AUDIO_BUF_NUM-1))t=0;   
		printf("bad position:%d\r\n",t);
	}
	saisavebuf=t;
	for(i=0;i<Size*2;i++)
	{
		saibuf[saisavebuf][i]=p[i];
	}
	SAI_Play_Start();		//����DMA  
}






















