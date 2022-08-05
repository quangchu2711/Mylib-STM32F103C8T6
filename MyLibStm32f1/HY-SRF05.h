/**
  ******************************************************************************
  * @file    HY-SRF05.h
  * @author  Chu Xuan Quang
  * @date    05-02-2022
  * @brief   HY-SRF05 program body
  ******************************************************************************
  * @attention 
  ******************************************************************************
*/
#ifndef HY_SRF05_H
#define HY_SRF05_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "DelayTimer.h"

#define HYSRF05_IRQHandler		EXTI9_5_IRQHandler
#define ECHOPIN_HIGH_TIME			15
#define ECHOPIN_LOW_TIME			2
#define Constant_HYSRF05 			0.017 //S = v*t = v*(T/2)= 340*100/10^6*( T/2) = 0.017*T (cm)
#define Min_Distance 					2.0
#define Max_Distance 					400.0


void HYSRF05_Init(void);
void HYSRF05_TtigStart(void);
float HYSRF05_GetDistance(void);

static void HYSRF05_TrigConfig(void);
static void HYSRF05_EchoConfig(void);

#endif
