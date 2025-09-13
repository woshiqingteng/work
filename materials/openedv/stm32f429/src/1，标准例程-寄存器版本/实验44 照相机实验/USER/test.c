#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h"    
#include "key.h" 
#include "malloc.h" 
#include "usmart.h"  
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"  
#include "text.h"	   
#include "piclib.h"	
#include "string.h"		
#include "math.h"	 
#include "ov5640.h" 
#include "dcmi.h"  
#include "pcf8574.h"  
//ALIENTEK ������STM32F429������ ʵ��44
//����� ʵ�� 
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

vu8 bmp_request=0;						//bmp��������:0,��bmp��������;1,��bmp��������,��Ҫ��֡�ж�����,�ر�DCMI�ӿ�.
u8 ovx_mode=0;							//bit0:0,RGB565ģʽ;1,JPEGģʽ 
u16 curline=0;							//����ͷ�������,��ǰ�б��
u16 yoffset=0;							//y�����ƫ����

#define jpeg_buf_size   4*1024*1024		//����JPEG���ݻ���jpeg_buf�Ĵ�С(4M�ֽ�)
#define jpeg_line_size	2*1024			//����DMA��������ʱ,һ�����ݵ����ֵ

u32 *dcmi_line_buf[2];					//RGB��ʱ,����ͷ����һ��һ�ж�ȡ,�����л���  
u32 *jpeg_data_buf;						//JPEG���ݻ���buf 

volatile u32 jpeg_data_len=0; 			//buf�е�JPEG��Ч���ݳ��� 
volatile u8 jpeg_data_ok=0;				//JPEG���ݲɼ���ɱ�־ 
										//0,����û�вɼ���;
										//1,���ݲɼ�����,���ǻ�û����;
										//2,�����Ѿ����������,���Կ�ʼ��һ֡����
		
 
