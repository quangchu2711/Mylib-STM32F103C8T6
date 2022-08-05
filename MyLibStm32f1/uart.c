#include"uart.h"

USART_TypeDef *UARTx;
uint32_t GPIO_UART_CLOCK;

uint32_t GPIO_CLOCK;
uint32_t UART_CLOCK;

GPIO_TypeDef* GPIOx;

void uart_begin(uint16_t baud, uint16_t tx_pin, uint16_t rx_pin)
{	
	if (tx_pin == GPIO_Pin_9 && rx_pin == GPIO_Pin_10)
	{
		UARTx = USART1;
		GPIOx = GPIOA;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE );		
	}
	else if (tx_pin == GPIO_Pin_2 && rx_pin == GPIO_Pin_3)
	{
		UARTx = USART2;
		GPIOx = GPIOA;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE );
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );		
	}
	else if (tx_pin == GPIO_Pin_10 && rx_pin == GPIO_Pin_11)
	{
		UARTx = USART3;
		GPIOx = GPIOB;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE );
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE );		
	}
	
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = rx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOx, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = tx_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOx, &GPIO_InitStructure );

	USART_InitStructure.USART_BaudRate            = baud;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UARTx, &USART_InitStructure);
	USART_Cmd(UARTx, ENABLE); //kich hoat usart1	
}

static void uart_send_char(USART_TypeDef *USARTx, char data)
{
    
	USARTx->DR = 0x00;
	USART_SendData(USARTx, data);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

static void uart_send_string(USART_TypeDef *USARTx, char *Str)
{
	while(*Str)
	{
			uart_send_char(USARTx, *Str);
			Str++;
	}
}

void uart_printf(const char* str, ...)
{
	char stringArray[100];
  va_list args;
  va_start(args, str);
  vsprintf(stringArray, str, args);
  va_end(args);
	uart_send_string(UARTx,stringArray);	
}
