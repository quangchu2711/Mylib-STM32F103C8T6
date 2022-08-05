#include "HY-SRF05.h"

static volatile uint32_t trig_time = 0;
static volatile uint32_t srf05_time = 0;
static uint8_t srf05_flag = 0;
static float hysrf05_distance;

void HYSRF05_Init(void)
{
	HYSRF05_TrigConfig();
	HYSRF05_EchoConfig();
}

void HYSRF05_TrigConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void HYSRF05_EchoConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  EXTI_ClearITPendingBit(EXTI_Line7);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
}

float HYSRF05_GetDistance(void)
{
	if (srf05_flag) 
	{
		hysrf05_distance = Constant_HYSRF05 * srf05_time;
		if (hysrf05_distance <= Min_Distance)
		{
			hysrf05_distance = Min_Distance;
		}
		else if (hysrf05_distance >= Max_Distance) 
		{
			hysrf05_distance = Max_Distance;
		}
	}
	else hysrf05_distance = 0;
	srf05_flag = 0;
	return hysrf05_distance;
}

void HYSRF05_TtigStart(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	DelayTimer_Us(ECHOPIN_LOW_TIME);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	DelayTimer_Us(ECHOPIN_HIGH_TIME);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void HYSRF05_IRQHandler(void)
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
	{
		srf05_time = Timer_GetTickUs();
	}
	else
	{
		srf05_time = Timer_GetTickUs() - srf05_time;
		srf05_flag = 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line7); 
}