//����JPEG����
//���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
void jpeg_data_process(void)
{
	u16 i;
	u16 rlen;			//ʣ�����ݳ���
	u32 *pbuf;
	curline=yoffset;	//������λ
	if(ovx_mode&0X01)	//ֻ����JPEG��ʽ��,����Ҫ������.
	{
		if(jpeg_data_ok==0)	//jpeg���ݻ�δ�ɼ���?
		{
			DMA2_Stream1->CR&=~(1<<0);		//ֹͣ��ǰ����
			while(DMA2_Stream1->CR&0X01);	//�ȴ�DMA2_Stream1������ 
			rlen=jpeg_line_size-DMA2_Stream1->NDTR;//�õ�ʣ�����ݳ���	
			pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ,�������
			if(DMA2_Stream1->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=dcmi_line_buf[1][i];//��ȡbuf1�����ʣ������
			else for(i=0;i<rlen;i++)pbuf[i]=dcmi_line_buf[0][i];//��ȡbuf0�����ʣ������ 
			jpeg_data_len+=rlen;			//����ʣ�೤��
			jpeg_data_ok=1; 				//���JPEG���ݲɼ��갴��,�ȴ�������������
		}
		if(jpeg_data_ok==2)	//��һ�ε�jpeg�����Ѿ���������
		{
			DMA2_Stream1->NDTR=jpeg_line_size;//���䳤��Ϊjpeg_buf_size*4�ֽ�
			DMA2_Stream1->CR|=1<<0;			//���´���
			jpeg_data_ok=0;					//�������δ�ɼ�
			jpeg_data_len=0;				//�������¿�ʼ
		}
	}else
	{  
		if(bmp_request==1)	//��bmp��������,�ر�DCMI
		{
			DCMI_Stop();	//ֹͣDCMI
			bmp_request=0;	//������������.
		}
		LCD_SetCursor(0,0);  
		LCD_WriteRAM_Prepare();				//��ʼд��GRAM  
	}  
} 
//jpeg���ݽ��ջص�����
void jpeg_dcmi_rx_callback(void)
{  
	u16 i;
	u32 *pbuf;
	pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ
	if(DMA2_Stream1->CR&(1<<19))//buf0����,��������buf1
	{ 
		for(i=0;i<jpeg_line_size;i++)pbuf[i]=dcmi_line_buf[0][i];//��ȡbuf0���������
		jpeg_data_len+=jpeg_line_size;//ƫ��
	}else //buf1����,��������buf0
	{
		for(i=0;i<jpeg_line_size;i++)pbuf[i]=dcmi_line_buf[1][i];//��ȡbuf1���������
		jpeg_data_len+=jpeg_line_size;//ƫ�� 
	} 
} 

//RGB�����ݽ��ջص�����
void rgblcd_dcmi_rx_callback(void)
{  
	u16 *pbuf;
	if(DMA2_Stream1->CR&(1<<19))//DMAʹ��buf1,��ȡbuf0
	{ 
		pbuf=(u16*)dcmi_line_buf[0]; 
	}else 						//DMAʹ��buf0,��ȡbuf1
	{
		pbuf=(u16*)dcmi_line_buf[1]; 
	} 	
	LTDC_Color_Fill(0,curline,lcddev.width-1,curline,pbuf);//DM2D��� 
	if(curline<lcddev.height)curline++;
	if(bmp_request==1&&curline==(lcddev.height-1))//��bmp��������,�ر�DCMI
	{
		DCMI_Stop();	//ֹͣDCMI
		bmp_request=0;	//������������.
	}
}

//�л�ΪOV5640ģʽ
void sw_ov5640_mode(void)
{ 
 	OV5640_WR_Reg(0X3017,0XFF);	//����OV5650���(����������ʾ)
	OV5640_WR_Reg(0X3018,0XFF); 
	//GPIOC8/9/11�л�Ϊ DCMI�ӿ�
 	GPIO_AF_Set(GPIOC,8,13);	//PC8,AF13  DCMI_D2
 	GPIO_AF_Set(GPIOC,9,13);	//PC9,AF13  DCMI_D3
 	GPIO_AF_Set(GPIOC,11,13);	//PC11,AF13 DCMI_D4  
} 
//�л�ΪSD��ģʽ
void sw_sdcard_mode(void)
{
	OV5640_WR_Reg(0X3017,0X00);	//�ر�OV5640ȫ�����(��Ӱ��SD��ͨ��)
	OV5640_WR_Reg(0X3018,0X00);  
 	//GPIOC8/9/11�л�Ϊ SDIO�ӿ�
  	GPIO_AF_Set(GPIOC,8,12);	//PC8,AF12
 	GPIO_AF_Set(GPIOC,9,12);	//PC9,AF12 
 	GPIO_AF_Set(GPIOC,11,12);	//PC11,AF12  
}
//�ļ������������⸲�ǣ�
//mode:0,����.bmp�ļ�;1,����.jpg�ļ�.
//bmp��ϳ�:����"0:PHOTO/PIC13141.bmp"���ļ���
//jpg��ϳ�:����"0:PHOTO/PIC13141.jpg"���ļ���
void camera_new_pathname(u8 *pname,u8 mode)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		if(mode==0)sprintf((char*)pname,"0:PHOTO/PIC%05d.bmp",index);
		else sprintf((char*)pname,"0:PHOTO/PIC%05d.jpg",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//���Դ�����ļ�
		if(res==FR_NO_FILE)break;		//���ļ���������=����������Ҫ��.
		index++;
	}
}  
//OV5640����jpgͼƬ
//����ֵ:0,�ɹ�
//    ����,�������
u8 ov5640_jpg_photo(u8 *pname)
{
	FIL* f_jpg; 
	u8 res=0,headok=0;
	u32 bwr;
	u32 i,jpgstart,jpglen;
	u8* pbuf;
	f_jpg=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	if(f_jpg==NULL)return 0XFF;				//�ڴ�����ʧ��.
	ovx_mode=1;
	jpeg_data_ok=0;
	sw_ov5640_mode();						//�л�ΪOV5640ģʽ 
	OV5640_JPEG_Mode();						//JPEGģʽ  
	OV5640_OutSize_Set(16,4,2592,1944);		//��������ߴ�(500W)  
	dcmi_rx_callback=jpeg_dcmi_rx_callback;	//JPEG�������ݻص�����
	DCMI_DMA_Init((u32)dcmi_line_buf[0],(u32)dcmi_line_buf[1],jpeg_line_size,2,1);//DCMI DMA����    
	DCMI_Start(); 			//�������� 
	while(jpeg_data_ok!=1);	//�ȴ���һ֡ͼƬ�ɼ���
	jpeg_data_ok=2;			//���Ա�֡ͼƬ,������һ֡�ɼ� 
	while(jpeg_data_ok!=1);	//�ȴ��ڶ�֡ͼƬ�ɼ���,�ڶ�֡,�ű��浽SD��ȥ. 
	DCMI_Stop(); 			//ֹͣDMA����
	ovx_mode=0; 
	sw_sdcard_mode();		//�л�ΪSD��ģʽ
	res=f_open(f_jpg,(const TCHAR*)pname,FA_WRITE|FA_CREATE_NEW);//ģʽ0,���߳��Դ�ʧ��,�򴴽����ļ�	 
	if(res==0)
	{
		printf("jpeg data size:%d\r\n",jpeg_data_len*4);//���ڴ�ӡJPEG�ļ���С
		pbuf=(u8*)jpeg_data_buf;
		jpglen=0;	//����jpg�ļ���СΪ0
		headok=0;	//���jpgͷ���
		for(i=0;i<jpeg_data_len*4;i++)//����0XFF,0XD8��0XFF,0XD9,��ȡjpg�ļ���С
		{
			if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD8))//�ҵ�FF D8
			{
				jpgstart=i;
				headok=1;	//����ҵ�jpgͷ(FF D8)
			}
			if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD9)&&headok)//�ҵ�ͷ�Ժ�,����FF D9
			{
				jpglen=i-jpgstart+2;
				break;
			}
		}
		if(jpglen)			//������jpeg���� 
		{
			pbuf+=jpgstart;	//ƫ�Ƶ�0XFF,0XD8��
			res=f_write(f_jpg,pbuf,jpglen,&bwr);
			if(bwr!=jpglen)res=0XFE; 
			
		}else res=0XFD; 
	}
	jpeg_data_len=0;
	f_close(f_jpg); 
	sw_ov5640_mode();		//�л�ΪOV5640ģʽ
	OV5640_RGB565_Mode();	//RGB565ģʽ  
	if(lcdltdc.pwidth!=0)	//RGB��
	{
		dcmi_rx_callback=rgblcd_dcmi_rx_callback;//RGB���������ݻص�����
		DCMI_DMA_Init((u32)dcmi_line_buf[0],(u32)dcmi_line_buf[1],lcddev.width/2,1,1);//DCMI DMA����  
	}else					//MCU ��
	{
		DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,1,0);			//DCMI DMA����,MCU��,����
	}
	myfree(SRAMIN,f_jpg); 
	return res;
}  

