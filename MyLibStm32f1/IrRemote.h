/**
  ******************************************************************************
  * @file    IrRemote.h
  * @author  Chu Xuan Quang
  * @date    04-02-2022
  * @brief   IrRemote program body
  ******************************************************************************
  ******************************************************************************
  * @attention on the stm32f103 kit only the following pins can be used:
	*						 PA5, PA6, PA7, PA8, PA9
	*						 PB5, PB6, PB7, PB8, PB9
  ******************************************************************************
*/
#ifndef IREMOTE_H
#define IREMOTE_H

#include <stdbool.h>
#include "stm32f10x.h"

#define FALSE (bool)0
#define TRUE 	(bool)1
#define initPulseTime_1 50
#define initPulseTime_2 150

void IrRemote_Config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X);
bool IrRemote_CheckFlags(void);
void IrRemote_ClearFlags(void);
uint8_t IrRemote_GetData(void);
	
static void Tim2_UPCount_Init(uint16_t Prescaler, uint16_t Period);
static void IR_Pin_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
static uint8_t PinSource_X(uint16_t GPIO_Pin_X);
static uint8_t Ir_Server(void);

#endif
