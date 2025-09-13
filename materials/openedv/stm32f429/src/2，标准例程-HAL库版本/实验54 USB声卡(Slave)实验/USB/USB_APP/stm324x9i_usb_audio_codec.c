#include "stm324x9i_usb_audio_codec.h"
#include "usbd_audio_core.h"
#include "wm8978.h"
#include "sai.h"
#include "malloc.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//USB声卡底层接口函数 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 
u8 volume=0;								//当前音量  

vu8 audiostatus=0;							//bit0:0,暂停播放;1,继续播放   
vu8 saiplaybuf=0;							//即将播放的音频帧缓冲编号
vu8 saisavebuf=0;							//当前保存到的音频缓冲编号 
#define AUDIO_BUF_NUM		100				//由于采用的是USB同步传输数据播放
											//而STM32 SAI的速度和USB传送过来数据的速度存在差异,比如在48Khz下,实
											//际SAI是低于48Khz(47.991Khz)的,所以电脑送过来的数据流,会比STM32播放
											//速度快,缓冲区写位置追上播放位置(saisavebuf==saiplaybuf)时,就会出现
											//混叠.设置尽量大的AUDIO_BUF_NUM值,可以尽量减少混叠次数. 
								
u8 *saibuf[AUDIO_BUF_NUM]; 					//音频缓冲帧,占用内存数=AUDIO_BUF_NUM*AUDIO_OUT_PACKET 字节
  
//音频数据SAI DMA传输回调函数
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
			DMA2_Stream3->M0AR=(u32)saibuf[saiplaybuf];//指向下一个buf 
		}
		else 
		{   		
			DMA2_Stream3->M1AR=(u32)saibuf[saiplaybuf];//指向下一个buf 
		} 
	}
}  

//配置音频接口
//OutputDevice:输出设备选择,未用到.
//Volume:音量大小,0~100
//AudioFreq:音频采样率
uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{   
	u16 t=0;
	for(t=0;t<AUDIO_BUF_NUM;t++)		//内存申请 
	{
		saibuf[t]=mymalloc(SRAMIN,AUDIO_OUT_PACKET);
	}
	if(saibuf[AUDIO_BUF_NUM-1]==NULL)	//内存申请失败
	{
		printf("Malloc Error!\r\n");
		for(t=0;t<AUDIO_BUF_NUM;t++)myfree(SRAMIN,saibuf[t]); 
		return 1;
	} 
    SAIA_Init(SAI_MODEMASTER_TX,SAI_CLOCKSTROBING_RISINGEDGE,SAI_DATASIZE_16);//设置SAI,主发送,16位数据				
	SAIA_SampleRate_Set(AudioFreq);	//设置采样率  
	EVAL_AUDIO_VolumeCtl(Volume);	//设置音量
	SAIA_TX_DMA_Init(saibuf[0],saibuf[1],AUDIO_OUT_PACKET/2,1); //配置TX DMA,16位 
 	sai_tx_callback=audio_sai_dma_callback;		//回调函数指向audio_sai_dma_callback
	SAI_Play_Start();							//开启DMA  
	printf("EVAL_AUDIO_Init:%d,%d\r\n",Volume,AudioFreq);
	return 0; 
}
 
//开始播放音频数据
//pBuffer:音频数据流首地址指针
//Size:数据流大小(单位:字节)
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size)
{  
	printf("EVAL_AUDIO_Play:%x,%d\r\n",pBuffer,Size);
	return 0;
}

//暂停/恢复音频流播放
//Cmd:0,暂停播放;1,恢复播放 
//返回值:0,成功
//    其他,设置失败
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd)
{     
	if(Cmd==AUDIO_PAUSE)
	{
 		audiostatus=0; 
	}else
	{
		audiostatus=1; 
		SAI_Play_Start();	//开启DMA  
	} 
	return 0;
}
 
//停止播放
//Option:控制参数,1/2,详见:CODEC_PDWN_HW定义
//返回值:0,成功
//    其他,设置失败
uint32_t EVAL_AUDIO_Stop(uint32_t Option)
{ 
	printf("EVAL_AUDIO_Stop:%d\r\n",Option);
	audiostatus=0;
	return 0;
} 
//音量设置 
//Volume:0~100
//返回值:0,成功
//    其他,设置失败
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume)
{ 
	volume=Volume; 
	WM8978_HPvol_Set(volume*0.63,volume*0.63);
	WM8978_SPKvol_Set(volume*0.63);
	return 0;
} 
//静音控制
//Cmd:0,正常
//    1,静音
//返回值:0,正常
//    其他,错误代码
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
//播放音频数据流
//Addr:音频数据流缓存首地址
//Size:音频数据流大小(单位:harf word,也就是2个字节)
void Audio_MAL_Play(uint32_t Addr, uint32_t Size)
{  
	u16 i;
	u8 t=saisavebuf;
	u8 *p=(u8*)Addr;
	u8 curplay=saiplaybuf;	//当前正在播放的缓存帧编号
	if(curplay)curplay--;
	else curplay=AUDIO_BUF_NUM-1; 
	audiostatus=1;
	t++;
	if(t>(AUDIO_BUF_NUM-1))t=0; 
	if(t==curplay)			//写缓存碰上了当前正在播放的帧,跳到下一帧
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
	SAI_Play_Start();		//开启DMA  
}






















