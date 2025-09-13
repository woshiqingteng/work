#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "sdram.h"
#include "pcf8574.h"
#include "timer.h" 
#include "ov5640.h"
#include "usart2.h"
#include "dcmi.h"
#include "usmart.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��37
 ����ͷʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

u8 ovx_mode=0;							//bit0:0,RGB565ģʽ;1,JPEGģʽ 
u16 curline=0;							//����ͷ�������,��ǰ�б��
u16 yoffset=0;							//y�����ƫ����

#define jpeg_buf_size   30*1024*1024		//����JPEG���ݻ���jpeg_buf�Ĵ�С(1*4M�ֽ�)
#define jpeg_line_size	2*1024			//����DMA��������ʱ,һ�����ݵ����ֵ

u32 dcmi_line_buf[2][jpeg_line_size] ;	//RGB��ʱ,����ͷ����һ��һ�ж�ȡ,�����л���  
u32 jpeg_data_buf[jpeg_buf_size] __attribute__((at(0XC0000000+1280*800*2)));//JPEG���ݻ���buf,������LCD֡����֮��

volatile u32 jpeg_data_len=0; 			//buf�е�JPEG��Ч���ݳ��� 
volatile u8 jpeg_data_ok=0;				//JPEG���ݲɼ���ɱ�־ 
										//0,����û�вɼ���;
										//1,���ݲɼ�����,���ǻ�û����;
										//2,�����Ѿ����������,���Կ�ʼ��һ֡����
		
//JPEG�ߴ�֧���б�(�����800*600,����ֱ����ٸ�,��λ������...)
const u16 jpeg_img_size_tbl[][2]=
{
	160,120,	//QQVGA
	176,144,	//QCIF
	320,240,	//QVGA
	400,240,	//WQVGA
	352,288,	//CIF
	640,480,	//VGA
	800,600,	//SVGA 
};
const u8*EFFECTS_TBL[7]={"Normal","Cool","Warm","B&W","Yellowish ","Inverse","Greenish"};	//7����Ч 
const u8*JPEG_SIZE_TBL[7]={"QQVGA","QCIF","QVGA","WQVGA","CIF","VGA","SVGA"};				//JPEGͼƬ 7�ֳߴ� 
 
