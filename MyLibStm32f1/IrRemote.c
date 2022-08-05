#include "IrRemote.h"

volatile uint32_t ucTim2Flag;
uint8_t irdata[33];   												//Used to record the time between two falling edges
bool receiveComplete; 												//Receive Complete Flag Bits
uint8_t idx;          												//Number received for indexing
bool startflag;       												//Indicates start of reception
uint8_t remote_code[4];
uint32_t EXTI_Line_X;

void IrRemote_Config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X)
{
	IR_Pin_Init(GPIOx, GPIO_Pin_X);
	Tim2_UPCount_Init(SystemCoreClock/1000000-1,100-1);    //timer2 interrupt time is 100 us
}

bool IrRemote_CheckFlags(void)
{
	return receiveComplete;
}

void IrRemote_ClearFlags(void)
{
	receiveComplete = FALSE;
}

uint8_t IrRemote_GetData(void)
{
	uint8_t data_receive = Ir_Server();
	return data_receive;
}

static void Tim2_UPCount_Init(uint16_t Prescaler, uint16_t Period)
{
	TIM_TimeBaseInitTypeDef TIM_StructInit;
	NVIC_InitTypeDef NVIC_StructInit;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_StructInit.TIM_Period = Period;
	TIM_StructInit.TIM_Prescaler = Prescaler;
	TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_StructInit.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_StructInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_StructInit);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);    
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

	NVIC_StructInit.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_StructInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_StructInit.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_StructInit.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_StructInit);
}

static uint8_t PinSource_X(uint16_t GPIO_Pin_X)
{
    uint8_t cnt= 0;
    while(GPIO_Pin_X)
    {
        GPIO_Pin_X = GPIO_Pin_X >> 1;
        cnt++;
    }
    return cnt-1;
}

static void IR_Pin_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_X)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	if (GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_X;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOx,&GPIO_InitStructure);

	EXTI_Line_X = (uint32_t)GPIO_Pin_X; 
	EXTI_ClearITPendingBit(EXTI_Line_X);
	if (GPIOx == GPIOA) GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, PinSource_X(GPIO_Pin_X)); 
	else if (GPIOx == GPIOB) GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, PinSource_X(GPIO_Pin_X)); 
	
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); 
	EXTI_InitStructure.EXTI_Line=EXTI_Line_X;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);
}
static uint8_t Ir_Server(void)
{
	uint8_t i,j,idx=1; //idx starts with 1 to indicate that the synchronization header time is not handled
  uint8_t temp;
  for(i=0; i<4; i++)
  {
		for(j=0; j<8; j++)
		{
			if(irdata[idx] >= 8 && irdata[idx] < 15)   //Represents 0
			{
				temp = 0;
			}
			else if(irdata[idx] >= 18 && irdata[idx]< 25) //Represents 1
			{
				temp = 1;
			}
			remote_code[i] <<= 1;
			remote_code[i] |= temp;
			idx++;
		}
  }
  return remote_code[2];  // The array records control codes, each key is different
}

void EXTI9_5_IRQHandler(void)
{
  uint16_t ir_time;
  if(startflag)
  {
		ir_time = ucTim2Flag; 
		if(ucTim2Flag < initPulseTime_2 && ucTim2Flag >= initPulseTime_1 ) // Receive Sync Header
		{
			idx = 0;  // Array subscript zeroing
		}

		irdata[idx] = ucTim2Flag;  // Get Count Time
		ucTim2Flag = 0;            // Zero count time for next count
		idx++;                     // Received a data, index plus 1

		if(idx == 33)       // If you receive 33 bits of data, including 32 bits and with a sync header
		{
				idx = 0;
				ucTim2Flag = 0;
				receiveComplete = TRUE;
		}
  }
  else   // Drop Edge First Trigger
  {
		idx = 0;
		ucTim2Flag = 0;
		startflag = TRUE;
  }
	
	EXTI_ClearITPendingBit(EXTI_Line_X);  // Clear interrupt flag
}

void TIM2_IRQHandler(void)
{
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  ucTim2Flag++;
}
