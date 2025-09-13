/**
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2011-2021 Bitcraze AB
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
 * crtpservice.c - Implements low level services for CRTP
 */

#include <stdbool.h>
#include <string.h>

/* FreeRtos includes */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "crtp.h"
#include "ledservice.h"
#include "static_mem.h"
#include "param.h"


typedef enum {
  ledSet   = 0x00,
  ledGet = 0x01,
} LinkNbr;

static int led0_status;
static bool isInit=false;

static void ledSrvTask(void* params)
{
  static CRTPPacket p;

  crtpInitTaskQueue(CRTP_PORT_LED);

  while(1) {
	crtpReceivePacketBlock(CRTP_PORT_LED, &p);

	switch (p.channel)
	{
	  case ledSet:
	  	/* 根据参数控制LED */
		led0_status = p.data[1];
	    printf("set led %d as %s\r\n", p.data[0], led0_status ? "on" : "off");
		break;
	  case ledGet:
	  	if (p.data[0] == 0)
	  	{
			p.size  = 2;			
			p.data[1] = led0_status;
			crtpSendPacketBlock(&p);
	  	}
		break;
	  default:
		break;
	}
  }
}


void ledserviceInit(void)
{
  if (isInit)
    return;

  //Start the task
  xTaskCreate(ledSrvTask, "ledSrvTask", 200, NULL, osPriorityNormal, NULL);

  isInit = true;
}

