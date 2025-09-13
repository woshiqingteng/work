#ifndef __TCP_SERVER_DEMO_H
#define __TCP_SERVER_DEMO_H
#include "sys.h"
#include "includes.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//��������ͷ����   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/2/24
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	  

#define NETCAM_FIFO_NUM			100			//����FIFO����
#define NETCAM_LINE_SIZE		4096		//�����д�С(*4�ֽ�)

INT8U netcamera_init(void);		            //��������ͷ��ʼ��(������������ͷ�߳�
u8 netcam_fifo_read(u32 **buf);
u8 netcam_fifo_write(u32 *buf);
u8 netmem_malloc(void);
void netmem_free(void);
u8 camera_init(void);
void camera_deinit(void);
void netcam_resolu_set(u8 resol);

#endif

