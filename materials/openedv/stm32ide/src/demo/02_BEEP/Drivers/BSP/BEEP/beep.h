/*
 * beep.h
 *
 *  Created on: Dec 13, 2023
 *      Author: login
 */

#ifndef BSP_BEEP_BEEP_H_
#define BSP_BEEP_BEEP_H_

#include "main.h"

#define BEEP(x)				x? 	HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET) : \
								HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET)

#endif /* BSP_BEEP_BEEP_H_ */
