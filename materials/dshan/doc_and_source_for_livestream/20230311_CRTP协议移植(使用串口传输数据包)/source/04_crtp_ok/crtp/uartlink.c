/*
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2012 BitCraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * uartlink.c
 */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cmsis_os.h"

#include "crtp.h"

static bool isInit = false;
static xQueueHandle crtpPacketDelivery;
static xQueueHandle uartRecvQueue;
static TaskHandle_t pxUARTParserTask;

static int uartlinkSendPacket(CRTPPacket *p);
static int uartlinkSetEnable(bool enable);
static int uartlinkReceivePacket(CRTPPacket *p);


static struct crtpLinkOperations uartlinkOp =
{
  .setEnable         = uartlinkSetEnable,
  .sendPacket        = uartlinkSendPacket,
  .receivePacket     = uartlinkReceivePacket,
};

static int uartlinkSendPacket(CRTPPacket *p)
{
	int i;
	
	/* 发出头部 0x55 */
	fputc(0x55, NULL);

	/* 发出size */
	fputc(p->size, NULL);

	/* 发出header */
	fputc(p->header, NULL);

	/* 发出data */
	for (i = 0; i < p->size; i++)
		fputc(p->data[i], NULL);

	return true;	
}

static int uartlinkSetEnable(bool enable)
{
  return 0;
}

static int uartlinkReceivePacket(CRTPPacket *p)
{
  if (xQueueReceive(crtpPacketDelivery, p, 100) == pdTRUE)
  {
    return 0;
  }

  return -1;
}

static void UARTParserPacket( void * params)
{
	unsigned char c;
	CRTPPacket packet;
	enum recv_status {
		WAITING = 0,
		GET_SIZE,
		GET_HEADER,
		GET_DATA,
	} status;

	int data_cnt;

	status = WAITING;
	
	while (1)
	{
		/* 平时等待数据 
		 * 测试序列: 55 07 F0 31 30 30 61 73 6B 00
	     */
		xQueueReceive(uartRecvQueue, &c, portMAX_DELAY);

		/* 解析出packet */
		switch (status)
		{
			case WAITING:
			{
				if (c != 0x55)
				{
					status = GET_SIZE;
					packet.size = c;
					status = GET_HEADER;
				}
				break;
			}

			case GET_HEADER:
			{
				packet.header = c;
				data_cnt = 0;
				status = GET_DATA;
				break;
			}

			case GET_DATA:
			{
				packet.data[data_cnt++] = c;
				if (data_cnt == packet.size)
				{
					/* 把Packet写入队列 */
					//printf("Get Packet: channel = %d, port = %d, data = %s\r\n", packet.channel, packet.port, packet.data);
					xQueueSend(crtpPacketDelivery, &packet, 100);
					status = WAITING;
				}
				break;
			}

		}
		

	}

}


/*
 * Public functions
 */

void StoreUARTDataInISR(unsigned char c)
{
	xQueueSendFromISR(uartRecvQueue, &c, NULL);
}

void uartlinkInit()
{
  if(isInit)
    return;

  crtpPacketDelivery = xQueueCreate(5, sizeof(CRTPPacket));
  uartRecvQueue = xQueueCreate(64, 1);

  // 创建一个"解析数据包的任务"
  
  xTaskCreate(UARTParserPacket, "ParserTask", 200, NULL, osPriorityNormal, &pxUARTParserTask);

  isInit = true;
}

struct crtpLinkOperations * uartlinkGetLink()
{
  return &uartlinkOp;
}
