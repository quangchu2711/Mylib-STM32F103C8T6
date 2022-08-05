/**
  ******************************************************************************
  * @file    uart.h
  * @author  Chu Xuan Quang
  * @date    29/06/2022
  * @brief   
  ******************************************************************************
  * @attention 	UART1(TX: PA9, RX PA10), 
								UART2(TX:PA2, RX: PA3)
								UART3(TX: PB10, RX: PB11)
  ******************************************************************************
*/
#ifndef UART_H
#define UART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdarg.h>
#include <stdint.h>

void uart_begin(uint16_t baud, uint16_t tx_pin, uint16_t rx_pin);
void uart_printf(const char* str, ...);

//static
static void uart_send_char(USART_TypeDef *USARTx, char data);
static void uart_send_string(USART_TypeDef *USARTx, char *Str);

#endif 
