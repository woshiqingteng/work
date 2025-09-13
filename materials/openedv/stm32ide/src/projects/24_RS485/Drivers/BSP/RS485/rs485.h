/**
 ****************************************************************************************************
 * @file        rs485.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-24
 * @brief       RS485 ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200424
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __RS485_H
#define __RS485_H

#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/

/* ����RS485_RE��, ����RS485����/����״̬
 * RS485_RE = 0, �������ģʽ
 * RS485_RE = 1, ���뷢��ģʽ
 */
#define RS485_RE(x)   do{ x ? \
                          HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_RESET); \
                      }while(0)


#define RS485_REC_LEN               64          /* �����������ֽ��� 64 */

extern uint8_t g_RS485_rx_buf[RS485_REC_LEN];   /* ���ջ���,���RS485_REC_LEN���ֽ� */
extern uint8_t g_RS485_rx_cnt;                  /* �������ݳ��� */


void rs485_init( uint32_t baudrate);  /* RS485��ʼ�� */
void rs485_send_data(uint8_t *buf, uint8_t len);    /* RS485�������� */
void rs485_receive_data(uint8_t *buf, uint8_t *len);/* RS485�������� */

#endif
