int main(void)
{     
	u8 res,fac;							 
	u8 *pname;				//��·�����ļ��� 
	u8 key;					//��ֵ		   
	u8 i;						 
	u8 sd_ok=1;				//0,sd��������;1,SD������. 
 	u8 scale=1;				//Ĭ����ȫ�ߴ�����
	u8 msgbuf[15];			//��Ϣ������ 
	u16 outputheight=0;
	
	Stm32_Clock_Init(360,25,2,8);//����ʱ��,180Mhz
	delay_init(180);			//��ʼ����ʱ���� 
	uart_init(90,115200);		//��ʼ�����ڲ�����Ϊ115200 
	usmart_dev.init(90);
   	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	SDRAM_Init();				//��ʼ��SDRAM 
	LCD_Init();					//��ʼ��LCD
	KEY_Init();					//��ʼ������
	PCF8574_Init();				//��ʼ��PCF8574
	OV5640_Init();				//��ʼ��OV5640
	sw_sdcard_mode();			//�����л�ΪOV5640ģʽ
	W25QXX_Init();				//��ʼ��W25Q256
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD��  
	POINT_COLOR=RED;      
	while(font_init()) 		//����ֿ�
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}  	 
 	Show_Str(30,50,200,16,"������STM32F4/F7������",16,0);	 			    	 
	Show_Str(30,70,200,16,"�����ʵ��",16,0);				    	 
	Show_Str(30,90,200,16,"KEY0:����(bmp��ʽ)",16,0);			    	 
	Show_Str(30,110,200,16,"KEY1:����(jpg��ʽ)",16,0);			    	 
	Show_Str(30,130,200,16,"KEY2:�Զ��Խ�",16,0);					    	 
	Show_Str(30,150,200,16,"WK_UP:FullSize/Scale",16,0);				    	 
	Show_Str(30,170,200,16,"2016��1��7��",16,0);
	res=f_mkdir("0:/PHOTO");		//����PHOTO�ļ���
	if(res!=FR_EXIST&&res!=FR_OK) 	//�����˴���
	{		
		res=f_mkdir("0:/PHOTO");		//����PHOTO�ļ���		
		Show_Str(30,190,240,16,"SD������!",16,0);
		delay_ms(200);				  
		Show_Str(30,190,240,16,"���չ��ܽ�������!",16,0);
		delay_ms(200);				  
		sd_ok=0;  	
	} 	
	dcmi_line_buf[0]=mymalloc(SRAMIN,jpeg_line_size*4);	//Ϊjpeg dma���������ڴ�	
	dcmi_line_buf[1]=mymalloc(SRAMIN,jpeg_line_size*4);	//Ϊjpeg dma���������ڴ�	
	jpeg_data_buf=mymalloc(SRAMEX,jpeg_buf_size);		//Ϊjpeg�ļ������ڴ�(���4MB)
 	pname=mymalloc(SRAMIN,30);//Ϊ��·�����ļ�������30���ֽڵ��ڴ�	 
 	while(pname==NULL||!dcmi_line_buf[0]||!dcmi_line_buf[1]||!jpeg_data_buf)	//�ڴ�������
 	{	    
		Show_Str(30,190,240,16,"�ڴ����ʧ��!",16,0);
		delay_ms(200);				  
		LCD_Fill(30,190,240,146,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}   
	while(OV5640_Init())//��ʼ��OV5640
	{
		Show_Str(30,190,240,16,"OV5640 ����!",16,0);
		delay_ms(200);
	    LCD_Fill(30,190,239,206,WHITE);
		delay_ms(200);
	}	
 	Show_Str(30,210,230,16,"OV5640 ����",16,0); 
	//�Զ��Խ���ʼ��
	OV5640_RGB565_Mode();	//RGB565ģʽ 
	OV5640_Focus_Init(); 
	OV5640_Light_Mode(0);	//�Զ�ģʽ
	OV5640_Color_Saturation(3);//ɫ�ʱ��Ͷ�0
	OV5640_Brightness(4);	//����0
	OV5640_Contrast(3);		//�Աȶ�0
	OV5640_Sharpness(33);	//�Զ����
	OV5640_Focus_Constant();//���������Խ�
	DCMI_Init();			//DCMI����
	if(lcdltdc.pwidth!=0)	//RGB��
	{
		dcmi_rx_callback=rgblcd_dcmi_rx_callback;//RGB���������ݻص�����
		DCMI_DMA_Init((u32)dcmi_line_buf[0],(u32)dcmi_line_buf[1],lcddev.width/2,1,1);//DCMI DMA����  
	}else					//MCU ��
	{
		DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,1,0);			//DCMI DMA����,MCU��,����
	}	
	if(lcddev.height==1024)
	{
		yoffset=(lcddev.height-800)/2;
		outputheight=800;
		OV5640_WR_Reg(0x3035,0X51);//�������֡�ʣ�������ܶ���
	}else if(lcddev.height==1280)
	{
		yoffset=(lcddev.height-600)/2;
		outputheight=600;
		OV5640_WR_Reg(0x3035,0X51);//�������֡�ʣ�������ܶ��� 
	}else 
	{
		yoffset=0;
		outputheight=lcddev.height;
	}
	curline=yoffset;		//������λ
	OV5640_OutSize_Set(16,4,lcddev.width,outputheight);	//����������ʾ
	DCMI_Start(); 			//��������
	LCD_Clear(BLACK);
 	while(1)
	{	
		key=KEY_Scan(0);//��֧������
		if(key)
		{ 
			if(key!=KEY2_PRES)
			{
				if(key==KEY0_PRES)//�����BMP����,��ȴ�1����,ȥ����,�Ի���ȶ���bmp��Ƭ	
				{
					delay_ms(300);
					bmp_request=1;		//����ر�DCMI
					while(bmp_request);	//�ȴ����������
 				}else DCMI_Stop();
			}
			if(key==WKUP_PRES)		//���Ŵ���
			{
				scale=!scale;  
				if(scale==0)
				{
					fac=800/outputheight;//�õ���������
					OV5640_OutSize_Set((1280-fac*lcddev.width)/2,(800-fac*outputheight)/2,lcddev.width,outputheight); 	 
					sprintf((char*)msgbuf,"Full Size 1:1");
				}else 
				{
					OV5640_OutSize_Set(16,4,lcddev.width,outputheight);
					sprintf((char*)msgbuf,"Scale");
				}
				delay_ms(800); 	
			}else if(key==KEY2_PRES)OV5640_Focus_Single(); //�ֶ������Զ��Խ�
			else if(sd_ok)//SD�������ſ�������
			{    
				sw_sdcard_mode();	//�л�ΪSD��ģʽ
				if(key==KEY0_PRES)	//BMP����
				{
					camera_new_pathname(pname,0);	//�õ��ļ���	
					res=bmp_encode(pname,0,yoffset,lcddev.width,outputheight,0);
					sw_ov5640_mode();				//�л�ΪOV5640ģʽ
				}else if(key==KEY1_PRES)//JPG����
				{
					camera_new_pathname(pname,1);//�õ��ļ���	
					res=ov5640_jpg_photo(pname);
					if(scale==0)
					{
						fac=800/lcddev.height;//�õ���������
						OV5640_OutSize_Set((1280-fac*lcddev.width)/2,(800-fac*lcddev.height)/2,lcddev.width,lcddev.height); 	 
 					}else 
					{
						OV5640_OutSize_Set(16,4,lcddev.width,outputheight);
 					}	
					if(lcddev.height>800)OV5640_WR_Reg(0x3035,0X51);//�������֡�ʣ�������ܶ���
				}
				if(res)//��������
				{
					Show_Str(30,130,240,16,"д���ļ�����!",16,0);		 
				}else 
				{
					Show_Str(30,130,240,16,"���ճɹ�!",16,0);
					Show_Str(30,150,240,16,"����Ϊ:",16,0);
					Show_Str(30+56,150,240,16,pname,16,0);		    
					PCF8574_WriteBit(BEEP_IO,0);	//�������̽У���ʾ�������
					delay_ms(100);
					PCF8574_WriteBit(BEEP_IO,1);	//�رշ�����
				}  
				delay_ms(1000);		//�ȴ�1����	
				DCMI_Start();		//������ʹ��dcmi,Ȼ�������ر�DCMI,�����ٿ���DCMI,���Է�ֹRGB���Ĳ�������.
				DCMI_Stop();			
			}else //��ʾSD������
			{					    
				Show_Str(30,130,240,16,"SD������!",16,0);
				Show_Str(30,150,240,16,"���չ��ܲ�����!",16,0);			    
			}   		
			if(key!=KEY2_PRES)DCMI_Start();//��ʼ��ʾ  
		} 
		delay_ms(10);
		i++;
		if(i==20)//DS0��˸.
		{
			i=0;
			LED0=!LED0;
 		}
	}
}






