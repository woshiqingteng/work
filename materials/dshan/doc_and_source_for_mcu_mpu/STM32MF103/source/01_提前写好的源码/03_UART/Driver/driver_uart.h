#ifndef __DRIVER_UART_H
#define __DRIVER_UART_H

#include "stm32f103ze.h"

#define UART_PORT                   GPIOA
#define UART_TX_PIN                 GPIO_PIN_9
#define UART_RX_PIN                 GPIO_PIN_10

extern void UART_Init(uint32_t baudrate);
extern void UART_Transmit(uint8_t *pdata, uint16_t len);
extern void UART_Receive(uint8_t *pdata, uint16_t len);

#endif