//����JPEG����
//���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
void jpeg_data_process(void)
{
	u16 i;
	u16 rlen;//ʣ�����ݳ���
	u32 *pbuf;
	curline=yoffset;	//������λ
	if(ovx_mode&0X01)//ֻ����JPEG��ʽ��,����Ҫ������.
	{
		if(jpeg_data_ok==0)	//jpeg���ݻ�δ�ɼ���?
		{
            __HAL_DMA_DISABLE(&DMADMCI_Handler);//�ر�DMA
			rlen=jpeg_line_size-__HAL_DMA_GET_COUNTER(&DMADMCI_Handler);//�õ�ʣ�����ݳ���	   
			pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ,�������
			if(DMADMCI_Handler.Instance->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=dcmi_line_buf[1][i];//��ȡbuf1�����ʣ������
			else for(i=0;i<rlen;i++)pbuf[i]=dcmi_line_buf[0][i];//��ȡbuf0�����ʣ������ 
			jpeg_data_len+=rlen;			//����ʣ�೤��
			jpeg_data_ok=1; 				//���JPEG���ݲɼ����,�ȴ�������������
		}
		if(jpeg_data_ok==2)	//��һ�ε�jpeg�����Ѿ���������
		{
            __HAL_DMA_SET_COUNTER(&DMADMCI_Handler,jpeg_line_size);	//���䳤��Ϊjpeg_buf_size*4�ֽ�
			__HAL_DMA_ENABLE(&DMADMCI_Handler); //��DMA
			jpeg_data_ok=0;					    //�������δ�ɼ�
			jpeg_data_len=0;				    //�������¿�ʼ
		}
	}else
	{
		LCD_SetCursor(0,0);  
		LCD_WriteRAM_Prepare();		//��ʼд��GRAM
	}	
}

//jpeg���ݽ��ջص�����
void jpeg_dcmi_rx_callback(void)
{  
	u16 i;
	u32 *pbuf;
	pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ
	if(DMADMCI_Handler.Instance->CR&(1<<19))//buf0����,��������buf1
	{ 
		for(i=0;i<jpeg_line_size;i++)pbuf[i]=dcmi_line_buf[0][i];//��ȡbuf0���������
		jpeg_data_len+=jpeg_line_size;//ƫ��
	}else //buf1����,��������buf0
	{
		for(i=0;i<jpeg_line_size;i++)pbuf[i]=dcmi_line_buf[1][i];//��ȡbuf1���������
		jpeg_data_len+=jpeg_line_size;//ƫ�� 
	} 
}

//JPEG����
//JPEG����,ͨ������2���͸�����.
void jpeg_test(void)
{
	u32 i,jpgstart,jpglen; 
	u8 *p;
	u8 key,headok=0;
	u8 effect=0,contrast=2;
	u8 size=2;			//Ĭ����QVGA 320*240�ߴ�
	u8 msgbuf[15];		//��Ϣ������ 
	LCD_Clear(WHITE);
    POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");
	LCD_ShowString(30,70,200,16,16,"OV5640 JPEG Mode");
	LCD_ShowString(30,100,200,16,16,"KEY0:Contrast");			//�Աȶ�
	LCD_ShowString(30,120,200,16,16,"KEY1:Auto Focus"); 		//ִ���Զ��Խ�
	LCD_ShowString(30,140,200,16,16,"KEY2:Effects"); 			//��Ч 
	LCD_ShowString(30,160,200,16,16,"KEY_UP:Size");				//�ֱ������� 
	sprintf((char*)msgbuf,"JPEG Size:%s",JPEG_SIZE_TBL[size]);
	LCD_ShowString(30,180,200,16,16,msgbuf);					//��ʾ��ǰJPEG�ֱ���
	//�Զ��Խ���ʼ��
	OV5640_RGB565_Mode();	//RGB565ģʽ 
	OV5640_Focus_Init(); 
	
 	OV5640_JPEG_Mode();		//JPEGģʽ
	
	OV5640_Light_Mode(0);	//�Զ�ģʽ
	OV5640_Color_Saturation(3);//ɫ�ʱ��Ͷ�0
	OV5640_Brightness(4);	//����0
	OV5640_Contrast(3);		//�Աȶ�0
	OV5640_Sharpness(33);	//�Զ����
	OV5640_Focus_Constant();//���������Խ�
	
	DCMI_Init();			//DCMI����
	dcmi_rx_callback=jpeg_dcmi_rx_callback;//JPEG�������ݻص�����
    DCMI_DMA_Init((u32)&dcmi_line_buf[0],(u32)&dcmi_line_buf[1],jpeg_line_size,DMA_MDATAALIGN_WORD,DMA_MINC_ENABLE);  
 	OV5640_OutSize_Set(4,0,jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);//��������ߴ� 
	DCMI_Start(); 		//��������
	while(1)
	{
		if(jpeg_data_ok==1)	//�Ѿ��ɼ���һ֡ͼ����
		{  
			p=(u8*)jpeg_data_buf;
			printf("jpeg_data_len:%d\r\n",jpeg_data_len*4);//��ӡjpegͼƬ��С
			LCD_ShowString(30,210,210,16,16,"Sending JPEG data..."); //��ʾ���ڴ������� 
			jpglen=0;	//����jpg�ļ���СΪ0
			headok=0;	//���jpgͷ���
			for(i=0;i<jpeg_data_len*4;i++)//����0XFF,0XD8��0XFF,0XD9,��ȡjpg�ļ���С
			{
				if((p[i]==0XFF)&&(p[i+1]==0XD8))//�ҵ�FF D8
				{
					jpgstart=i;
					headok=1;	//����ҵ�jpgͷ(FF D8)
				}
				if((p[i]==0XFF)&&(p[i+1]==0XD9)&&headok)//�ҵ�ͷ�Ժ�,����FF D9
				{
					jpglen=i-jpgstart+2;
					break;
				}
			}
			if(jpglen)	//������jpeg���� 
			{
				p+=jpgstart;			//ƫ�Ƶ�0XFF,0XD8�� 
				for(i=0;i<jpglen;i++)	//��������jpg�ļ�
				{
					USART2->DR=p[i];  
					while((USART2->SR&0X40)==0);	//ѭ������,ֱ���������   
					
					key=KEY_Scan(0); 
					if(key)break;
				}  
			}
			if(key)	//�а�������,��Ҫ����
			{  
				LCD_ShowString(30,210,210,16,16,"Quit Sending data   ");//��ʾ�˳����ݴ���
				switch(key)
				{				    
					case KEY0_PRES:	//�Աȶ�����
						contrast++;
						if(contrast>6)contrast=0;
						OV5640_Contrast(contrast);
						sprintf((char*)msgbuf,"Contrast:%d",(signed char)contrast-3);
						break; 
					case KEY1_PRES:	//ִ��һ���Զ��Խ�
						OV5640_Focus_Single();
						break;
					case KEY2_PRES:	//��Ч����				 
						effect++;
						if(effect>6)effect=0;
						OV5640_Special_Effects(effect);//������Ч
						sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
						break;
					case WKUP_PRES:	//JPEG����ߴ�����   
						size++;  
						if(size>6)size=0;   
						OV5640_OutSize_Set(4,0,jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);//��������ߴ�  
						sprintf((char*)msgbuf,"JPEG Size:%s",JPEG_SIZE_TBL[size]);
						break;
				}
				LCD_Fill(30,180,239,190+16,WHITE);
				LCD_ShowString(30,180,210,16,16,msgbuf);//��ʾ��ʾ����
				delay_ms(800); 				  
			}else LCD_ShowString(30,210,210,16,16,"Send data complete!!");//��ʾ����������� 
			jpeg_data_ok=2;	//���jpeg���ݴ�������,������DMAȥ�ɼ���һ֡��.
		}		
	}    
}

//RGB�����ݽ��ջص�����
void rgblcd_dcmi_rx_callback(void)
{  
	u16 *pbuf;
	if(DMADMCI_Handler.Instance->CR&(1<<19))//DMAʹ��buf1,��ȡbuf0
	{ 
		pbuf=(u16*)dcmi_line_buf[0]; 
	}else 						//DMAʹ��buf0,��ȡbuf1
	{
		pbuf=(u16*)dcmi_line_buf[1]; 
	} 	
	LTDC_Color_Fill(0,curline,lcddev.width-1,curline,pbuf);//DM2D��� 
	if(curline<lcddev.height)curline++;
}

//RGB565����
//RGB����ֱ����ʾ��LCD����
void rgb565_test(void)
{ 
	u8 key;
	u8 effect=0,contrast=2,fac;
	u8 scale=1;		//Ĭ����ȫ�ߴ�����
	u8 msgbuf[15];	//��Ϣ������
	u16 outputheight=0;	
	LCD_Clear(WHITE);
    POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");
	LCD_ShowString(30,70,200,16,16,"OV5640 RGB565 Mode");
	
	LCD_ShowString(30,100,200,16,16,"KEY0:Contrast");			//�Աȶ�
	LCD_ShowString(30,120,200,16,16,"KEY1:Auto Focus"); 		//ִ���Զ��Խ�
	LCD_ShowString(30,140,200,16,16,"KEY2:Effects"); 			//��Ч 
	LCD_ShowString(30,160,200,16,16,"KEY_UP:FullSize/Scale");	//1:1�ߴ�(��ʾ��ʵ�ߴ�)/ȫ�ߴ�����
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
         DCMI_DMA_Init((u32)dcmi_line_buf[0],(u32)dcmi_line_buf[1],lcddev.width/2,DMA_MDATAALIGN_HALFWORD,DMA_MINC_ENABLE);
	}else					//MCU ��
	{
        DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MDATAALIGN_HALFWORD,DMA_MINC_DISABLE);
	}
	TIM3->CR1&=~(0x01);		//�رն�ʱ��3,�ر�֡��ͳ�ƣ��򿪵Ļ���RGB�����ڴ��ڴ�ӡ��ʱ�򣬻ᶶ
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
	OV5640_OutSize_Set(4,0,lcddev.width,outputheight);		//����������ʾ
	DCMI_Start(); 			//��������
	LCD_Clear(BLACK);
	while(1)
	{ 
		key=KEY_Scan(0); 
		if(key)
		{ 
			if(key!=KEY1_PRES)DCMI_Stop(); //��KEY1����,ֹͣ��ʾ
			switch(key)
			{				    
				case KEY0_PRES:	//�Աȶ�����
					contrast++;
					if(contrast>6)contrast=0;
					OV5640_Contrast(contrast);
					sprintf((char*)msgbuf,"Contrast:%d",(signed char)contrast-3);
					break;
				case KEY1_PRES:	//ִ��һ���Զ��Խ�
					OV5640_Focus_Single();
					break;
				case KEY2_PRES:	//��Ч����				 
					effect++;
					if(effect>6)effect=0;
					OV5640_Special_Effects(effect);//������Ч
					sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
					break;
				case WKUP_PRES:	//1:1�ߴ�(��ʾ��ʵ�ߴ�)/����	    
					scale=!scale;
					if(scale==0)
					{
						fac=800/outputheight;	//�õ���������
 						OV5640_OutSize_Set((1280-fac*lcddev.width)/2,(800-fac*outputheight)/2,lcddev.width,outputheight); 	 
						sprintf((char*)msgbuf,"Full Size 1:1");
					}else 
					{
						OV5640_OutSize_Set(4,0,lcddev.width,outputheight);
 						sprintf((char*)msgbuf,"Scale");
					}
					break;
			}
			if(key!=KEY1_PRES)	//��KEY1����
			{
				LCD_ShowString(30,50,210,16,16,msgbuf);//��ʾ��ʾ����
				delay_ms(800); 
				DCMI_Start();	//���¿�ʼ����
			}
		} 
		delay_ms(10);		
	}    
} 
int main(void)
{
	u8 key;
	u8 t;
	HAL_Init();                     //��ʼ��HAL��   
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
	delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART
	usart2_init(921600);            //��ʼ��USART2
	usmart_dev.init(90); 		    //��ʼ��USMART	
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	SDRAM_Init();                   //��ʼ��SDRAM
	LCD_Init();                     //��ʼ��LCD
	TIM3_Init(10000-1,9000-1);      //10Khz����,1�����ж�һ��
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"OV5640 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/1/16");  	 
	while(OV5640_Init())//��ʼ��OV5640
	{
		LCD_ShowString(30,130,240,16,16,"OV5640 ERR");
		delay_ms(200);
	    LCD_Fill(30,130,239,170,WHITE);
		delay_ms(200);
        LED0=!LED0;
	}	
    LCD_ShowString(30,130,200,16,16,"OV5640 OK");  
    while(1)
    {
       key=KEY_Scan(0);
		if(key==KEY0_PRES){ovx_mode=0;break;}		//RGB565ģʽ
	    else if(key==KEY1_PRES){ovx_mode=1;break;}	//JPEGģʽ 
		t++; 									  
		if(t==100)LCD_ShowString(30,150,230,16,16,"KEY0:RGB565  KEY1:JPEG"); //��˸��ʾ��ʾ��Ϣ
 		if(t==200)
		{	
			LCD_Fill(30,150,210,150+16,WHITE);
			t=0; 
			LED0=!LED0;
		}
		delay_ms(5);	  
    }
	if(ovx_mode==1)jpeg_test();
	else rgb565_test(); 
}
