/*
 * led.h
 *
 *  Created on: Dec 12, 2023
 *      Author: login
 */

#ifndef BSP_LED_LED_H_
#define BSP_LED_LED_H_
#include "main.h"

#define LED0(x)				x? 	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET) : \
								HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)

#define LED1(x)				x? 	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET) : \
								HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#endif /* BSP_LED_LED_H_ */
